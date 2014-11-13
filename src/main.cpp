#include <iostream>
#include <unistd.h>

#include <GLFW/glfw3.h>

#include "Creature.h"
#include "Edge.h"
#include "SimWorld.h"

int main()
{
	GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    SimWorld world;
	Creature creature;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT);

		int microseconds = 10000;
		creature.SimulateNextStep(&world, (float) microseconds / 1000000);
		usleep(microseconds);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}