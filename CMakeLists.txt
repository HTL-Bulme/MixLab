cmake_minimum_required(VERSION 3.18)
project(mixlab VERSION 1.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion")
set(CMAKE_CXX_STANDARD 20)
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
  molecule_data.cpp
)

add_executable(${PROJECT_NAME} ${APP_SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${wxWidgets_DEFINITIONS})
# --- Tests (optional, if you use Catch2) ---