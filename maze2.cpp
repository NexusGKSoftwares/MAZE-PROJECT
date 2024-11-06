#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

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

// Function to generate the maze using Prim's Algorithm
void generateMaze() {
    std::srand(std::time(0));

    std::vector<std::pair<int, int>> walls; // List of walls to consider
    int startX = 0, startY = 0;
    maze[startX][startY].visited = true;

    // Add walls of the starting cell to the wall list
    for (int i = 0; i < 4; i++) {
        int nx = startX + DX[i], ny = startY + DY[i];
        if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny < MAZE_HEIGHT) {
            walls.push_back({startX, startY});
        }
    }

    // While there are walls to consider
    while (!walls.empty()) {
        // Randomly pick a wall and check if it can be part of the maze
        int index = std::rand() % walls.size();
        int cx = walls[index].first;
        int cy = walls[index].second;
        walls.erase(walls.begin() + index);

        // Check the adjacent cell
        int nx = cx + DX[std::rand() % 4], ny = cy + DY[std::rand() % 4];

        if (nx >= 0 && ny >= 0 && nx < MAZE_WIDTH && ny < MAZE_HEIGHT && !maze[nx][ny].visited) {
            maze[cx][cy].walls[std::rand() % 4] = false; // Remove wall
            maze[nx][ny].visited = true;
            // Add adjacent walls to the wall list
            for (int i = 0; i < 4; i++) {
                int nnx = nx + DX[i], nny = ny + DY[i];
                if (nnx >= 0 && nny >= 0 && nnx < MAZE_WIDTH && nny < MAZE_HEIGHT) {
                    walls.push_back({nnx, nny});
                }
            }
        }
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
    return 0;
}
