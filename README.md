```markdown
# MixLab

C++ / wxWidgets application for visualizing 
chemical element reactions with animation.

## Team
- Nataliia
- Anh

## Idea
User selects two chemical elements and the number 
of atoms. The app shows a reaction animation and 
determines whether the compound is safe or dangerous.

## Technologies

* C++17
* wxWidgets
* CMake

## Build and Run

1. Install dependencies (e.g., wxWidgets):
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

---
**Repository contains only the minimum required files:**
- Source files (*.cpp, *.hpp)
- CMakeLists.txt
- README.md
- .gitignore (ignores build/, CMakeFiles/, mixlab)

All build and temporary files are automatically ignored.
