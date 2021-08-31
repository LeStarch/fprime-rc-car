####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
#
####
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Ports/")
set(SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/Pca9685ComponentAi.xml"
                 "${CMAKE_CURRENT_LIST_DIR}/Pca9685.cpp")
register_fprime_module()
