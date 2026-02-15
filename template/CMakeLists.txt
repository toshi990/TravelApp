add_library(TemplateLib INTERFACE)

target_include_directories(TemplateLib INTERFACE .)

add_executable(ht_tests ht_tests.cpp)
target_link_libraries(ht_tests PRIVATE TemplateLib)
target_compile_options(ht_tests PRIVATE -fsanitize=address -g)
target_link_options(ht_tests PRIVATE -fsanitize=address)