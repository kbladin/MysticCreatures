#include <iostream>
#include <algorithm>
#include <unistd.h>

#include <GLFW/glfw3.h>

#include "Creature.h"
#include "Edge.h"
#include "SimWorld.h"

int main()
{
	const int population_size = 50;
	const int n_generations = 1;
	const float simulation_time = 10.0f;
	const float simulation_step = 0.01f;

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
    Creature population[population_size];

    for (int i = 0; i < population_size; ++i)
    {
    	population[i].SetWorld(&world);
    }

    for (int i = 0; i < n_generations; ++i)
    {
    	std::cout << "Generation " << i << " simulating. " << std::endl;
    	for (int j = 0; j < population_size; ++j)
    	{
    		for (float t = 0; t < simulation_time; t = t+simulation_step)
	    	{
	    		population[j].SimulateNextStep(simulation_step);
	    	}
    	}
    	std::sort(std::begin(population), std::end(population));

		for (int j = 0; j < population_size; ++j)
    	{
    		std::cout << "  Performance of creature " << j << ": " <<
	    	population[j].GetPerformance() << std::endl;
    	}
    	std::cout << "Done!" << std::endl;
    }

	Creature creature;
	creature.SetWorld(&world);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT);

		creature.SimulateNextStep(simulation_step);

		world.Draw();
		creature.Draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

	return 0;
}