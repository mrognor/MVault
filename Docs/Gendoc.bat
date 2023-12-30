:: Add main page doxygen command to ReadMe.md
cd /d %~dp0
echo \mainpage > ..\Source\ReadMe.md
cat ..\ReadMe.md >> ..\Source\ReadMe.md

:: Create documentation
doxygen Doxyfile

:: Remove tmp file
rm ..\Source\ReadMe.md

:: Copy svg scheme to html docs
if not exist .\Generated\html\Docs\ mkdir .\Generated\html\Docs
cp .\Scheme.svg .\Generated\html\Docs