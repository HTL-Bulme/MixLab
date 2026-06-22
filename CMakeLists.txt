cmake_minimum_required(VERSION 3.18)
project(mixlab VERSION 1.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CTest)

# --- wxWidgets ---
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

set(APP_SRCS
  atom_app.cpp
  atom_canvas.cpp
  atom_frame.cpp
  atom_mixer.cpp
  reaction_adapter.cpp
  reaction_controller.cpp
  molecule_data.cpp
  reaction.cpp
  language.cpp
  gui_sidebar.cpp
  gui_menu.cpp
  gui_dialogs.cpp
  reaction_history.cpp
)

add_executable(${PROJECT_NAME} ${APP_SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${wxWidgets_DEFINITIONS})

add_executable(test_logic
    tests/test_logic.cpp
    reaction.cpp
    reaction_history.cpp
)

add_executable(test_atom_mixer
    tests/test_atom_mixer.cpp
    reaction.cpp
)

target_include_directories(test_atom_mixer PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${wxWidgets_INCLUDE_DIRS}
)
target_include_directories(test_logic PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(test_atom_mixer PRIVATE ${wxWidgets_LIBRARIES})
target_compile_definitions(test_atom_mixer PRIVATE ${wxWidgets_DEFINITIONS})

add_test(NAME test_atom_mixer COMMAND test_atom_mixer)
add_test(NAME test_logic COMMAND test_logic)