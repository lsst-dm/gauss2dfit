export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$CONDA_PREFIX/.local/lib64/pkgconfig
prefix="--prefix=$CONDA_PREFIX/.local"
meson $prefix --buildtype release build-release
./build-cc-release.sh
cd python
meson $prefix --buildtype release build-release
cd ..
./build-py-release.sh
