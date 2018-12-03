
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
endif()

set(CMAKE_CXX_EXTENSIONS Off)

add_library(test_main catch_main.cpp)

add_executable(test_span11
    test_span.cpp
)
add_test(span11 test_span11)
target_link_libraries(test_span11 PUBLIC test_main span)
target_compile_features(test_span11 PUBLIC cxx_std_11)

add_executable(test_span14
    test_span.cpp
)
add_test(span14 test_span14)
target_link_libraries(test_span14 PUBLIC test_main span)
target_compile_features(test_span14 PUBLIC cxx_std_14)

add_executable(test_span17
    test_span.cpp
    test_deduction_guides.cpp
    test_structured_bindings.cpp
)
add_test(span17 test_span17)
target_link_libraries(test_span17 PUBLIC test_main span)
target_compile_features(test_span17 PUBLIC cxx_std_17)

add_executable(test_contract_checking
    test_contract_checking.cpp
)
add_test(contractchecking test_contract_checking)
target_link_libraries(test_contract_checking PUBLIC test_main span)
