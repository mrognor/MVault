# Add main page doxygen command to ReadMe.md
cd ..
mv ReadMe.md TmpReadMe.md
echo '\mainpage' > ReadMe.md
cat TmpReadMe.md >> ReadMe.md
rm TmpReadMe.md

# Create documentation
mv Docs/Doxyfile Doxyfile
doxygen Doxyfile
mv Doxyfile Docs/Doxyfile

# Remove main page doxygen command from ReadMe.md
sed -i '1d' ReadMe.md