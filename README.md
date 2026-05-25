# MixLab

C++ / wxWidgets application for visualizing chemical element reactions with animation.

## Team

- Nataliia
- Anh

## Idea

The user selects two chemical elements and the number of atoms.  
The application shows a reaction animation and determines whether the compound is safe, dangerous or unknown.

## Technologies

- C++17
- wxWidgets
- CMake

## Simple Project Structure

This is a small school project, so the structure is intentionally simple.

```text
MixLab/
├── CMakeLists.txt
├── README.md
├── .gitignore
│
├── atom_app.hpp
├── atom_app.cpp
├── atom_frame.hpp
├── atom_frame.cpp
├── atom_canvas.hpp
├── atom_canvas.cpp
│
├── reaction_api.hpp
├── fallback_reaction.cpp
├── ui_state.hpp
├── reaction_controller.cpp
│
├── molecule_data.hpp
├── molecule_data.cpp
├── atom_mixer.hpp
├── atom_mixer.cpp
├── reaction_adapter.cpp
├── reaction_history.cpp
│
├── gui_sidebar.hpp
├── gui_sidebar.cpp
├── gui_menu.hpp
├── gui_menu.cpp
├── gui_dialogs.hpp
├── gui_dialogs.cpp
│
├── test_logic.cpp
└── project_division.html
```

## Balanced 50/50 Project Division

The project is divided into two equally important parts.  
Both participants work on logic and user interface tasks.

## Participant 1: Application Core, Integration and Animation

Participant 1 works on the runnable application core.

### Logic tasks

- `reaction_api.hpp`  
  Shared interface between the user interface and the reaction logic.

- `fallback_reaction.cpp`  
  Small demo/fallback reaction logic for early testing.

- `ui_state.hpp`  
  Stores selected elements, atom counts, speed, theme mode and current result.

- `reaction_controller.cpp`  
  Connects button clicks and user input with reaction calculation and result display.

### Interface tasks

- `atom_app.hpp` / `atom_app.cpp`  
  Starts the wxWidgets application.

- `atom_frame.hpp` / `atom_frame.cpp`  
  Creates the main window, toolbar, speed slider, theme button and main layout.

- `atom_canvas.hpp` / `atom_canvas.cpp`  
  Draws the animation area, particles, background grid and reaction animation.

- Result row  
  Shows formula, molecule name, status and hint.

## Participant 2: Chemical Logic, Sidebar and Dialogs

Participant 2 works on the chemical reaction model and additional interface parts.

### Logic tasks

- `molecule_data.hpp` / `molecule_data.cpp`  
  Stores elements and known molecules.

- `atom_mixer.hpp` / `atom_mixer.cpp`  
  Finds matching molecules and checks atom ratios.

- `reaction_adapter.cpp`  
  Connects the final reaction logic with `reaction_api.hpp`.

- `reaction_history.cpp`  
  Stores performed reactions.

- `test_logic.cpp`  
  Tests molecule data, reaction search and ratio checks.

### Interface tasks

- `gui_sidebar.hpp` / `gui_sidebar.cpp`  
  Sidebar with element list and reaction history.

- `gui_menu.hpp` / `gui_menu.cpp`  
  Menu entries and menu actions.

- `gui_dialogs.hpp` / `gui_dialogs.cpp`  
  Help dialog, element table, about dialog and reaction overview.

- Saved/export view  
  Simple interface for saved reactions or text export.


## Integration Concept

The user interface and the chemical logic are connected through `reaction_api.hpp`.

```text
User input
   ↓
UI state
   ↓
Reaction controller
   ↓
reaction_api.hpp
   ↓
Fallback logic or final reaction logic
   ↓
Result row and animation
```

This makes it possible to work on both parts independently and connect them later.

## Build and Run

1. Install dependencies, for example wxWidgets:

   ```sh
   sudo apt install libwxgtk3.2-dev
   ```

2. Clone the repository and enter the project directory:

   ```sh
   git clone <repository-url>
   cd MixLab
   ```

3. Build the project:

   ```sh
   cmake .
   make
   ```

4. Run the application:

   ```sh
   ./mixlab
   ```

## Repository Contents

The repository should contain only the required project files:

- source files: `*.cpp`, `*.hpp`;
- `CMakeLists.txt`;
- `README.md`;
- `.gitignore`;
- `project_division.html`, if a visual project division is needed.

Build and temporary files should not be committed:

- `build/`;
- `CMakeFiles/`;
- `CMakeCache.txt`;
- `Makefile`;
- `compile_commands.json`;
- `mixlab`;
- `Testing/Temporary/`.
