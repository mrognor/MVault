cd $(dirname "$0")

# Create examples
ExampleFiles=(SortExample.cpp CustomTypeExample.cpp)

for ExampleFile in "${ExampleFiles[@]}";
do
    mkdir -p Examples
    cp -f ../Examples/$ExampleFile ./Examples
    sed -i 's!// startcode!/** @code!' Examples/$ExampleFile
    sed -i 's!// endcode! @endcode */!' Examples/$ExampleFile
    sed -i 's!// ignorecode!/*!' Examples/$ExampleFile
    sed -i 's!// endignore!*/!' Examples/$ExampleFile
    sed -i 's!// insertcode!!' Examples/$ExampleFile
done

# Create documentation
doxygen Doxyfile

# Copy svg scheme to html docs
mkdir -p ./Generated/html/Docs
cp ./Scheme.svg ./Generated/html/Docs