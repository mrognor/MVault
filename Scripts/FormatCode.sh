# Navigate to source dir
cd $(dirname "$0")
cd ../Source

find . -regex '.*\.\(cpp\|hpp\|cu\|cuh\|c\|h\)' -exec clang-format -style=file -i {} \;