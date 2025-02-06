# # Configure
cd $(dirname "$0")
mkdir -p Build
cd Build
cmake .. -D CMAKE_EXPORT_COMPILE_COMMANDS=1 -D CMAKE_BUILD_TYPE=$1
mv compile_commands.json ..
echo "Reopen ide to get highlight in examples"

# Copy csv folder
cd ..
cp -r Resources/Csv Build/