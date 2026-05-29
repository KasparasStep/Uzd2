cmake_minimum_required(VERSION 3.14)
project(Uzd2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---- Optimizavimo lygis ----
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

# ---- Pagrindinis projektas (v3.0: naudoja MyVector vietoj std::vector) ----
add_executable(Uzd2
    Uzd2.cpp
    vektorius.cpp
    failu_greicio_test.cpp
    ${SHARED_SRC}
)

# ---- v3.0: std::vector vs MyVector spartos palyginimas ----
add_executable(palyginimas
    palyginimas.cpp
    ${SHARED_SRC}
)

# ---- Konteineriu tyrimas (std::vector vs std::list vs std::deque) ----
add_executable(stud_Vector  Vector/Vector.cpp  ${SHARED_SRC})
add_executable(stud_List    List/List.cpp      ${SHARED_SRC})
add_executable(stud_Deque   Deque/Deque.cpp    ${SHARED_SRC})

# Subkatalogai turi rasti struktura.h ir MyVector.h pagrindiniame kataloge
target_include_directories(stud_Vector PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(stud_List   PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(stud_Deque  PRIVATE ${CMAKE_SOURCE_DIR})

# ---- Metodu testas (v1.2/v1.5) ----
add_executable(metodu_test  metodu_test.cpp  ${SHARED_SRC})

# ---- Catch2 unit testai (v2.0 + v3.0) ----
# v2.0: Studentas/Zmogus testai (unit_test.cpp turi CATCH_CONFIG_MAIN)
# v3.0: MyVector testai (unit_test_myvector.cpp tik #include "catch.hpp")
# Abu kompiliuojami i viena vykdomaji - paleidus jis paleidzia visus testus.
set(UNIT_TEST_FILES "")
if(EXISTS "${CMAKE_SOURCE_DIR}/unit_test/unit_test.cpp")
    list(APPEND UNIT_TEST_FILES unit_test/unit_test.cpp)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/unit_test/unit_test_myvector.cpp")
    list(APPEND UNIT_TEST_FILES unit_test/unit_test_myvector.cpp)
endif()

if(UNIT_TEST_FILES)
    add_executable(unit_testai  ${UNIT_TEST_FILES}  ${SHARED_SRC})
    target_include_directories(unit_testai PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/unit_test)
    message(STATUS "unit_testai bus sukompiliuoti is: ${UNIT_TEST_FILES}")
else()
    message(STATUS "Jokiu unit_test failu nerasta - unit_testai praleisti")
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

# g++ 8 reikalauja -lstdc++fs; g++ 9+ itraukia automatiskai
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    foreach(tgt Uzd2 palyginimas stud_Vector stud_List stud_Deque metodu_test)
        target_link_libraries(${tgt} stdc++fs)
    endforeach()
    if(TARGET unit_testai)
        target_link_libraries(unit_testai stdc++fs)
    endif()
endif()