:: Add main page doxygen command to ReadMe.md
cd /d %~dp0

:: Create documentation
doxygen Doxyfile

:: Copy svg scheme to html docs
if not exist .\Generated\html\Docs\ mkdir .\Generated\html\Docs
cp .\Scheme.svg .\Generated\html\Docs