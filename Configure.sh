# # Configure
cd $(dirname "$0")
mkdir -p Build
cd Build
cmake .. -D CMAKE_BUILD_TYPE=$1

# Copy csv folder
cd ..
cp -r Resources/Csv Build/