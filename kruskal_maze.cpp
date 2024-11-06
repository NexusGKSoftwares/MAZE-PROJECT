#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>

const int WIDTH = 800;  // Width of the window
const int HEIGHT = 600; // Height of the window
const int ROWS = 30;    // Number of rows
const int COLS = 30;    // Number of columns
const int CELL_SIZE = 20; // Size of each cell

struct Edge {
    int x1, y1, x2, y2; // Two points of the wall
};

class DisjointSet {
public:
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void union_sets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

class Maze {
public:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    Maze(int rows, int cols) : rows(rows), cols(cols), disjointSet(rows * cols) { 
        maze.resize(rows, std::vector<int>(cols, 1)); // Initialize maze with walls (1)
        walls = generateWalls(); // Generate all possible walls
=======
    Maze(int rows, int cols) : rows(rows), cols(cols) {
        maze.resize(rows, std::vector<int>(cols, 1)); // Initialize maze with walls (1)
        walls = generateWalls(); // Generate all possible walls
        disjointSet = DisjointSet(rows * cols);
>>>>>>> 58fe050 ("Added Kruskal's algorithm maze generation implementation with SDL visualization")
=======
    Maze(int rows, int cols) : rows(rows), cols(cols), disjointSet(rows * cols) {  // Pass rows * cols to DisjointSet constructor
=======
    Maze(int rows, int cols) : rows(rows), cols(cols), disjointSet(rows * cols) { 
>>>>>>> 0ef0ffc ("Added debugging output, minor whitespace changes, and clarified wall drawing condition in Maze class.")
        maze.resize(rows, std::vector<int>(cols, 1)); // Initialize maze with walls (1)
        walls = generateWalls(); // Generate all possible walls
>>>>>>> 50512ea ("Moved DisjointSet initialization to Maze constructor initializer list")
    }

    // Function to generate all possible walls
    std::vector<Edge> generateWalls() {
        std::vector<Edge> walls;
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                if (x + 1 < cols) { // Horizontal wall
                    walls.push_back({x, y, x + 1, y});
                }
                if (y + 1 < rows) { // Vertical wall
                    walls.push_back({x, y, x, y + 1});
                }
            }
        }
        return walls;
    }

    // Function to generate the maze using Kruskal's algorithm
    void generate() {
        std::random_shuffle(walls.begin(), walls.end()); // Shuffle walls to ensure randomness

        for (const auto& wall : walls) {
            int cell1 = wall.y1 * cols + wall.x1;
            int cell2 = wall.y2 * cols + wall.x2;

            if (disjointSet.find(cell1) != disjointSet.find(cell2)) {
                disjointSet.union_sets(cell1, cell2);
                maze[wall.y1][wall.x1] = 0; // Remove wall
                maze[wall.y2][wall.x2] = 0; // Remove wall
            }
        }
<<<<<<< HEAD
<<<<<<< HEAD

        std::cout << "Maze generated successfully!" << std::endl;  // Debugging output
=======
>>>>>>> 58fe050 ("Added Kruskal's algorithm maze generation implementation with SDL visualization")
=======

        std::cout << "Maze generated successfully!" << std::endl;  // Debugging output
>>>>>>> 0ef0ffc ("Added debugging output, minor whitespace changes, and clarified wall drawing condition in Maze class.")
    }

    // Function to draw the maze using SDL
    void draw(SDL_Renderer* renderer) {
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
<<<<<<< HEAD
<<<<<<< HEAD
                if (maze[y][x] == 1) {  // If there's a wall
=======
                if (maze[y][x] == 1) {
>>>>>>> 58fe050 ("Added Kruskal's algorithm maze generation implementation with SDL visualization")
=======
                if (maze[y][x] == 1) {  // If there's a wall
>>>>>>> 0ef0ffc ("Added debugging output, minor whitespace changes, and clarified wall drawing condition in Maze class.")
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for walls
                    SDL_Rect rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
    }

private:
    int rows, cols;
    std::vector<std::vector<int>> maze;
    std::vector<Edge> walls;
    DisjointSet disjointSet;
};

// SDL2 initialization function
bool init(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *window = SDL_CreateWindow("Kruskal's Algorithm Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

// Main function
int main() {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!init(&window, &renderer)) {
        return 1;
    }

    Maze maze(ROWS, COLS);
<<<<<<< HEAD
<<<<<<< HEAD
    maze.generate(); // Generate the maze
=======
    maze.generate();
>>>>>>> 58fe050 ("Added Kruskal's algorithm maze generation implementation with SDL visualization")
=======
    maze.generate(); // Generate the maze
>>>>>>> 0ef0ffc ("Added debugging output, minor whitespace changes, and clarified wall drawing condition in Maze class.")

    bool quit = false;
    SDL_Event e;

    // Main loop
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for background
        SDL_RenderClear(renderer);

        maze.draw(renderer); // Draw the maze

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
