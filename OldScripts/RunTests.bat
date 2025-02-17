:: Navigate to root dir
@echo off
cd /d %~dp0
cd ..

:: Build all
Call Configure.bat
cd Build
make -j

cd Tests

:: Launch tests
echo Start scenarios
Scenarios.exe

echo Start Vault unit tests
VaultUnitTest.exe

echo Start VaultRecordSet unit tests
VaultRecordSetUnitTest.exe

echo Start VaultRecordRef unit tests
VaultRecordRefUnitTest.exe

echo Start VaultRecordRef multi-threading tests
VaultRecordRefMpTest.exe