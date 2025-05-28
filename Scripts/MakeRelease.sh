# Navigate to root dir
cd $(dirname "$0")
cd ..

# Clear previous
rm -rf MVault

# Create release build dir
mkdir MVault
cd MVault

# Build release binaries
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make MVault -j $1
cd ..

# Build debug binaries
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make MVault -j $1
cd ..

# Move headers
cp -r ../Source .
mv Source include
find include -type f -not -iname "*.h*" -delete

# Create release release dir
mkdir MVault
mkdir MVault/lib
cp Release/Source/libMVault.a MVault/lib
cp -r include MVault/

# Create debug release dir
mkdir MVaultD
mkdir MVaultD/lib
cp Debug/Source/libMVault.a MVaultD/lib
cp -r include MVaultD/

# Create tar archives
if command -v tar 2>&1 >/dev/null
then
    tar -czf MVault.tar.gz MVault
    tar -czf MVaultD.tar.gz MVaultD
fi