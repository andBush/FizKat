cmake_minimum_required(VERSION 3.16)
project(FizKat LANGUAGES CXX)

add_executable(FizKat
    main.cpp
    Commands.cpp
    Fleet.cpp
    Inventory.cpp
    Transport.cpp
)

target_compile_features(FizKat PRIVATE cxx_std_20)
set_target_properties(FizKat PROPERTIES CXX_EXTENSIONS OFF)
