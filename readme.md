

---

# 🌀 Maze Generator and Solver

Welcome to the **Maze Generator and Solver** project! 🚀 This C++ application, powered by SDL2, creates and solves mazes with captivating visuals. Experience the beauty of algorithms as you watch mazes being generated and solved before your eyes. Ready to dive in? Let's get started!

> **GitHub Repository:** [MAZE-PROJECT](https://github.com/NexusGKSoftwares/MAZE-PROJECT.git)

---

## 📋 Table of Contents
1. [🔍 Project Overview](#-project-overview)
2. [✨ Features](#-features)
3. [💻 Technologies Used](#-technologies-used)
4. [⚙️ Setup Instructions](#%EF%B8%8F-setup-instructions)
5. [🖥️ How to Run on Different Operating Systems](#%EF%B8%8F-how-to-run-on-different-operating-systems)
6. [🔧 Code Explanation](#-code-explanation)
7. [🎮 Usage](#-usage)
8. [🚀 Future Improvements](#-future-improvements)
9. [🤝 Contributors](#-contributors)
10. [📜 License](#-license)

---

## 🔍 Project Overview
This project is a visual demonstration of maze generation using a **randomized depth-first search algorithm** and maze-solving using the **A* algorithm**. 🧩 It combines stunning graphics with an intuitive interface, making algorithmic pathfinding both fun and educational.

## ✨ Features
- **🌀 Dynamic Maze Generation:** Watch as a maze unfolds in real-time with the depth-first search algorithm.
- **🟢 Pathfinding Visualization:** See the A* algorithm in action as it solves the maze and reveals the optimal path.
- **🔧 Customizable Maze Sizes:** Adjust the maze dimensions to your liking for varied experiences.
- **📸 Image-Based Navigator:** A visually appealing way to follow the solution through the maze.
- **💬 Interactive Graphics:** Experience a seamless and engaging user interface.

---

## 💻 Technologies Used
- **Programming Language:** C++
- **Graphics Library:** SDL2
- **IDE:** Visual Studio Code or any C++ IDE of your choice
- **Build Tools:** CMake, Makefiles

---

## ⚙️ Setup Instructions

### Prerequisites
Ensure you have:
- A **C++ compiler** (e.g., GCC, Clang, or MSVC)
- **SDL2** development libraries
- **SDL2_image** library for image support

---

## 🖥️ How to Run on Different Operating Systems

### 🐧 Running on Linux (Ubuntu)
1. **Install SDL2 and SDL2_image:**
   ```bash
   sudo apt-get install libsdl2-dev libsdl2-image-dev
   ```
2. **Clone the Repository:**
   ```bash
   git clone https://github.com/NexusGKSoftwares/MAZE-PROJECT.git
   cd MAZE-PROJECT
   ```
3. **Build the Project:**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
4. **Run the Application:**
   ```bash
   ./maze_solver
   ```

### 🪟 Running on Windows
1. **Install SDL2:**
   - Download SDL2 from the [SDL website](https://www.libsdl.org/download-2.0.php).
   - Extract the files to `C:\SDL2` (or a path of your choice).
2. **Install a C++ Compiler:**
   - **MinGW:** Download from [MinGW](https://www.mingw-w64.org/) and add it to your PATH.
   - **Visual Studio:** Set up Visual Studio for C++ development.
3. **Set Up SDL2 in Your IDE:**
   - **For MinGW:** Copy `SDL2.dll` to your project directory and compile:
     ```bash
     g++ maze.cpp -o maze.exe -I"C:\path\to\SDL2\include" -L"C:\path\to\SDL2\lib" -lmingw32 -lSDL2main -lSDL2
     ```
   - **For Visual Studio:** Configure the include and library directories.
4. **Run the Program:**
   - **MinGW:** Run `maze.exe` in your terminal.
   - **Visual Studio:** Press **F5** to build and run.

### 🍏 Running on macOS
1. **Install SDL2 and SDL2_image Using Homebrew:**
   ```bash
   brew install sdl2 sdl2_image
   ```
2. **Clone the Repository:**
   ```bash
   git clone https://github.com/NexusGKSoftwares/MAZE-PROJECT.git
   cd MAZE-PROJECT
   ```
3. **Build and Run:**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ./maze_solver
   ```

---

## 🔧 Code Explanation
The project is divided into components that handle maze generation, solving, and rendering. 

- **🌀 Maze Generation:** Uses a depth-first search to create complex paths.
- **🟢 Pathfinding:** Implements the A* algorithm for efficient navigation.

### Key Classes and Algorithms
- **Point Struct:** Represents (x, y) coordinates in the maze grid.
- **Maze Class:** Manages the maze structure, generation, and solving processes.
- **Depth-First Search:** Generates the maze layout.
- **A* Algorithm:** Solves the maze efficiently.

---

## 🎮 Usage
1. **Launch the Program:** The maze will be generated and displayed.
2. **Watch the Solution:** The A* algorithm will illuminate the path, guiding the image-based navigator through the maze.
3. **Explore and Customize:** Modify `WINDOW_SIZE` or `CELL_SIZE` in the code for a different experience.

---

## 🚀 Future Improvements
- **🎲 Multiple Maze Algorithms:** Add new generation techniques like Prim’s or Kruskal’s algorithm.
- **🛠️ Difficulty Settings:** Allow users to choose the maze complexity.
- **📱 Improved GUI:** Introduce a more user-friendly interface.
- **🎮 User-Controlled Navigation:** Let users manually solve the maze.

---

## 🤝 Contributors
- **NexusGK** - Lead Developer and Designer

---

## 📜 License
This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for more information.

---

Feel free to reach out with questions, feedback, or suggestions. Happy maze-solving! 🎉
