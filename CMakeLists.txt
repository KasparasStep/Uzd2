cmake_minimum_required(VERSION 3.14)
project(Uzd1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Visi vykdomieji failai rašomi tiesiai į šaltinio katalogą,
# kad Data/ aplankas būtų randamas pagal santykinį kelią.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
 
# MSVC: debug/release subkatalogai ignoruojami
foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} ${CMAKE_SOURCE_DIR})
endforeach()

# Bendras šaltinio failas (matematika + pagalbinės funkcijos)
set(SHARED_SRC funkcijos.cpp)

# ---- Pagrindinis projektas ----
add_executable(Uzd1
    Uzd1.cpp
    vektorius.cpp
    testavimas.cpp
    ${SHARED_SRC}
)



# g++ 8 reikalauja -lstdc++fs; g++ 9+ įtraukia automatiškai.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    foreach(target Uzd1 stud_Vector stud_List stud_Deque)
        target_link_libraries(${target} stdc++fs)
    endforeach()
endif()