# Navigate to root dir
cd $(dirname "$0")
cd ..

# Clear previous
rm -rf Release
rm -rf MVault

# Create release build dir
mkdir -p Release

# Build lib
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ../Source/
make MVault -j
cd ..

# Create release dir
mkdir -p MVault

# Move headers
cp -r Source MVault/include
find ./MVault/include -type f -not -iname "*.h*" -delete

# Move lib file
mkdir ./MVault/lib
cp ./Release/libMVault.a ./MVault/lib
