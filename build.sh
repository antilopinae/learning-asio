#!/bin/bash
sudo rm -R build
echo -e "\nGenerating"
cmake -DLLVM_ENABLE_PROJECTS=clang -DCMAKE_BUILD_TYPE=Debug -D CMAKE_C_COMPILER="/usr/bin/clang" -D CMAKE_CXX_COMPILER="/usr/bin/clang++" -G Ninja -B build -S .  # note, the ninja build tool is faster than the make tool referenced in the docs.

echo -e "\nBuilding"
cmake --build build

# echo -e "\nTesting"
# cd build
# ctest
# cd ..

# echo -e "\nGraphviz"
# cd build
# mkdir graph
# cmake --graphviz=graph/test.dot .
# dot -Tpdf graph/test.dot >> deps.pdf
# cd ..

echo -e "\nRunning"
./build/bin/bootstrap
