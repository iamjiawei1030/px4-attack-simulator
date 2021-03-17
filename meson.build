project('px4_gazebo_attack', 'cpp', version: '0.1')

gazebo = dependency('gazebo', version: '>=11.0.0')
threads = dependency('threads')

common_deps = [gazebo, threads]
plugin_path = '/plugins'

shared_library('custom_plugin', sources: 'plugins/custom_plugin.cpp', 
                dependencies: common_deps, install: true, install_dir: plugin_path)

executable('command', 'scripts/command.cpp', dependencies: common_deps, install: true, install_dir: '/bin')