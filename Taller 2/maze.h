#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <algorithm>
#include <random>


class Maze {
public:
    struct Point {
            int x, y;
            Point(int x, int y) : x(x), y(y) {}
    };
private:
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<bool>> visited;
    int rows, cols;
    std::vector<std::pair<int, int>> directions = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};  // Direcciones para moverse

    
public:
    bool isValid(int x, int y) {
        return x >= 0 && y >= 0 && x < rows && y < cols;
    }

    bool isWall(Point pt) {
        return grid[pt.x][pt.y] == 1;
    }

    void markVisited(Point pt) {
        visited[pt.x][pt.y] = true;
    }

    void markPath(Point pt) {
        grid[pt.x][pt.y] = 2;
    }

     Maze(int r, int c) : rows(r), cols(c) {
        srand(time(0));

        // Inicializar el laberinto con todas las paredes
        grid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 1));
        visited = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false));

        // Pila para DFS
        std::stack<std::pair<int, int>> s;
        s.push({1, 1});
        grid[1][1] = 0;

        std::random_device rd;
        std::mt19937 g(rd());

        while (!s.empty()) {
            int x = s.top().first;
            int y = s.top().second;
            s.pop();

            // Mezclar direcciones
            std::shuffle(directions.begin(), directions.end(), g);

            for (auto dir : directions) {
                int newX = x + dir.first;
                int newY = y + dir.second;

                if (isValid(newX, newY) && grid[newX][newY] == 1) {
                    if (dir.first == 0) {
                        grid[x][y + dir.second / 2] = 0;
                    } else {
                        grid[x + dir.first / 2][y] = 0;
                    }
                    grid[newX][newY] = 0;
                    s.push({newX, newY});
                }
            }
        }

        // Asegurar la entrada y la salida
        grid[0][1] = 0;
        grid[rows-1][cols-2] = 0;
    }

    bool isVisited(Point pt) {
        return visited[pt.x][pt.y];
    }

    bool isInside(Point pt) {
        return pt.x >= 0 && pt.y >= 0 && pt.x < rows && pt.y < cols;
    }

    void display() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i == 0 && j == 1) {
                    std::cout << " E ";
                } else if (i == rows - 1 && j == cols - 2) {
                    std::cout << " S ";
                } else if (grid[i][j] == 1) {
                    std::cout << "###";  // Pared
                } else if (grid[i][j] == 2) {
                    std::cout << " * ";  // Pared 
                }else {
                    std::cout << "   ";  // Camino
                }
            }
            std::cout << std::endl;
        }
    }

    Point getEntry() {
        return Point(0, 1);
    }

    Point getExit() {
        return Point(rows - 1, cols - 2);
    }

    std::vector<Point> getNeighbors(Point pt) {
        return {
            Point(pt.x+1, pt.y),
            Point(pt.x-1, pt.y),
            Point(pt.x, pt.y+1),
            Point(pt.x, pt.y-1)
        };
    }
};

#endif //MAZE_H