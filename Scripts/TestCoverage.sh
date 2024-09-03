# Run tests to cretae coverage data
cd $(dirname "$0")
# ./RunTests.sh

# Navigate to root dir
cd ..

# Copy Vault class coverage data
mkdir -p Coverage
mkdir -p Coverage/Vault
cp -r Build/Tests/CMakeFiles/VaultUnitTest.dir/VaultUnitTest.cpp.gcno Coverage/Vault
cp -r Build/Tests/CMakeFiles/VaultUnitTest.dir/VaultUnitTest.cpp.gcda Coverage/Vault
cp -r Build/Source/CMakeFiles/MVault.dir/*.gcno Coverage/Vault
cp -r Build/Source/CMakeFiles/MVault.dir/*.gcda Coverage/Vault
cp -r Build/Source/CMakeFiles/MVault.dir/Functions/*.gcno Coverage/Vault
cp -r Build/Source/CMakeFiles/MVault.dir/Functions/*.gcda Coverage/Vault

# Create coverage html
lcov -t "VaultCoverage" -o VaultCoverage.info -c -d Coverage/Vault --demangle-cpp
genhtml -o Coverage/Vault VaultCoverage.info
rm VaultCoverage.info

# Copy VaultRecordRef class coverage data
mkdir -p Coverage/VaultRecordRef
cp -f Build/Tests/CMakeFiles/VaultRecordRefUnitTest.dir/VaultRecordRefUnitTest.cpp.gcno Coverage/VaultRecordRef
cp -f Build/Tests/CMakeFiles/VaultRecordRefUnitTest.dir/VaultRecordRefUnitTest.cpp.gcda Coverage/VaultRecordRef
cp -f Build/Source/CMakeFiles/MVault.dir/*.gcno Coverage/VaultRecordRef
cp -f Build/Source/CMakeFiles/MVault.dir/*.gcda Coverage/VaultRecordRef
cp -f Build/Source/CMakeFiles/MVault.dir/Functions/*.gcno Coverage/VaultRecordRef
cp -f Build/Source/CMakeFiles/MVault.dir/Functions/*.gcda Coverage/VaultRecordRef

# Create coverage html
lcov -t "VaultRecordRefCoverage" -o VaultRecordRefCoverage.info -c -d Coverage/VaultRecordRef --demangle-cpp
genhtml -o Coverage/VaultRecordRef VaultRecordRefCoverage.info
rm VaultRecordRefCoverage.info

# Copy VaultRecordSet class coverage data
mkdir -p Coverage/VaultRecordSet
cp -f Build/Tests/CMakeFiles/VaultRecordSetUnitTest.dir/VaultRecordSetUnitTest.cpp.gcno Coverage/VaultRecordSet
cp -f Build/Tests/CMakeFiles/VaultRecordSetUnitTest.dir/VaultRecordSetUnitTest.cpp.gcda Coverage/VaultRecordSet
cp -f Build/Source/CMakeFiles/MVault.dir/*.gcno Coverage/VaultRecordSet
cp -f Build/Source/CMakeFiles/MVault.dir/*.gcda Coverage/VaultRecordSet
cp -f Build/Source/CMakeFiles/MVault.dir/Functions/*.gcno Coverage/VaultRecordSet
cp -f Build/Source/CMakeFiles/MVault.dir/Functions/*.gcda Coverage/VaultRecordSet

# Create coverage html
lcov -t "VaultRecordSetCoverage" -o VaultRecordSetCoverage.info -c -d Coverage/VaultRecordSet --demangle-cpp
genhtml -o Coverage/VaultRecordSet VaultRecordSetCoverage.info
rm VaultRecordSetCoverage.info

# Copy manin html coverage file
cp -f Resources/Html/CodeCoverage.html Coverage