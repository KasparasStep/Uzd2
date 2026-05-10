cmake_minimum_required(VERSION 3.14)
project(Uzd2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---- Optimizavimo lygis ----
# Pakeiskite šią eilutę norėdami keisti lygį: -O1, -O2, arba -O3
# MSVC (Visual Studio) naudoja /O1, /O2, /Ox — keičiama per Project Properties → C/C++ → Optimization
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O1")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O1")
endif()

# Visi vykdomieji failai rašomi tiesiai į šaltinio katalogą,
# kad Data/ aplankas būtų randamas pagal santykinį kelią.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# MSVC: debug/release subkatalogai ignoruojami
foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} ${CMAKE_SOURCE_DIR})
endforeach()

# Bendras šaltinio failas (pagalbinės funkcijos)
set(SHARED_SRC funkcijos.cpp)

# ---- Pagrindinis projektas ----
add_executable(Uzd2
    Uzd1.cpp
    vektorius.cpp
    testavimas.cpp
    ${SHARED_SRC}
)

# ---- Tyrimo projektai ----
add_executable(stud_Vector
    Vector/Vector.cpp
    ${SHARED_SRC}
)

add_executable(stud_List
    List/List.cpp
    ${SHARED_SRC}
)

add_executable(stud_Deque
    Deque/Deque.cpp
    ${SHARED_SRC}
)

target_include_directories(stud_Vector PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(stud_List PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(stud_Deque PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(Uzd2 PRIVATE ${CMAKE_SOURCE_DIR})
# _WIN32 apibrėžiamas automatiškai Windows kompiliatoriaus —
# papildoma target_compile_definitions eilutė nereikalinga.

# g++ 8 reikalauja -lstdc++fs; g++ 9+ įtraukia automatiškai.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    foreach(tgt Uzd2 stud_Vector stud_List stud_Deque)
        target_link_libraries(${tgt} stdc++fs)
    endforeach()
endif()