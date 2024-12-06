# # Configure
cd $(dirname "$0")
mkdir -p Build
cd Build
cmake ..

# Copy csv folder
cd ..
cp -r Resources/Csv Build/