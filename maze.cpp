#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iostream>

const int CELL_SIZE = 20;
const int WINDOW_SIZE = 600;
const int GRID_SIZE = WINDOW_SIZE / CELL_SIZE;

enum Direction { NONE = 0, NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8 };
enum CellStatus { WALL = 0, PATH = 1, START = 2, END = 3, VISITED = 4 };

struct Point {
    int x, y;

    bool operator<(const Point& other) const {
        if (y == other.y) return x < other.x;
        return y < other.y;
    }
};

struct Cell {
    int x, y, g, h, f;
    Cell* parent;

    bool operator<(const Cell& other) const {
        return f > other.f;
    }
};

class Maze {
private:
    SDL_Renderer* renderer;
    std::vector<int> cells;
    std::vector<CellStatus> status;
    int size;

    int opposite(Direction dir) {
        switch (dir) {
            case NORTH: return SOUTH;
            case SOUTH: return NORTH;
            case EAST:  return WEST;
            case WEST:  return EAST;
            default:    return NONE;
        }
    }

    void carvePassage(int x, int y) {
        std::vector<Direction> directions = { NORTH, EAST, SOUTH, WEST };
        std::random_shuffle(directions.begin(), directions.end());

        for (Direction dir : directions) {
            int nx = x + (dir == EAST ? 1 : dir == WEST ? -1 : 0);
            int ny = y + (dir == SOUTH ? 1 : dir == NORTH ? -1 : 0);

            if (nx >= 0 && ny >= 0 && nx < size && ny < size && cells[ny * size + nx] == WALL) {
                cells[y * size + x] |= dir;
                cells[ny * size + nx] |= opposite(dir);
                carvePassage(nx, ny);
            }
        }
    }

public:
    Maze(int s, SDL_Renderer* r) : size(s), renderer(r), cells(s * s, WALL), status(s * s, WALL) {
        carvePassage(0, 0);
    }

    void draw() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                int cx = x * CELL_SIZE;
                int cy = y * CELL_SIZE;
                int cell = cells[y * size + x];

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                if (!(cell & NORTH)) SDL_RenderDrawLine(renderer, cx, cy, cx + CELL_SIZE, cy);
                if (!(cell & SOUTH)) SDL_RenderDrawLine(renderer, cx, cy + CELL_SIZE, cx + CELL_SIZE, cy + CELL_SIZE);
                if (!(cell & WEST))  SDL_RenderDrawLine(renderer, cx, cy, cx, cy + CELL_SIZE);
                if (!(cell & EAST))  SDL_RenderDrawLine(renderer, cx + CELL_SIZE, cy, cx + CELL_SIZE, cy + CELL_SIZE);
            }
        }
        SDL_RenderPresent(renderer);
    }

    void solveAStar(Point start, Point end) {
        auto heuristic = [](Point a, Point b) {
            return abs(a.x - b.x) + abs(a.y - b.y);
        };

        std::priority_queue<Cell> openSet;
        std::vector<bool> closedSet(size * size, false);
        openSet.push({ start.x, start.y, 0, heuristic(start, end), heuristic(start, end), nullptr });

        while (!openSet.empty()) {
            Cell current = openSet.top();
            openSet.pop();

            int idx = current.y * size + current.x;
            if (closedSet[idx]) continue;
            closedSet[idx] = true;

            if (current.x == end.x && current.y == end.y) {
                for (Cell* cell = &current; cell; cell = cell->parent) {
                    int cx = cell->x * CELL_SIZE + CELL_SIZE / 2;
                    int cy = cell->y * CELL_SIZE + CELL_SIZE / 2;
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderDrawPoint(renderer, cx, cy);
                }
                SDL_RenderPresent(renderer);
                return;
            }

            std::vector<std::pair<Direction, Point>> neighbors = {
                {NORTH, {current.x, current.y - 1}},
                {SOUTH, {current.x, current.y + 1}},
                {EAST, {current.x + 1, current.y}},
                {WEST, {current.x - 1, current.y}}
            };

            for (auto [dir, neighbor] : neighbors) {
                int nx = neighbor.x, ny = neighbor.y;
                if (nx >= 0 && ny >= 0 && nx < size && ny < size && !closedSet[ny * size + nx]) {
                    int cost = current.g + 1;
                    int h = heuristic(neighbor, end);
                    openSet.push({ nx, ny, cost, h, cost + h, new Cell(current) });
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Maze Generator and Solver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Maze maze(GRID_SIZE, renderer);
    maze.draw();
    SDL_Delay(1000);

    Point start = { 0, 0 };
    Point end = { GRID_SIZE - 1, GRID_SIZE - 1 };
    maze.solveAStar(start, end);

    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) && e.type == SDL_QUIT) break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
