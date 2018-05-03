
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
endif()

set(CMAKE_CXX_EXTENSIONS Off)

add_executable(test_span11
    catch_main.cpp
    test_span.cpp
)
target_link_libraries(test_span11 PUBLIC span)
target_compile_features(test_span11 PUBLIC cxx_std_11)

add_executable(test_span14
    catch_main.cpp
    test_span.cpp
)
target_link_libraries(test_span14 PUBLIC span)
target_compile_features(test_span14 PUBLIC cxx_std_14)

add_executable(test_span17
    catch_main.cpp
    test_span.cpp
    test_deduction_guides.cpp
    test_structured_bindings.cpp
)
target_link_libraries(test_span17 PUBLIC span)
target_compile_features(test_span17 PUBLIC cxx_std_17)

add_executable(test_contract_checking
    catch_main.cpp
    test_contract_checking.cpp
)
target_link_libraries(test_contract_checking PUBLIC span)