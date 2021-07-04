cmake_minimum_required(VERSION 3.10)
project(mandelbrot VERSION 1.0.0)
find_package(OpenMP REQUIRED)
if(APPLE)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)

    if(CMAKE_C_COMPILER_ID MATCHES "Clang\$")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp")
        set(OpenMP_C_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY omp)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang\$")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp")
        set(OpenMP_CXX_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY omp)
    endif()

endif()



include_directories(${PROJECT_SOURCE_DIR})
set(CMAKE_BUILD_TYPE Debug)

configure_file(defaultcolormap.txt defaultcolormap.txt COPYONLY)

add_executable(mandelbrot main.cpp mandelbrot.cpp colorMapLoader.cpp)
target_link_libraries(mandelbrot PRIVATE OpenMP::OpenMP_CXX)