:: Add main page doxygen command to ReadMe.md
cd /d %~dp0..
mv ReadMe.md TmpReadMe.md
echo '\mainpage' > ReadMe.md
cat TmpReadMe.md >> ReadMe.md
rm TmpReadMe.md

:: Create documentation
cd /d %~dp0
doxygen Doxyfile

:: Remove main page doxygen command from ReadMe.md
cd /d %~dp0..
more +1 "ReadMe.md" > "tmp"
move /Y "tmp" "ReadMe.md"