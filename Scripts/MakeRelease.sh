# Navigate to root dir
cd $(dirname "$0")
cd ..

# Create Release dir
mkdir -p Release

# Build lib
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ../Source/
make MVault -j

# Create include folder
mkdir -p include
cp -rf ../Source/*.h* include/
mkdir -p include/Functions
cp -rf ../Source/Functions/*.h* include/Functions/

# Create bin folder
mkdir -p bin
cp libMVault.a bin/

# Create release folder
mkdir -p MVault

# Make release
if [ -d MVault/include ]; then
    rm -rf MVault/include
fi

if [ -d MVault/bin ]; then
    rm -rf MVault/bin
fi

mv -f include MVault
mv -f bin MVault