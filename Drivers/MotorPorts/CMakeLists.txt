####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
#
####
set(SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/DirectionEnumAi.xml"
                 "${CMAKE_CURRENT_LIST_DIR}/SpinPortAi.xml")
register_fprime_module()
