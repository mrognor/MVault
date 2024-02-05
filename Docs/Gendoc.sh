# Add main page doxygen command to ReadMe.md
cd $(dirname "$0")

# Create documentation
doxygen Doxyfile

# Copy svg scheme to html docs
mkdir -p ./Generated/html/Docs
cp ./Scheme.svg ./Generated/html/Docs