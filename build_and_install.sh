mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cp Earley ../Earley
cp EarleyTest ../EarleyTest
cd ..
rm -rf build
