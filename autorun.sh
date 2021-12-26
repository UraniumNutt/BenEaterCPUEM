# run the code
echo "Compiling..."

cd bin
./customasm input.s -o output.bin
g++ *.cpp -o BECPU 

echo "Running..."
./BECPU

echo "Cleaning..."
