#include "GLFW/glfw3.h"
#include "Calculator.hpp"

using namespace calculator;

int main(){
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(900, 900, "Ong, Lance", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    Calculator calculator = Calculator();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POLYGON);
        
        glVertex2f((*calculator.getXPointsArray())[0], (*calculator.getYPointsArray())[0]);
        glVertex2f((*calculator.getXPointsArray())[1], (*calculator.getYPointsArray())[1]);
        glVertex2f((*calculator.getXPointsArray())[2], (*calculator.getYPointsArray())[2]);
        glVertex2f((*calculator.getXPointsArray())[3], (*calculator.getYPointsArray())[3]);
        glVertex2f((*calculator.getXPointsArray())[4], (*calculator.getYPointsArray())[4]);

        glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}