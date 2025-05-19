# This script runs unit tests and gets code coverage with them.
# Running the script without parameters will run all unit tests and create reports for them.
# You can pass a test to the script to run and verify coverage.
# To get full coverage, but without running the covers of individual classes, you can pass the "Full" parameter.


# Function to run tests and create coverage report
function create_report() {
    target=*
    target_dir=Full

    if [ $# -ne 0 ] && [ "$1" != "Full" ];
    then
        target=$1
        target_dir=$1
    fi

    # Remove previous test coverage files
    find Build -name '*.gcda' -delete

    # Run unit tests
    ./Build/Tests/UnitTests $1

    # Copy coverage data
    mkdir -p Coverage
    mkdir -p Coverage/$target_dir
    cp -r Build/Tests/CMakeFiles/UnitTests.dir/UnitTests/${target}UnitTests.cpp.gcno Coverage/$target_dir
    cp -r Build/Tests/CMakeFiles/UnitTests.dir/UnitTests/${target}UnitTests.cpp.gcda Coverage/$target_dir
    cp -r Build/Source/CMakeFiles/MVault.dir/$target.cpp.gcno Coverage/$target_dir
    cp -r Build/Source/CMakeFiles/MVault.dir/$target.cpp.gcda Coverage/$target_dir

    # Create coverage html
    lcov --exclude '*/c++/*' -t "Coverage" -o Coverage.info -c -d Coverage/$target_dir --demangle-cpp
    genhtml -o Coverage/$target_dir Coverage.info
    rm Coverage.info
}

# Navigate to root dir
cd $(dirname "$0")
cd ../..

# Check if Build dir exist
if [ ! -d "Build" ]; then
  ./Configure.sh
fi

# Build project
cd Build
make -j
cd ..

create_report
# # Get script arguments count
# if [ $# -eq 0 ]
# then # If 0 arguments passed then launch all
#     # Create report for all separate parts
#     array=("Vault" "VaultRecordSet")
#     for element in "${array[@]}"
#     do
#         create_report $element
#     done

#     # Create full report
#     create_report
# else # Create required report
#     if  [ "$1" == "Full" ]
#     then
#         create_report
#     else
#         create_report $1
#     fi
# fi