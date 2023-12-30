# Add main page doxygen command to ReadMe.md
cd $(dirname "$0")
echo '\mainpage' > ../Source/ReadMe.md
cat ../ReadMe.md >> ../Source/ReadMe.md

# Create documentation
doxygen Doxyfile

# Remove tmp file
rm ../Source/ReadMe.md

# Copy svg scheme to html docs
mkdir -p ./Generated/html/Docs
cp ./Scheme.svg ./Generated/html/Docs