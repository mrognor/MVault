# Navigate to root dir
cd $(dirname "$0")
cd ../

# Build all
./Configure.sh
cd Build
make -j

cd Tests

#Launch tests
echo "Start scenarios"
./Scenarios

echo "Start Vault unit tests"
./VaultUnitTest

echo "Start VaultRecordSet unit tests"
./VaultRecordSetUnitTest

echo "Start VaultRecordRef unit tests"
./VaultRecordRefUnitTest