cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VERSION})

add_executable(graphics_editor main.cpp Document.h Primitive.h Redactor_MVC.h)

set_target_properties(graphics_editor PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
 
target_include_directories(graphics_editor
    PRIVATE "${CMAKE_BINARY_DIR}"
)

install(TARGETS graphics_editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT alexsh93@live.ru)
include(CPack)
