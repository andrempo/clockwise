import os
import re
from pathlib import Path

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.components import time
from esphome.const import CONF_ID
from esphome.core import CORE
from esphome.helpers import write_file_if_changed

clockwise_component_ns = cg.esphome_ns.namespace('clockwise_component')
ClockwiseComponent = clockwise_component_ns.class_('ClockwiseComponent', cg.Component)

# Pending files to be (re)written to build tree after copy_src_tree.
# copy_src_tree() walks <build>/src/esphome and deletes any file not in
# component.resources. Our wrappers/registry are generated, not static
# resources, so the first write in to_code() would be deleted by the later
# copy_src_tree() call in writer.write_cpp(). We work around this by
# monkey-patching copy_src_tree to re-write our files afterwards, so they
# live in the same folder as the rest of ESPHome's generated code
# (<build>/src/esphome/components/clockwise/ == alongside main.cpp/esphome.h)
# without needing to stage them in the source component dir.
_pending_build_files: dict[Path, str] = {}


def _ensure_writer_patched():
    try:
        import esphome.writer as writer
    except ImportError:
        return
    if getattr(writer.copy_src_tree, "_clockwise_patched", False):
        return
    orig = writer.copy_src_tree

    def patched_copy_src_tree():
        # Run original (copies static resources + may delete our earlier writes)
        result = orig()
        # Re-create our generated files in the build tree
        for p, content in _pending_build_files.items():
            p.parent.mkdir(parents=True, exist_ok=True)
            write_file_if_changed(p, content)
        return result

    patched_copy_src_tree._clockwise_patched = True  # type: ignore[attr-defined]
    writer.copy_src_tree = patched_copy_src_tree  # type: ignore[assignment]

MATRIX_DISPLAY_CONFIG = 'matrix_display'
TIME_CONFIG = 'time'

# Clockfaces configuration
CONFIG_CLOCKFACES = 'clockfaces'
CONF_NAME = 'name'
CONF_SOURCE = 'source'
CONF_NEEDS_DOUBLE_BUFFER = 'needs_double_buffer'

CLOCKFACE_SCHEMA = cv.Schema({
    cv.Required(CONF_NAME): cv.string,
    cv.Required(CONF_SOURCE): cv.string,
    cv.Optional(CONF_NEEDS_DOUBLE_BUFFER, default=False): cv.boolean,
})

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ClockwiseComponent),
    cv.Required(MATRIX_DISPLAY_CONFIG): cv.use_id(display.Display),
    cv.Required(TIME_CONFIG): cv.use_id(time.RealTimeClock),
    cv.Optional(CONFIG_CLOCKFACES, default=[]): cv.ensure_list(CLOCKFACE_SCHEMA),
}).extend(cv.COMPONENT_SCHEMA)


def safe_name(name):
    """Convert a clockface name to a valid C++ identifier."""
    result = re.sub(r'[^a-zA-Z0-9]', '_', name)
    if result and result[0].isdigit():
        result = '_' + result
    return result or '_'


def _discover_cpp_files(source_path):
    """Recursively discover all .cpp files in a directory, sorted for determinism."""
    cpp_files = []
    if not os.path.isdir(source_path):
        return cpp_files
    for root, dirs, files in os.walk(source_path):
        # Exclude tooling and VCS dirs from traversal and from results
        dirs[:] = [d for d in dirs if d not in ('tools', '.git', '.claude', '.superpowers', 'docs', '.pio', 'build', '.esphome')]
        if '/tools/' in root or root.endswith('/tools'):
            continue
        dirs.sort()
        for f in sorted(files):
            if f.endswith('.cpp'):
                rel_path = os.path.relpath(os.path.join(root, f), source_path)
                if rel_path.startswith('tools/') or '/tools/' in rel_path:
                    continue
                cpp_files.append(rel_path)
    cpp_files.sort()
    return cpp_files


def _generate_wrapper(name, source_path, needs_double_buffer=False):
    """Generate a .cpp wrapper that includes all submodule .cpp files in one TU."""
    safe = safe_name(name)
    cpp_files = _discover_cpp_files(source_path)
    if not cpp_files:
        return None
    includes = ''.join(f'#include "{os.path.join(source_path, rel_path)}"\n' for rel_path in cpp_files)

    # Common cw-gfx-engine and cw-commons headers that must remain in global namespace.
    # Pre-included here so include guards skip them when clockface code re-includes them inside the unique namespace.
    shared_headers = [
        '<Arduino.h>',
        '<functional>',
        '<Object.h>',
        '<Locator.h>',
        '<EventBus.h>',
        '<EventTask.h>',
        '<Game.h>',
        '<Sprite.h>',
        '<Tile.h>',
        '<Macros.h>',
        '<ColorUtil.h>',
        '<ImageUtils.h>',
    ]
    shared_includes = '\n'.join(f'#include {h}' for h in shared_headers)

    return f'''// Auto-generated wrapper for clockface "{name}"
// Source: {source_path}
#include "IClockface.h"
#include "CWDateTime.h"
#include "clockface_manager.h"
#include <Adafruit_GFX.h>

// Pre-include shared library headers so they remain in global namespace.
// Include guards skip them when clockface code includes them inside the unique namespace below.
{shared_includes}

// Unique namespace prevents linker collision on class Clockface
// when multiple wrappers are compiled in the same project.
namespace {safe}_detail {{
{includes}
}}  // namespace {safe}_detail

class Face_{safe} : public IClockface {{
    {safe}_detail::Clockface _impl;

public:
    Face_{safe}(Adafruit_GFX* gfx) : _impl(gfx) {{}}
    void setup(CWDateTime* dt) override {{ _impl.setup(dt); }}
    void update() override {{ _impl.update(); }}
    bool needsDoubleBuffer() const override {{ return {"true" if needs_double_buffer else "false"}; }}
}};

// Registration function called by clockface_registry.cpp
void registerFace_{safe}(ClockfaceManager& mgr, Adafruit_GFX* gfx) {{
    mgr.registerFace(new Face_{safe}(gfx), "{name}");
}}
'''


def _generate_registry_header(entries, wrapper_sources):
    """Generate clockface_registry.h -- declares the registration fn only."""
    lines = [
        '// Auto-generated clockface registry',
        '#pragma once',
        '',
        '#include "clockface_manager.h"',
        '#include <Adafruit_GFX.h>',
    ]
    lines.append('')
    lines.append('void registerAllFaces(ClockfaceManager& mgr, Adafruit_GFX* gfx);')
    return '\n'.join(lines) + '\n'


def _generate_registry_source(entries):
    """Generate clockface_registry.cpp -- calls per-wrapper registration functions."""
    lines = [
        '// Auto-generated clockface registry implementation',
        '#include "clockface_registry.h"',
    ]
    for name, var_name in entries:
        lines.append(f'void registerFace_{var_name}(ClockfaceManager& mgr, Adafruit_GFX* gfx);')
    lines.append('')
    lines.append('void registerAllFaces(ClockfaceManager& mgr, Adafruit_GFX* gfx) {')
    for name, var_name in entries:
        lines.append(f'    registerFace_{var_name}(mgr, gfx);')
    lines.append('}')
    return '\n'.join(lines) + '\n'


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    disp = await cg.get_variable(config[MATRIX_DISPLAY_CONFIG])
    cg.add(var.set_matrix_display(disp))
    time_rtc = await cg.get_variable(config[TIME_CONFIG])
    cg.add(var.set_time(time_rtc))

    # Process clockfaces
    clockface_configs = config.get(CONFIG_CLOCKFACES, [])
    registry_entries = []

    # In-repo shared libs live next to this component (firmware/lib/...).
    # Registered here from the component location so clockwise.yaml stays
    # portable (no machine-specific absolute paths in versioned config).
    _component_dir = Path(__file__).resolve().parent
    _firmware_lib = _component_dir.parent.parent.parent / "lib"
    for _lib_name in ("cw-gfx-engine", "cw-commons"):
        _lib_path = _firmware_lib / _lib_name
        if _lib_path.is_dir():
            cg.add_library(_lib_name, None, f"symlink://{_lib_path}")

    # Base directory for relative clockface sources: the yaml's own dir,
    # so sources stay valid regardless of the invoking CWD.
    _config_dir = getattr(CORE, "config_dir", None) or os.getcwd()

    for cf in clockface_configs:
        name = cf[CONF_NAME]
        source_path = cf[CONF_SOURCE]

        # Resolve symlink:// prefix
        if source_path.startswith('symlink://'):
            source_path = source_path[len('symlink://'):]

        # Expand ~ and env vars; resolve relative paths against the yaml dir
        source_path = os.path.expanduser(os.path.expandvars(source_path))
        if not os.path.isabs(source_path):
            source_path = os.path.join(str(_config_dir), source_path)
        source_path = os.path.realpath(source_path)

        # Validate source directory exists
        if not os.path.isdir(source_path):
            raise cv.Invalid(
                f"Clockface source directory does not exist: {source_path} "
                f"(configured for '{name}')"
            )

        # Generate wrapper file
        needs_db = cf.get(CONF_NEEDS_DOUBLE_BUFFER, False)
        wrapper_content = _generate_wrapper(name, source_path, needs_db)
        if wrapper_content is None:
            raise cv.Invalid(
                f"No .cpp files found in clockface source directory: {source_path} "
                f"(configured for '{name}')"
            )

        wrapper_filename = f"clockface_wrapper_{safe_name(name)}.cpp"
        # Write directly to ESPHome build tree (same folder as main.cpp / esphome.h)
        # CORE.relative_src_path("esphome/components/clockwise/...") == <build>/src/esphome/components/clockwise/...
        wrapper_path: Path = CORE.relative_src_path(
            "esphome", "components", "clockwise", wrapper_filename
        )
        wrapper_path.parent.mkdir(parents=True, exist_ok=True)
        write_file_if_changed(wrapper_path, wrapper_content)
        _pending_build_files[wrapper_path] = wrapper_content
        _ensure_writer_patched()

        # Check for gfx/ subdirectory and add its include path
        gfx_path = os.path.join(source_path, 'gfx')
        if os.path.isdir(gfx_path):
            cg.add_build_flag(f'-I{gfx_path}')

        var_name = safe_name(name)
        registry_entries.append((name, var_name))

    # Generate registry header (declaration only) - to build tree
    registry_content = _generate_registry_header(registry_entries, [])
    registry_path: Path = CORE.relative_src_path(
        "esphome", "components", "clockwise", "clockface_registry.h"
    )
    registry_path.parent.mkdir(parents=True, exist_ok=True)
    write_file_if_changed(registry_path, registry_content)
    _pending_build_files[registry_path] = registry_content
    _ensure_writer_patched()

    # Generate registry implementation (.cpp) that calls per-wrapper registration fns
    registry_impl = _generate_registry_source(registry_entries)
    registry_impl_path: Path = CORE.relative_src_path(
        "esphome", "components", "clockwise", "clockface_registry.cpp"
    )
    registry_impl_path.parent.mkdir(parents=True, exist_ok=True)
    write_file_if_changed(registry_impl_path, registry_impl)
    _pending_build_files[registry_impl_path] = registry_impl
    _ensure_writer_patched()

    await cg.register_component(var, config)
