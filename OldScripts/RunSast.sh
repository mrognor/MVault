# Navigate to root dir
cd $(dirname "$0")
cd ..

# Navigate to sast dir
mkdir -p Sast
cd Sast

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../Source/

CodeChecker analyze ./compile_commands.json --analyzers clangsa --enable=sensitive --output clangsa
CodeChecker parse --export html --output ./html_clangsa ./clangsa

CodeChecker analyze ./compile_commands.json --analyzers clang-tidy \
    --enable=sensitive \
    --disable=clang-diagnostic-unused-parameter \
    --disable=clang-diagnostic-unused-variable \
    --disable=bugprone-unhandled-exception-at-new \
    --disable=misc-header-include-cycle \
    --disable=cppcoreguidelines-pro-type-const-cast \
    --output clang-tidy
CodeChecker parse --export html --output ./html_clang_tidy ./clang-tidy

echo "Finish sast"