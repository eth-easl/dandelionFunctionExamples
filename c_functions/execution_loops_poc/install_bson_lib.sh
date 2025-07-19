# Script for building the bson library, the c_functions directory is mounted inside the DandelionSDK container.

it clone https://github.com/mongodb/libbson.git
cd libbson
mkdir libson_build
cd libson_build/
cmake ..   -DENABLE_STATIC=ON   -DCMAKE_INSTALL_PREFIX=/workspace/dandelion_bson_install   -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc) bson_static

# Copy static library
cp libbson-static-1.0.a /workspace/dandelion_bson_install/lib/

# Copy headers
cp -r ../src/bson /workspace/dandelion_bson_install/include/libbson-1.0
cp src/bson/*.h /workspace/dandelion_bson_install/include/libbson-1.0/bson/

