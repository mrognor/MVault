# Navigate to root dir
cd $(dirname "$0")
cd ../..

# Check if Build dir exist
if [ ! -d "Build" ]; then
  ./Configure.sh
fi

# Build project
cd Build
make -j

# Run unit tests
./Tests/UnitTests $1