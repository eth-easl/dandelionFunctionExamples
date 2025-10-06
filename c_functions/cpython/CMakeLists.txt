cmake_minimum_required(VERSION 3.16)
project(CPythonBuild LANGUAGES NONE)

include(ExternalProject)

# Default parallelism unless caller sets it
if(NOT CMAKE_BUILD_PARALLEL_LEVEL)
    set(CMAKE_BUILD_PARALLEL_LEVEL 8)
endif()

# Where CPython "make altinstall" will install
set(PYTHON_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Install prefix for CPython")

# Allow overriding the tag from the CLI: -DCPYTHON_TAG=<tag/commit>
set(CPYTHON_TAG "3.12" CACHE STRING "CPython git tag/branch/commit")

set(FROZEN_MODULES_STRING
"'<asyncio.*>', '<collections.*>', '<concurrent.*>', '<ctypes.*>', '<curses.*>', '<dbm.*>', '<email.*>', '<html.*>', '<http.*>', '<idlelib.*>', '<json.*>', '<logging.*>', '<multiprocessing.*>', '<pydoc_data.*>', '<sqlite3.*>', '<tkinter.*>', '<tomllib.*>', '<turtledemo.*>', '<urllib.*>', '<venv.*>', '<wsgiref.*>', '<xml.*>', '<xmlrpc.*>', '<zipfile.*>', '<zoneinfo.*>', '__future__', '_aix_support', '_compat_pickle', '_compression', '_markupbase', '_py_abc', '_pydatetime', '_pyio', '_pylong', '_strptime', '_threading_local', 'antigravity', 'argparse', 'ast', 'base64', 'bdb', 'bisect', 'bz2', 'cProfile', 'calendar', 'cmd', 'code', 'codeop', 'colorsys', 'compileall', 'configparser', 'contextlib', 'contextvars', 'copy', 'copyreg', 'csv', 'dataclasses', 'datetime', 'decimal', 'difflib', 'dis', 'enum', 'filecmp', 'fileinput', 'fnmatch', 'fractions', 'ftplib', 'functools', 'getopt', 'getpass', 'gettext', 'glob', 'graphlib', 'gzip', 'hashlib', 'heapq', 'hmac', 'imaplib', 'inspect', 'ipaddress', 'keyword', 'linecache', 'locale', 'lzma', 'mailbox', 'mimetypes', 'modulefinder', 'netrc', 'nturl2path', 'numbers', 'opcode', 'operator', 'optparse', 'pdb', 'pickle', 'pickletools', 'pkgutil', 'platform', 'plistlib', 'poplib', 'pprint', 'profile', 'pstats', 'pty', 'py_compile', 'pyclbr', 'pydoc', 'queue', 'quopri', 'random', 'reprlib', 'rlcompleter', 'sched', 'secrets', 'selectors', 'shelve', 'shlex', 'shutil', 'signal', 'smtplib', 'socket', 'socketserver', 'sre_compile', 'sre_constants', 'sre_parse', 'ssl', 'statistics', 'string', 'stringprep', 'struct', 'subprocess', 'symtable', 'tabnanny', 'tarfile', 'tempfile', 'textwrap', 'this', 'threading', 'timeit', 'token', 'tokenize', 'trace', 'traceback', 'tracemalloc', 'tty', 'turtle', 'types', 'typing', 'uuid', 'warnings',"
)

ExternalProject_Add(cpython
        PREFIX       "${CMAKE_BINARY_DIR}/cpython"
        SOURCE_DIR   "${CMAKE_BINARY_DIR}/cpython/src"
        BINARY_DIR   "${CMAKE_BINARY_DIR}/cpython/build"
        STAMP_DIR    "${CMAKE_BINARY_DIR}/cpython/stamp"
        TMP_DIR      "${CMAKE_BINARY_DIR}/cpython/tmp"
        INSTALL_DIR  "${PYTHON_INSTALL_PREFIX}/cpython/target"

        GIT_REPOSITORY https://github.com/python/cpython.git
        GIT_TAG        "${CPYTHON_TAG}"
        GIT_SHALLOW    1
        UPDATE_DISCONNECTED 1

        # Apply your local patch after checkout/update
        PATCH_COMMAND
        ${CMAKE_COMMAND} -E echo "Patching CPython with cpython-${CPYTHON_TAG}.patch"
        COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR> patch -p1 -i "${CMAKE_CURRENT_SOURCE_DIR}/cpython-${CPYTHON_TAG}.patch"

        # Configure step with environment variables
        CONFIGURE_COMMAND
        ${CMAKE_COMMAND} -E env
        MODULE_MMAP_TRUE=#
        MODULE_BUILDTYPE=static
        CONFIG_SITE=${CMAKE_CURRENT_SOURCE_DIR}/config.site
        CC=<BINARY_DIR>/../../dandelion_sdk/x86_64-unknown-dandelion-clang
        CFLAGS=-g3\ -O0\ -femulated-tls
        HOST_CFLAGS=-Wno-unused-value\ -Wno-empty-body\ -Qunused-arguments
        LDFLAGS=-fuse-ld=lld
        LDSHARED=<BINARY_DIR>/../../dandelion_sdk/x86_64-unknown-dandelion-clang
        CCSHARED=
        <SOURCE_DIR>/configure
        --host=x86_64-unknown-linux
        --build=x86_64-linux-gnu
        --disable-ipv6
        --prefix=<INSTALL_DIR>
        --with-build-python=/usr/bin/python3
        --without-builtin-hashlib-hashes
        --disable-test-modules
        --with-openssl=/opt/libssl
        --with-pydebug

        # inside ExternalProject_Add(...) after CONFIGURE_COMMAND
        COMMAND ${CMAKE_COMMAND} -E echo "Modifying Modules/Setup.stdlib in build directory"
        COMMAND ${CMAKE_COMMAND} -E chdir <BINARY_DIR>
                sed -i -e "s/^mmap mmapmodule.c/#mmap mmapmodule.c/" Modules/Setup.stdlib
        
        COMMAND ${CMAKE_COMMAND} -E echo "Updating freeze_modules.py with frozen modules"
        COMMAND ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
        sed -i "s|#'<encodings.*>',|${FROZEN_MODULES_STRING}|g" Tools/build/freeze_modules.py

        BUILD_COMMAND
                /bin/sh -c "make -C <BINARY_DIR> -j${CMAKE_BUILD_PARALLEL_LEVEL} \
                && make -C <BINARY_DIR> regen-frozen \
                && make -C <BINARY_DIR> -j${CMAKE_BUILD_PARALLEL_LEVEL}"
        
        INSTALL_COMMAND $(MAKE) altinstall

        # Helpful logging/TTY
        LOG_CONFIGURE 1
        LOG_BUILD     1
        LOG_INSTALL   1
        USES_TERMINAL_CONFIGURE 1
        USES_TERMINAL_BUILD     1
        USES_TERMINAL_INSTALL   1
)

# Build CPython as part of the default 'all' target
add_custom_target(cpython-all ALL DEPENDS cpython)
