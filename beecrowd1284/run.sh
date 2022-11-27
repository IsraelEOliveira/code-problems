echo [Compile]
time g++ ./src/main.cpp -o ./build/main

echo
echo [Run]
time cat ./sample/input.txt | ./build/main