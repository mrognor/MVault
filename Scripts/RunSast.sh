# Navigate to root dir
cd $(dirname "$0")
cd ..

# Navigate to sast dir
mkdir -p Sast
cd Sast

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../Source/

CodeChecker analyze ./compile_commands.json --analyzers clangsa --enable=sensitive --output clangsa
CodeChecker parse --export html --output ./html_clangsa ./clangsa

CodeChecker analyze ./compile_commands.json --analyzers cppcheck --enable=sensitive --output cppcheck
CodeChecker parse --export html --output ./html_cppcheck ./cppcheck

CodeChecker analyze ./compile_commands.json --analyzers clang-tidy --enable=sensitive --output clang-tidy
CodeChecker parse --export html --output ./html_clang_tidy ./clang-tidy

echo "Finish sast"