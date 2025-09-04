# CMakeList.txt: проект CMake для Mod28_HW; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Mod28_HW "Mod28_HW.cpp" "Mod28_HW.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Mod28_HW PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
