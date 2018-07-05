#include <iostream>
#include <GLFW/glfw3.h>
#include <ctime>
#include <cstdlib>

static const short int WIDTH = 800;
static const short int HEIGHT = 800;

class GameOfLife {
public:
    int cellSize = 10;
    int amountCells = WIDTH / cellSize;
    int cells[80][80];
    int generation = 0;

    void generate() {
        srand(time(0));
        for (int row = 0; row < amountCells; row++) {
            for (int col = 0; col < amountCells; col++) {
                cells[row][col] = rand() % 2;
                //std::cout << cells[row][col] << "\n";
            }
        }
    }

    int getNeighbors(int r, int c) {
        const short int rowCoords[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        const short int colCoords[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

        short int neighbors = 0;
        for (int i = 0; i < 8; i++) {
            if (r + rowCoords[i] < 0 || r + rowCoords[i] > amountCells - 1 || c + colCoords[i] < 0 || c + colCoords[i] > amountCells - 1) {
                ;
            } else if (cells[r + rowCoords[i]][c + colCoords[i]] == 1) {
                neighbors++;
            }
        }
        return neighbors;
    }

    void runRules() {
        int temp[80][80];
        for (int row = 0; row < amountCells; row++) {
            for (int col = 0; col < amountCells; col++) {
                if (cells[row][col] == 1 && getNeighbors(row, col) < 2) { // Loneliness
                    temp[row][col] = 0;
                } else if (cells[row][col] == 1 && getNeighbors(row, col) > 3) { // Overpopulation
                    temp[row][col] = 0;
                } else if (cells[row][col] == 0 && getNeighbors(row, col) == 3) { // Reproduction
                    temp[row][col] = 1;
                } else { // Stasis
                    temp[row][col] = cells[row][col];
                }
            }
        }
        generation++;
        for (int row = 0; row < amountCells; row++) {
            for (int col = 0; col < amountCells; col++) {
                cells[row][col] = temp[row][col];
            }
        }
    }

    void createRect(int x, int y, int width, int height) {
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2i(x, y);
        glVertex2i(x, y + width);
        glVertex2i(x + width, y);
        glVertex2i(x + width, y + height);
        glEnd();
    }

    void show() {
        for (int row = 0; row < amountCells; row++) {
            for (int col = 0; col < amountCells; col++) {
                if (cells[row][col] == 1) {
                    createRect(row * cellSize, col * cellSize, cellSize, cellSize);
                }
            }
        }
    }
};

int main() {
    GLFWwindow* window;
    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "Game of Life", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Change the coord system
    glOrtho(0.0f, WIDTH, HEIGHT, 0.0f, 0.0f, 1.0f);

    int frameCount = 0;

    GameOfLife ga;
    ga.generate();
    //std::cout << ga.getNeighbors(2, 2) << std::endl;

    // The main loop
    while (!glfwWindowShouldClose(window)) {
        frameCount++;
        glClear(GL_COLOR_BUFFER_BIT);
        ga.show();
        if (frameCount % 90 == 0) {
            ga.runRules();
            std::cout << "GENERATION: " << ga.generation << std::endl;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}