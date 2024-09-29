#include <iostream>
#include "maze.h"

bool solveMaze(Maze& maze, Maze::Point pt);


int main() {
    int N, M;  // Solo funcionan enteros impares
    N = 11;
    M = 11;
    Maze maze(N, M);
    maze.display();
    std::cout << "----SOLUCION-----" << std::endl;
    solveMaze(maze, maze.getEntry());
    maze.display();
    
    return 0;
}

/**
 * Solves a given maze by recursively exploring neighboring points.
 *
 * @param maze The maze to be solved.
 * @param pt The current point in the maze.
 *
 * @return True if the maze has been solved, false otherwise.
 */
bool solveMaze(Maze& maze, Maze::Point pt) {
    // Caso base
    if (pt.x == maze.getExit().x && pt.y == maze.getExit().y) {
        return true;
    }

    // Caso base
    if (!maze.isValid(pt.x, pt.y) || maze.isWall(pt) || maze.isVisited(pt)) {
        return false;
    }

    // Caso recursivo

    // Marcar punto como visitado
    maze.markVisited(pt);

    // Explorar vecinos
    for (auto p: maze.getNeighbors(pt)) {
        if (solveMaze(maze, p)) {
            maze.markPath(pt);
            return true;
        }
    }

    // No hay solucion
    return false;
}