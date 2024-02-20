:: Add main page doxygen command to ReadMe.md
cd /d %~dp0

:: Create directory with examples
cp -rf ../Examples/ ./
powershell -Command "(gc Examples/SortExample.cpp) -replace '// startcode', '/** @code' | Out-File -encoding ASCII Examples/SortExample.cpp"
powershell -Command "(gc Examples/SortExample.cpp) -replace '// endcode', ' @endcode */' | Out-File -encoding ASCII Examples/SortExample.cpp"
powershell -Command "(gc Examples/CustomTypeExample.cpp) -replace '// startcode', '/** @code' | Out-File -encoding ASCII Examples/CustomTypeExample.cpp"
powershell -Command "(gc Examples/CustomTypeExample.cpp) -replace '// endcode', ' @endcode */' | Out-File -encoding ASCII Examples/CustomTypeExample.cpp"

:: Create documentation
doxygen Doxyfile

:: Copy svg scheme to html docs
if not exist .\Generated\html\Docs\ mkdir .\Generated\html\Docs
cp .\Scheme.svg .\Generated\html\Docs