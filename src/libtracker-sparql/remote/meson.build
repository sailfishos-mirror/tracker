remote_files = files(
    'tracker-http.c',
    'tracker-remote-statement.c',
    'tracker-remote-namespaces.c',
    'tracker-remote.c',
)

module_sources = files('tracker-http-module.c')

http_common_deps = [avahi_glib, avahi_client]


libtracker_http_soup3 = shared_module('tracker-http-soup3',
    module_sources,
    dependencies: [libsoup3, http_common_deps],
    c_args: tracker_c_args + [
        '-include', join_paths(build_root, 'config.h'),
        '-include', '../tracker-enums-private.h',
        '-DMODULE',
    ],
    install: true,
    install_dir: tracker_internal_libs_dir,
    name_suffix: 'so',
)
libtracker_sparql_modules += libtracker_http_soup3
