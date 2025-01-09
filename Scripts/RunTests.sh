# Navigate to root dir
cd $(dirname "$0")
cd ../

# Build all
./Configure.sh

# Copy csv folder
cp -r Resources/Csv Build/Tests

cd Build

BUILD_START=$SECONDS
make -j
echo -e "Times:  \n  Build time: <b>" $(($SECONDS - $BUILD_START)) "</b>  \n\n<pre>" > report.md

mv report.md Tests

cd Tests

# Launch tests
BUILD_START=$SECONDS
echo "Start scenarios"
./Scenarios

echo "Start Vault unit tests"
./VaultUnitTest

echo "Start VaultRecordSet unit tests"
./VaultRecordSetUnitTest

echo "Start VaultRecordRef unit tests"
./VaultRecordRefUnitTest

# echo "Start VaultRecordRef multi-threading tests"
./VaultRecordRefMpTest

# echo "Start VaultRecordSet multi-threading tests"
./VaultRecordSetMpTest

echo "</pre>" >> report.md
sed -i '2i\  Tests time: <b> '"$(($SECONDS - $BUILD_START))"' </b>  \' report.md
