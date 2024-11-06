#include <SDL2/SDL.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>
#include <climits> // Include for INT_MAX

// Maze generation and solving with SDL
const int WINDOW_SIZE = 600;
const int CELL_SIZE = 20;
const int MAZE_SIZE = WINDOW_SIZE / CELL_SIZE;
const int FADE_SPEED = 10; // Speed of fading effect

enum Direction { NORTH = 1, SOUTH = 2, EAST = 4, WEST = 8 };
std::random_device rd;
std::mt19937 rng(rd());

struct Point {
    int x, y;

    // Operator to compare two Points
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

class Maze {
public:
    Maze(int size) : size(size), cells(size * size, 0) {
        generateMaze();
    }

    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Set background to black
        SDL_RenderClear(renderer);

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                int cell = cells[y * size + x];
                int nx = x * CELL_SIZE;
                int ny = y * CELL_SIZE;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Walls color
                if (!(cell & NORTH)) SDL_RenderDrawLine(renderer, nx, ny, nx + CELL_SIZE, ny);
                if (!(cell & EAST)) SDL_RenderDrawLine(renderer, nx + CELL_SIZE, ny, nx + CELL_SIZE, ny + CELL_SIZE);
                if (!(cell & SOUTH)) SDL_RenderDrawLine(renderer, nx, ny + CELL_SIZE, nx + CELL_SIZE, ny + CELL_SIZE);
                if (!(cell & WEST)) SDL_RenderDrawLine(renderer, nx, ny, nx, ny + CELL_SIZE);
            }
        }

        // Draw the path once after generating the maze
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE); // Path color
        for (const auto& p : path) {
            SDL_RenderDrawPoint(renderer, p.x * CELL_SIZE + CELL_SIZE / 2, p.y * CELL_SIZE + CELL_SIZE / 2);
        }

        SDL_RenderPresent(renderer);
    }

    void solveMaze(SDL_Renderer* renderer) {
        // A* algorithm to find the path
        std::priority_queue<std::pair<int, Point>, std::vector<std::pair<int, Point>>, std::greater<>> openSet;
        std::vector<int> dist(size * size, INT_MAX);
        std::vector<Point> cameFrom(size * size, {-1, -1}); // To reconstruct path
        dist[0] = 0;
        openSet.push({0, {0, 0}});

        while (!openSet.empty()) {
            Point p = openSet.top().second;
            openSet.pop();

            if (p.x == size - 1 && p.y == size - 1) break; // End reached

            std::vector<Point> neighbors = getNeighbors(p);
            for (Point neighbor : neighbors) {
                int alt = dist[p.y * size + p.x] + 1;
                if (alt < dist[neighbor.y * size + neighbor.x]) {
                    dist[neighbor.y * size + neighbor.x] = alt;
                    cameFrom[neighbor.y * size + neighbor.x] = p; // Keep track of the path
                    openSet.push({alt + manhattan(neighbor, {size - 1, size - 1}), neighbor});
                }
            }
        }

        // Store the path for visualization
        path.clear();
        Point current = {size - 1, size - 1};
        while (!(current.x == 0 && current.y == 0)) {
            path.push_back(current);
            current = cameFrom[current.y * size + current.x];
        }
        path.push_back({0, 0}); // Add the start point
        std::reverse(path.begin(), path.end()); // Reverse to start from the beginning
    }

    void moveNavigator(SDL_Renderer* renderer) {
        Point navigator = {0, 0}; // Starting position
        int alpha = 255; // For fading effect
        bool fadingOut = true;

        while (navigator.x != size - 1 || navigator.y != size - 1) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Clear background
            draw(renderer); // Draw maze and path

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, alpha); // Color for the navigator (circle)
            int radius = CELL_SIZE / 4;

            // Draw the navigator as a circle
            for (int w = 0; w < 2 * radius; w++) {
                for (int h = 0; h < 2 * radius; h++) {
                    int dx = radius - w; // horizontal offset
                    int dy = radius - h; // vertical offset
                    if ((dx * dx + dy * dy) <= (radius * radius)) {
                        SDL_RenderDrawPoint(renderer, navigator.x * CELL_SIZE + dx, navigator.y * CELL_SIZE + dy);
                    }
                }
            }

            SDL_RenderPresent(renderer);

            // Update alpha for fade effect
            if (fadingOut) {
                alpha -= FADE_SPEED; // Fade out
                if (alpha <= 0) {
                    fadingOut = false; // Start fading in
                }
            } else {
                alpha += FADE_SPEED; // Fade in
                if (alpha >= 255) {
                    fadingOut = true; // Start fading out again
                }
            }

            SDL_Delay(100); // Delay for visualization speed

            // Move to the next step in the path
            navigator = path[std::min(static_cast<size_t>(path.size() - 1), static_cast<size_t>(std::find(path.begin(), path.end(), navigator) - path.begin() + 1))];
        }
    }

private:
    int size;
    std::vector<int> cells;
    std::vector<Point> path; // Path from start to end

    void generateMaze() {
        std::stack<Point> stack;
        stack.push({0, 0});

        while (!stack.empty()) {
            Point p = stack.top();
            stack.pop();

            std::vector<Direction> directions = {NORTH, SOUTH, EAST, WEST};
            std::shuffle(directions.begin(), directions.end(), rng);

            for (Direction dir : directions) {
                int nx = p.x, ny = p.y;
                if (move(nx, ny, dir) && !cells[ny * size + nx]) {
                    cells[p.y * size + p.x] |= dir;
                    cells[ny * size + nx] |= opposite(dir);
                    stack.push({nx, ny});
                }
            }
        }
    }

    bool move(int& x, int& y, Direction dir) {
        switch (dir) {
            case NORTH: if (y > 0) --y; else return false; break;
            case SOUTH: if (y < size - 1) ++y; else return false; break;
            case EAST: if (x < size - 1) ++x; else return false; break;
            case WEST: if (x > 0) --x; else return false; break;
        }
        return true;
    }

    Direction opposite(Direction dir) {
        switch (dir) {
            case NORTH: return SOUTH;
            case SOUTH: return NORTH;
            case EAST: return WEST;
            case WEST: return EAST;
        }
        return NORTH; // Default return
    }

    int manhattan(const Point& a, const Point& b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    std::vector<Point> getNeighbors(const Point& p) {
        std::vector<Point> neighbors;
        if (p.y > 0 && (cells[p.y * size + p.x] & NORTH)) neighbors.push_back({p.x, p.y - 1});
        if (p.y < size - 1 && (cells[p.y * size + p.x] & SOUTH)) neighbors.push_back({p.x, p.y + 1});
        if (p.x < size - 1 && (cells[p.y * size + p.x] & EAST)) neighbors.push_back({p.x + 1, p.y});
        if (p.x > 0 && (cells[p.y * size + p.x] & WEST)) neighbors.push_back({p.x - 1, p.y});
        return neighbors;
    }
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Maze Generator and Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Maze maze(MAZE_SIZE);
    maze.draw(renderer);
    SDL_Delay(2000);
    maze.solveMaze(renderer);
    maze.moveNavigator(renderer);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
