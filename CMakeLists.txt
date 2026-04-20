cmake_minimum_required(VERSION 3.15)
project(TravelApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        .
        controller
        exc
        interfaces
        model
        repo
        src
        template
        view
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(controller)
add_subdirectory(template)
add_subdirectory(view)

add_executable(app
        main.cpp
        exc/excursion.cpp
        interfaces/transfer.h
        file/fileSystem.h
        interfaces/IFile.h
        interfaces/Iterator.h
        benchmark.cpp
)

target_link_libraries(app
        ControllerLib
        TemplateLib
        ViewLib
)

add_executable(tests
        tests.cpp
        exc/excursion.cpp
)

target_include_directories(tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/controller
        ${CMAKE_CURRENT_SOURCE_DIR}/exc
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/model
        ${CMAKE_CURRENT_SOURCE_DIR}/repo
        ${CMAKE_CURRENT_SOURCE_DIR}/template
        ${CMAKE_CURRENT_SOURCE_DIR}/view
)

target_link_libraries(tests PRIVATE
        ControllerLib
        TemplateLib
        ViewLib
)

add_custom_target(run_tests
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests --success
        DEPENDS tests
)