#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <sstream>

using json = nlohmann::json;

enum CellState {
    EMPTY,
    WALL,
    START,
    END,
    VISITED,
    PATH
};

struct Cell {
    int row, col;
    CellState state;
    int distance;
    Cell* previous;

    Cell(int r = 0, int c = 0) : row(r), col(c), state(EMPTY), distance(std::numeric_limits<int>::max()), previous(nullptr) {}
};

class PathFinder {
private:
    std::vector<std::vector<Cell>> grid;
    Cell* startCell;
    Cell* endCell;
    int gridSize;

    std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    bool isValid(int row, int col) {
        return row >= 0 && row < gridSize && col >= 0 && col < gridSize;
    }

    std::vector<Cell*> getNeighbors(Cell* cell) {
        std::vector<Cell*> neighbors;
        for (const auto& dir : directions) {
            int newRow = cell->row + dir.first;
            int newCol = cell->col + dir.second;
            if (isValid(newRow, newCol) && grid[newRow][newCol].state != WALL) {
                neighbors.push_back(&grid[newRow][newCol]);
            }
        }
        return neighbors;
    }

public:
    PathFinder(const std::vector<std::vector<int>>& inputGrid, const json& start, const json& end)
        : gridSize(inputGrid.size()), startCell(nullptr), endCell(nullptr) {
        grid.resize(gridSize, std::vector<Cell>(gridSize));
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                grid[i][j] = Cell(i, j);
                grid[i][j].state = static_cast<CellState>(inputGrid[i][j]);
                if (grid[i][j].state == START) startCell = &grid[i][j];
                if (grid[i][j].state == END) endCell = &grid[i][j];
            }
        }
        if (!startCell) startCell = &grid[start["row"].get<int>()][start["col"].get<int>()];
        if (!endCell) endCell = &grid[end["row"].get<int>()][end["col"].get<int>()];
    }

    json dijkstra() {
        std::priority_queue<std::pair<int, Cell*>, std::vector<std::pair<int, Cell*>>, std::greater<>> pq;
        startCell->distance = 0;
        pq.push({ 0, startCell });

        std::vector<std::vector<int>> visited;
        std::vector<std::vector<int>> path;

        while (!pq.empty()) {
            Cell* current = pq.top().second;
            pq.pop();

            if (current == endCell) {
                while (current != startCell) {
                    path.push_back({ current->row, current->col });
                    current = current->previous;
                }
                std::reverse(path.begin(), path.end());
                return { {"visited", visited}, {"path", path} };
            }

            if (current->state == VISITED) continue;
            current->state = VISITED;
            visited.push_back({ current->row, current->col });

            for (Cell* neighbor : getNeighbors(current)) {
                int newDist = current->distance + 1;
                if (newDist < neighbor->distance) {
                    neighbor->distance = newDist;
                    neighbor->previous = current;
                    pq.push({ newDist, neighbor });
                }
            }
        }

        return { {"visited", visited}, {"path", path} };
    }

    json bfs() {
        std::queue<Cell*> q;
        q.push(startCell);
        startCell->state = VISITED;

        std::vector<std::vector<int>> visited;
        std::vector<std::vector<int>> path;

        while (!q.empty()) {
            Cell* current = q.front();
            q.pop();

            visited.push_back({ current->row, current->col });

            if (current == endCell) {
                while (current != startCell) {
                    path.push_back({ current->row, current->col });
                    current = current->previous;
                }
                std::reverse(path.begin(), path.end());
                return { {"visited", visited}, {"path", path} };
            }

            for (Cell* neighbor : getNeighbors(current)) {
                if (neighbor->state == EMPTY || neighbor->state == END) {
                    neighbor->state = VISITED;
                    neighbor->previous = current;
                    q.push(neighbor);
                }
            }
        }

        return { {"visited", visited}, {"path", path} };
    }
};

int main(int argc, char* argv[]) {
    std::ofstream logFile("pathfinder_log.txt", std::ios::app);

    logFile << "Pathfinder started" << std::endl;
    logFile.flush();

    if (argc < 2) {
        logFile << "Error: No input file specified" << std::endl;
        logFile.flush();
        std::cerr << "Error: No input file specified" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    logFile << "Reading input from file: " << inputFilename << std::endl;
    logFile.flush();

    std::ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        logFile << "Error: Unable to open input file: " << inputFilename << std::endl;
        logFile.flush();
        std::cerr << "Error: Unable to open input file: " << inputFilename << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string input = buffer.str();
    inputFile.close();

    logFile << "Received input: " << input << std::endl;
    logFile.flush();

    json data;
    try {
        data = json::parse(input);
    }
    catch (const json::parse_error& e) {
        logFile << "JSON parse error: " << e.what() << std::endl;
        logFile << "Input causing the error: " << input << std::endl;
        logFile.flush();
        std::cerr << json({ {"error", "Invalid input JSON"} }).dump() << std::endl;
        return 1;
    }

    logFile << "Parsed JSON data:" << std::endl;
    logFile << data.dump(2) << std::endl;
    logFile.flush();

    std::vector<std::vector<int>> grid = data["grid"];
    json start = data["start"];
    json end = data["end"];
    std::string algorithm = data["algorithm"];

    logFile << "Grid size: " << grid.size() << "x" << grid[0].size() << std::endl;
    logFile.flush();
    logFile << "Start: (" << start["row"] << "," << start["col"] << ")" << std::endl;
    logFile.flush();
    logFile << "End: (" << end["row"] << "," << end["col"] << ")" << std::endl;
    logFile.flush();
    logFile << "Algorithm: " << algorithm << std::endl;
    logFile.flush();

    PathFinder finder(grid, start, end);
    json result;

    if (algorithm == "dijkstra") {
        result = finder.dijkstra();
    }
    else if (algorithm == "bfs") {
        result = finder.bfs();
    }
    else {
        result = { {"error", "Invalid algorithm"} };
    }

    logFile << "Final result:" << std::endl;
    logFile << result.dump(2) << std::endl;
    logFile.flush();

    std::cout << result.dump() << std::endl;
    logFile << "Pathfinder finished" << std::endl;
    logFile.flush();
    return 0;
}

