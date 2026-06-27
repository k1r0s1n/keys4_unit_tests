Для проверки на ubuntu linux
Установка пакетов - sudo apt insall -y g++ pkgconf clang libgtest-dev cmake build-essential
Сборка для тестирования - g++ -std=c++17 main.cpp $(pkg-config --cflags --libs gtest gtest_main) -pthread -o run_tests
Запуск теста ./run_tests
