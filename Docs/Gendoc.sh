# Add main page doxygen command to ReadMe.md
cd $(dirname "$0")

# Create directory with examples
cp -rf ../Examples/ ./
sed -i 's!// startcode!/** @code!' Examples/SortExample.cpp
sed -i 's!// endcode!@endcode */!' Examples/SortExample.cpp

# Create documentation
doxygen Doxyfile

# Copy svg scheme to html docs
mkdir -p ./Generated/html/Docs
cp ./Scheme.svg ./Generated/html/Docs