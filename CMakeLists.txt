cmake_minimum_required(VERSION 3.28)
project(PasswordChecker C)

set(CMAKE_C_STANDARD 99)

add_executable(PasswordChecker src/main.c src/platform.c)
