import os
import re

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.components import time
from esphome.const import CONF_ID

clockwise_component_ns = cg.esphome_ns.namespace('clockwise_component')
ClockwiseComponent = clockwise_component_ns.class_('ClockwiseComponent', cg.Component)

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
    component_dir = os.path.dirname(os.path.realpath(__file__))
    registry_entries = []

    for cf in clockface_configs:
        name = cf[CONF_NAME]
        source_path = cf[CONF_SOURCE]

        # Resolve symlink:// prefix
        if source_path.startswith('symlink://'):
            source_path = source_path[len('symlink://'):]

        # Expand ~ and env vars
        source_path = os.path.expanduser(os.path.expandvars(source_path))
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
        wrapper_path = os.path.join(component_dir, wrapper_filename)

        with open(wrapper_path, 'w') as f:
            f.write(wrapper_content)

        # Check for gfx/ subdirectory and add its include path
        gfx_path = os.path.join(source_path, 'gfx')
        if os.path.isdir(gfx_path):
            cg.add_build_flag(f'-I{gfx_path}')

        var_name = safe_name(name)
        registry_entries.append((name, var_name))

    # Generate registry header (declaration only)
    registry_content = _generate_registry_header(registry_entries, [])
    registry_path = os.path.join(component_dir, 'clockface_registry.h')

    with open(registry_path, 'w') as f:
        f.write(registry_content)

    # Generate registry implementation (.cpp) that calls per-wrapper registration fns
    registry_impl = _generate_registry_source(registry_entries)
    registry_impl_path = os.path.join(component_dir, 'clockface_registry.cpp')

    with open(registry_impl_path, 'w') as f:
        f.write(registry_impl)

    await cg.register_component(var, config)
