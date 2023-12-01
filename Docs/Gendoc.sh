# Add main page doxygen command to ReadMe.md
cd $(dirname "$0")/.. 
mv ReadMe.md TmpReadMe.md
echo '\mainpage' > ReadMe.md
cat TmpReadMe.md >> ReadMe.md
rm TmpReadMe.md

# Create documentation
cd Docs
doxygen Doxyfile

# Remove main page doxygen command from ReadMe.md 
cd ..
sed -i '1d' ReadMe.md