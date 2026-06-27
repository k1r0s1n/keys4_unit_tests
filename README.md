Для проверки на ubuntu linux
sudo apt insall -y g++ pkgconf clang libgtest-dev cmake build-essential
g++ -std=c++17 main.cpp $(pkg-config --cflags --libs gtest gtest_main) -pthread -o run_tests
./run_tests
