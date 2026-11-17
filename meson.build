project(
    'btk',
    ['cpp', 'c'],
    version: '1.0.0',
    license: 'MIT',
    license_files: 'LICENSE',
)
btk_dep = declare_dependency(
    include_directories: include_directories('.'),
)
meson.override_dependency('btk', btk_dep)
