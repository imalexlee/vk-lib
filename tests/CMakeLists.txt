# current link is for Google Test 1.16.0
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/4902ea2d7c6faed89b6facee00baa34bb108fc0d.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

link_libraries(vk-lib GTest::gtest_main)

add_executable(core_tests core_tests.cpp)

include(GoogleTest)
gtest_discover_tests(core_tests)
