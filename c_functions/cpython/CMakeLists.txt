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
set(CPYTHON_TAG "3.13" CACHE STRING "CPython git tag/branch/commit")

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
        ${CMAKE_COMMAND} -E echo "Patching CPython with cpython-3.13.patch"
        COMMAND
        ${CMAKE_COMMAND} -E chdir <SOURCE_DIR> patch -p1 -i "${CMAKE_CURRENT_SOURCE_DIR}/cpython-3.13.patch"

        # Configure step with environment variables
        CONFIGURE_COMMAND
        ${CMAKE_COMMAND} -E env
        MODULE_MMAP_TRUE=\#
        MODULE_BUILDTYPE=static
        CONFIG_SITE=${CMAKE_CURRENT_SOURCE_DIR}/config.site
        CC=/workspaces/semester-project/dandelionFunctionExamples/c_functions/build/dandelion_sdk/x86_64-unknown-dandelion-clang
        CFLAGS=-g3\ -O0\ -femulated-tls
        HOST_CFLAGS=-Wno-unused-value\ -Wno-empty-body\ -Qunused-arguments
        LDFLAGS=-fuse-ld=lld
        LDSHARED=/workspaces/semester-project/dandelionFunctionExamples/c_functions/build/dandelion_sdk/x86_64-unknown-dandelion-clang
        CCSHARED=
        <SOURCE_DIR>/configure
        --host=x86_64-unknown-linux
        --build=x86_64-linux-gnu
        --disable-ipv6
        --prefix=<INSTALL_DIR>
        --with-build-python=/home/vscode/.pyenv/shims/python
        --without-builtin-hashlib-hashes
        --disable-test-modules
        --with-openssl=/opt/libssl
        --with-pydebug

        # Modify Modules/Setup.stdlib in the build directory
        COMMAND
        ${CMAKE_COMMAND} -E echo "Modifying Modules/Setup.stdlib in build directory"
        COMMAND
        ${CMAKE_COMMAND} -E chdir <BINARY_DIR> sed -i 's/^mmap mmapmodule.c/#mmap mmapmodule.c/' <BINARY_DIR>/Modules/Setup.stdlib


        BUILD_COMMAND   $(MAKE) -j${CMAKE_BUILD_PARALLEL_LEVEL}
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