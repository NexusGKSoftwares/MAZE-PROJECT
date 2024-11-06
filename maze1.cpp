<<<<<<< HEAD
<<<<<<< HEAD
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
<<<<<<< HEAD
<<<<<<< HEAD
#include <algorithm> // Include for std::shuffle
#include <random>    // Include for random number generator

// Constants for window and maze dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = 20;
const int MAZE_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int MAZE_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// Directions for moving in the maze
const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

// Cell structure to represent each cell in the maze
struct Cell {
    bool visited = false;
    bool walls[4] = {true, true, true, true};
    bool path = false; // Used to mark the path when solving
};

std::vector<std::vector<Cell>> maze(MAZE_WIDTH, std::vector<Cell>(MAZE_HEIGHT));

// SDL Window and Renderer
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Function to initialize SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Maze Generator and Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

// Custom function to draw a circle in SDL
void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

// Function to draw the maze and the solver circle
void drawMaze(int currentX = -1, int currentY = -1) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int x = 0; x < MAZE_WIDTH; ++x) {
        for (int y = 0; y < MAZE_HEIGHT; ++y) {
            Cell& cell = maze[x][y];
            int x1 = x * CELL_SIZE;
            int y1 = y * CELL_SIZE;

            if (cell.walls[0]) SDL_RenderDrawLine(renderer, x1, y1, x1 + CELL_SIZE, y1); // Top
            if (cell.walls[1]) SDL_RenderDrawLine(renderer, x1 + CELL_SIZE, y1, x1 + CELL_SIZE, y1 + CELL_SIZE); // Right
            if (cell.walls[2]) SDL_RenderDrawLine(renderer, x1 + CELL_SIZE, y1 + CELL_SIZE, x1, y1 + CELL_SIZE); // Bottom
            if (cell.walls[3]) SDL_RenderDrawLine(renderer, x1, y1 + CELL_SIZE, x1, y1); // Left

            if (cell.path) { // Draw path
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_Rect rect = {x1 + 1, y1 + 1, CELL_SIZE - 2, CELL_SIZE - 2};
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
        }
    }

    // Draw the circle at the current position
    if (currentX != -1 && currentY != -1) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        int centerX = currentX * CELL_SIZE + CELL_SIZE / 2;
        int centerY = currentY * CELL_SIZE + CELL_SIZE / 2;
        SDL_RenderDrawCircle(renderer, centerX, centerY, CELL_SIZE / 4);
    }

    SDL_RenderPresent(renderer);
}

// Function to generate the maze using a depth-first search algorithm
void generateMaze() {
    std::srand(std::time(0));
    std::stack<std::pair<int, int>> stack;
    int x = 0, y = 0;
    maze[x][y].visited = true;
    stack.push({x, y});

    std::random_device rd;
    std::mt19937 rng(rd());

    while (!stack.empty()) {
        auto [cx, cy] = stack.top();
        std::vector<int> directions = {0, 1, 2, 3};
        std::shuffle(directions.begin(), directions.end(), rng);

        bool moved = false;
        for (int i : directions) {
            int nx = cx + DX[i];
            int ny = cy + DY[i];
            if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny < MAZE_HEIGHT && !maze[nx][ny].visited) {
                maze[cx][cy].walls[i] = false;
                maze[nx][ny].walls[i ^ 1] = false;
                maze[nx][ny].visited = true;
                stack.push({nx, ny});
                moved = true;
                break;
            }
        }
        if (!moved) stack.pop();
    }
}

// Function to solve the maze visually
bool solveMaze(int x, int y) {
    if (x == MAZE_WIDTH - 1 && y == MAZE_HEIGHT - 1) {
        maze[x][y].path = true;
        return true;
    }

    maze[x][y].path = true;
    drawMaze(x, y);
    SDL_Delay(50); // Delay for visual effect

    for (int i = 0; i < 4; ++i) {
        int nx = x + DX[i];
        int ny = y + DY[i];
        if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny < MAZE_HEIGHT && !maze[nx][ny].path && !maze[x][y].walls[i]) {
            if (solveMaze(nx, ny)) return true;
        }
    }

    maze[x][y].path = false;
    drawMaze(x, y);
    SDL_Delay(50); // Delay for visual effect
    return false;
}

// Function to clean up SDL resources
void cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    if (!initSDL()) return -1;

    generateMaze();
    drawMaze();

    // Solve the maze visually
    solveMaze(0, 0);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    cleanUp();
=======
=======
#include <SDL2/SDL.h>
>>>>>>> ab521e6 ("Refactored maze generation algorithm, added SDL for visualization, and modified main function to handle events and cleanup.")
#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
=======
#include <algorithm> // Include this header for std::shuffle
#include <random>    // Include this header for random number generator
>>>>>>> df7d0ed ("Added includes for algorithm and random headers, and replaced std::random_shuffle with std::shuffle and introduced random number generator.")
=======
#include <algorithm> // Include for std::shuffle
#include <random>    // Include for random number generator
>>>>>>> 8e19349 ("Added maze solving functionality and visual representation")

// Constants for window and maze dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = 20;
const int MAZE_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int MAZE_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// Directions for moving in the maze
const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

// Cell structure to represent each cell in the maze
struct Cell {
    bool visited = false;
    bool walls[4] = {true, true, true, true};
    bool path = false; // Used to mark the path when solving
};

std::vector<std::vector<Cell>> maze(MAZE_WIDTH, std::vector<Cell>(MAZE_HEIGHT));

// SDL Window and Renderer
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Function to initialize SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Maze Generator and Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

// Function to draw the maze and the solver circle
void drawMaze(int currentX = -1, int currentY = -1) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int x = 0; x < MAZE_WIDTH; ++x) {
        for (int y = 0; y < MAZE_HEIGHT; ++y) {
            Cell& cell = maze[x][y];
            int x1 = x * CELL_SIZE;
            int y1 = y * CELL_SIZE;

            if (cell.walls[0]) SDL_RenderDrawLine(renderer, x1, y1, x1 + CELL_SIZE, y1); // Top
            if (cell.walls[1]) SDL_RenderDrawLine(renderer, x1 + CELL_SIZE, y1, x1 + CELL_SIZE, y1 + CELL_SIZE); // Right
            if (cell.walls[2]) SDL_RenderDrawLine(renderer, x1 + CELL_SIZE, y1 + CELL_SIZE, x1, y1 + CELL_SIZE); // Bottom
            if (cell.walls[3]) SDL_RenderDrawLine(renderer, x1, y1 + CELL_SIZE, x1, y1); // Left

            if (cell.path) { // Draw path
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_Rect rect = {x1 + 1, y1 + 1, CELL_SIZE - 2, CELL_SIZE - 2};
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
        }
    }

    // Draw the circle at the current position
    if (currentX != -1 && currentY != -1) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        int centerX = currentX * CELL_SIZE + CELL_SIZE / 2;
        int centerY = currentY * CELL_SIZE + CELL_SIZE / 2;
        for (int w = 0; w < CELL_SIZE / 4; ++w) {
            SDL_RenderDrawCircle(renderer, centerX, centerY, CELL_SIZE / 4 - w);
        }
    }

    SDL_RenderPresent(renderer);
}

// Custom function to draw a circle in SDL
void SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

// Function to generate the maze using a depth-first search algorithm
void generateMaze() {
    std::srand(std::time(0));
    std::stack<std::pair<int, int>> stack;
    int x = 0, y = 0;
    maze[x][y].visited = true;
    stack.push({x, y});

    std::random_device rd;
    std::mt19937 rng(rd());

    while (!stack.empty()) {
        auto [cx, cy] = stack.top();
        std::vector<int> directions = {0, 1, 2, 3};
        std::shuffle(directions.begin(), directions.end(), rng);

        bool moved = false;
        for (int i : directions) {
            int nx = cx + DX[i];
            int ny = cy + DY[i];
            if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny < MAZE_HEIGHT && !maze[nx][ny].visited) {
                maze[cx][cy].walls[i] = false;
                maze[nx][ny].walls[i ^ 1] = false;
                maze[nx][ny].visited = true;
                stack.push({nx, ny});
                moved = true;
                break;
            }
        }
        if (!moved) stack.pop();
    }
}

// Function to solve the maze visually
bool solveMaze(int x, int y) {
    if (x == MAZE_WIDTH - 1 && y == MAZE_HEIGHT - 1) {
        maze[x][y].path = true;
        return true;
    }

    maze[x][y].path = true;
    drawMaze(x, y);
    SDL_Delay(50); // Delay for visual effect

    for (int i = 0; i < 4; ++i) {
        int nx = x + DX[i];
        int ny = y + DY[i];
        if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny < MAZE_HEIGHT && !maze[nx][ny].path && !maze[x][y].walls[i]) {
            if (solveMaze(nx, ny)) return true;
        }
    }

    maze[x][y].path = false;
    drawMaze(x, y);
    SDL_Delay(50); // Delay for visual effect
    return false;
}

// Function to clean up SDL resources
void cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    if (!initSDL()) return -1;

    generateMaze();
<<<<<<< HEAD
    printMaze();
>>>>>>> fb32ccf ("Added maze generation algorithm implementation in C++")
=======
    drawMaze();

    // Solve the maze visually
    solveMaze(0, 0);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
    }

    cleanUp();
>>>>>>> ab521e6 ("Refactored maze generation algorithm, added SDL for visualization, and modified main function to handle events and cleanup.")
    return 0;
}
