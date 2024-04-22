# # Configure
cd $(dirname "$0")
mkdir -p Build
cd Build
cmake ..

# Copy csv table
cd ..
cp Resources/Csv/CPU_benchmark_v4.csv Build/