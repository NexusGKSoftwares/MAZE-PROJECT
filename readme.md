

---

# Maze Generation and Solving Project Documentation

## Table of Contents
1. [Project Overview](#project-overview)
2. [Features](#features)
3. [Technologies Used](#technologies-used)
4. [Setup Instructions](#setup-instructions)
5. [Code Explanation](#code-explanation)
6. [Usage](#usage)
7. [Future Improvements](#future-improvements)
8. [Contributors](#contributors)
9. [License](#license)

## Project Overview
Welcome to the Maze Generation and Solving project! This innovative application utilizes the power of C++ and the SDL2 library to generate captivating random mazes. By employing a randomized depth-first search algorithm, it creates intricate maze patterns, while the A* pathfinding algorithm elegantly visualizes the solution from start to finish. With an intuitive interface and engaging graphics, this project showcases the beauty of algorithms in action.

## Features
- **Dynamic Maze Generation:** Experience the thrill of watching a random maze come to life through a depth-first search algorithm.
- **Intuitive Pathfinding Visualization:** Witness the A* algorithm as it expertly navigates the maze, illuminating the path from start to exit.
- **Customizable Maze Size:** Tailor your experience by adjusting the size of the maze to your preference.
- **Image-Based Navigator:** Replace the traditional circle navigator with a visually appealing image that indicates the current position in the maze.
- **Interactive User Experience:** Enjoy a seamless interface that makes exploring the maze both fun and engaging.

## Technologies Used
- **Programming Language:** C++
- **Graphics Library:** SDL2 (Simple DirectMedia Layer)
- **Development Environment:** Visual Studio Code or any C++ IDE of your choice
- **Build Tools:** CMake or Makefiles (as applicable)

## Setup Instructions

### Prerequisites
To get started, ensure you have the following installed on your machine:
- A C++ compiler (e.g., GCC, Clang)
- The SDL2 development libraries
- The SDL2_image library (for loading various image formats)

### Installation Steps
1. **Clone the Repository:**
   Start by cloning the project repository to your local machine:
   ```bash
   git clone <repository-url>
   cd <project-directory>
   ```

2. **Install SDL2:**
   - **On Ubuntu:**
     ```bash
     sudo apt-get install libsdl2-dev libsdl2-image-dev
     ```
   - **On Windows:** Download the SDL2 development libraries from the [SDL website](https://www.libsdl.org/download-2.0.php) and follow the installation instructions provided.

3. **Build the Project:**
   Create a build directory and compile the code:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the Application:**
   Execute the generated binary to see the magic unfold:
   ```bash
   ./maze_solver
   ```

5. **Prepare an Image:**
   Make sure you have an image file named `image.png` located in the same directory as the executable. This image will serve as the navigator within the maze.

## Code Explanation

### Structure
The code is organized into several components, each playing a vital role in the maze generation and solving process.

#### 1. Point Struct
The `Point` struct represents a coordinate in the maze grid, encapsulating both `x` and `y` values. It features overloaded operators for easy comparison.

#### 2. Maze Class
The heart of the project lies within the `Maze` class, which includes methods for generating, drawing, solving, and moving the navigator through the maze.

- **Constructor:** Initializes the maze size and triggers the maze generation process.
- **generateMaze():** Implements a randomized depth-first search algorithm to create the maze structure.
- **draw():** Renders the maze walls and the solution path on the screen.
- **solveMaze():** Utilizes the A* algorithm to determine the optimal path from the start point to the exit.
- **moveNavigator():** Visualizes the movement of the navigator image along the calculated solution path.

#### 3. Main Function
The main entry point of the application, responsible for initializing SDL, creating the maze, solving it, and managing user interactions.

### Key Functions and Algorithms
- **Randomized Depth-First Search:** A recursive algorithm that generates the maze by exploring random paths and backtracking when necessary.
- **A* Pathfinding Algorithm:** An efficient algorithm that finds the shortest route from the start to the goal using heuristics.

## Usage
Upon launching the application, a captivating maze will be generated and displayed. After a brief pause, the solution path will be revealed in green, guiding you through the maze. The navigator image will dynamically move along this path, providing a visual representation of the solution journey.

## Future Improvements
This project has immense potential for growth. Here are a few ideas for future enhancements:
- **Implement Alternative Maze Generation Algorithms:** Explore options like Prim's or Kruskal's algorithms for generating diverse maze structures.
- **User-Defined Maze Size and Difficulty Levels:** Allow users to customize their experience by selecting maze dimensions and difficulty settings.
- **Enhanced Graphical User Interface (GUI):** Improve interactivity with a more polished user interface.
- **User-Controlled Navigation:** Enable users to navigate the maze manually for a more immersive experience.
- **Maze Saving and Loading Functionality:** Allow users to save generated mazes and revisit them later.

## Contributors
- **NexusGK** - Developer, Designer, and Documenter of this project.

## License
This project is licensed under the MIT License. For more details, please refer to the [LICENSE](LICENSE) file.

---


# 🌀 Maze Generator and Solver

Welcome to the Maze Generator and Solver project! 🚀 This interactive program, built in C++ with SDL2, creates and solves a maze right before your eyes. Originally designed for Ubuntu, but no worries — here’s how to get it running smoothly on your Windows system! 💻✨

## Prerequisites

### 1. Install SDL2 🧩
- Download the **SDL2** development libraries for Windows from the [SDL2 official website](https://www.libsdl.org/).
- Extract the folder and place it somewhere convenient, like `C:\SDL2`.

### 2. Install a C++ Compiler 🛠️
This project works best with either **MinGW** or **Visual Studio** as the compiler.

#### Option A: Using MinGW
- Download **MinGW** from the [MinGW website](https://www.mingw-w64.org/).
- Install MinGW, and make sure to add the `bin` directory to your system’s PATH. 🌐

#### Option B: Using Visual Studio
- If you prefer **Visual Studio**, ensure it’s set up to compile C++ projects.
- Don’t forget to configure SDL2 paths in the project settings!

## Setting Up SDL2 with Your Compiler

### Setting Up SDL2 with MinGW 📝
1. Copy `SDL2.dll` from the SDL2 folder (usually in the `lib` or `bin` folder) to your project directory.
2. Compile your project with the following command:
   ```bash
   g++ maze.cpp -o maze.exe -I"C:\path\to\SDL2\include" -L"C:\path\to\SDL2\lib" -lmingw32 -lSDL2main -lSDL2
   ```
   🔧 Replace `C:\path\to\SDL2` with your SDL2 installation path.

### Setting Up SDL2 with Visual Studio 🎨
1. Open Visual Studio and create a new project.
2. Go to **Project Properties**:
   - Under **VC++ Directories**, add the `include` folder of SDL2 to **Include Directories** and the `lib` folder to **Library Directories**.
   - Under **Linker > Input**, add `SDL2.lib` and `SDL2main.lib` to **Additional Dependencies**.
3. Make sure `SDL2.dll` is in the project’s output directory (where your `.exe` is generated).

## Running the Program 🚦

1. Open a terminal in your project directory.
2. If using MinGW, run the following:
   ```bash
   maze.exe
   ```
   👉 If using Visual Studio, press **F5** to build and run the project from within the IDE.

## Troubleshooting 🛑

- **SDL2.dll Not Found**: Make sure `SDL2.dll` is in the same folder as `maze.exe`.
- **Library Linking Issues**: Double-check the SDL2 installation path and linking flags.

## Additional Notes 📌

- Want to adjust the maze size or cell size? Edit `WINDOW_SIZE` or `CELL_SIZE` in `maze.cpp`.
- For the best experience, keep your graphics drivers up to date! 🎮

Enjoy solving your maze adventure, and feel free to reach out if you hit a roadblock! 🚀✨

---
