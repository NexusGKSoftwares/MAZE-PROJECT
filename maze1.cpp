#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;

enum CellType {
    WALL,
    PATH
};

struct Cell {
    int x, y;
    Cell(int _x, int _y) : x(_x), y(_y) {}
};

vector<vector<CellType>> maze(WIDTH, vector<CellType>(HEIGHT, WALL));
vector<Cell> frontier;

void addFrontier(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && maze[x][y] == WALL) {
        maze[x][y] = PATH;
        frontier.push_back(Cell(x, y));
    }
}

void generateMaze() {
    srand(time(0));
    
    // Start from a random cell
    int startX = rand() % WIDTH;
    int startY = rand() % HEIGHT;
    maze[startX][startY] = PATH;
    addFrontier(startX + 1, startY);
    addFrontier(startX - 1, startY);
    addFrontier(startX, startY + 1);
    addFrontier(startX, startY - 1);

    while (!frontier.empty()) {
        // Choose a random frontier cell
        int randIndex = rand() % frontier.size();
        Cell current = frontier[randIndex];
        frontier.erase(frontier.begin() + randIndex);

        vector<Cell> neighbors;
        
        // Check all four possible neighbors
        if (current.x >= 2 && maze[current.x - 2][current.y] == PATH) neighbors.push_back(Cell(current.x - 2, current.y));
        if (current.x < WIDTH - 2 && maze[current.x + 2][current.y] == PATH) neighbors.push_back(Cell(current.x + 2, current.y));
        if (current.y >= 2 && maze[current.x][current.y - 2] == PATH) neighbors.push_back(Cell(current.x, current.y - 2));
        if (current.y < HEIGHT - 2 && maze[current.x][current.y + 2] == PATH) neighbors.push_back(Cell(current.x, current.y + 2));

        if (!neighbors.empty()) {
            // Connect with a random neighbor
            Cell neighbor = neighbors[rand() % neighbors.size()];
            int wallX = (current.x + neighbor.x) / 2;
            int wallY = (current.y + neighbor.y) / 2;
            maze[wallX][wallY] = PATH;
            maze[current.x][current.y] = PATH;

            // Add adjacent walls to frontier
            addFrontier(current.x + 2, current.y);
            addFrontier(current.x - 2, current.y);
            addFrontier(current.x, current.y + 2);
            addFrontier(current.x, current.y - 2);
        }
    }
}

void printMaze() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            cout << (maze[x][y] == WALL ? "##" : "  ");
        }
        cout << endl;
    }
}

int main() {
    generateMaze();
    printMaze();
    return 0;
}
