cmake_minimum_required(VERSION 3.14)
project(Uzd2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---- Optimizavimo lygis ----
# Pakeiskite noredami keisti: -O1, -O2, -O3
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

# Vykdomieji failai rasomi i saltinio kataloga (kad Data/ butu randamas)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} ${CMAKE_SOURCE_DIR})
endforeach()

set(SHARED_SRC funkcijos.cpp)

# ---- Pagrindinis projektas ----
add_executable(Uzd2
    Uzd2.cpp
    vektorius.cpp
    testavimas.cpp
    ${SHARED_SRC}
)

# ---- Konteineriu tyrimas ----
add_executable(stud_Vector stud_Vector.cpp ${SHARED_SRC})
add_executable(stud_List   stud_List.cpp   ${SHARED_SRC})
add_executable(stud_Deque  stud_Deque.cpp  ${SHARED_SRC})

# ---- Paprastas metodų testas (v1.2/v1.5) ----
add_executable(metodu_test metodu_test.cpp ${SHARED_SRC})

# ---- Catch2 unit testai (v2.0) ----
if(EXISTS "${CMAKE_SOURCE_DIR}/catch.hpp")
    add_executable(unit_testai unit_testai.cpp ${SHARED_SRC})
    message(STATUS "catch.hpp rastas - unit_testai bus sukompiliuoti")
else()
    message(STATUS "catch.hpp nerastas - unit_testai praleisti")
    message(STATUS "  Atsisiuskite is https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp")
endif()

# ---- Doxygen dokumentacija ----
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generuojama Doxygen dokumentacija"
        VERBATIM)
endif()

# g++ 8 reikalauja -lstdc++fs
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    foreach(tgt Uzd2 stud_Vector stud_List stud_Deque metodu_test)
        target_link_libraries(${tgt} stdc++fs)
    endforeach()
    if(TARGET unit_testai)
        target_link_libraries(unit_testai stdc++fs)
    endif()
endif()