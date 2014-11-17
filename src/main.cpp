#include <iostream>
#include <algorithm>
#include <unistd.h>

#include <GLFW/glfw3.h>

#include "Creature.h"
#include "Edge.h"
#include "SimWorld.h"

SimWorld* world;
static std::default_random_engine generator;

void MouseButtonFun(GLFWwindow * window, int button, int action, int mods){
    if (action == GLFW_PRESS)
    {
        double x_pos, y_pos;
        int x_size, y_size;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        glfwGetWindowSize(window, &x_size, &y_size);

        glm::vec2 position(((x_pos / x_size) - 0.5)*2, -((y_pos / y_size) - 0.5) * 2);

        world->SetTargetPosition(position);

        //std::cout << "position = (" << position.x << " , " << position.y << ")" << std::endl;
    }
}

int main()
{
	const int population_size = 100;
	const int n_generations = 100;
    const float elitism = 0.3;
    const float mutation_rate = 0.2;
    const float mutation_sigma = 0.5;
	const float simulation_time = 10.0f;
	const float simulation_step = 0.01f;
    bool draw_simulation = false;

	GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, *MouseButtonFun);

    world = new SimWorld();
    Creature* population[population_size];

    for (int i = 0; i < population_size; ++i)
    {
        population[i] = new Creature();
        population[i]->SetWorld(world);//) = &population[i];
    }

    for (int i = 0; i < n_generations; ++i)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator = std::default_random_engine(seed);

        std::uniform_real_distribution<float> position_distribution(-1.0f, 1.0f);
        glm::vec2 position(position_distribution(generator), position_distribution(generator));
        world->SetTargetPosition(position);
        //std::cout << "position = (" << position.x << " , " << position.y << ")" << std::endl;

        std::cout << "Generation " << i << " simulating. " << std::endl;
        for (int j = 0; j < population_size; ++j)
        {
            //std::cout << "  Simulating creature " << j << std::endl;
            population[j]->ResetState();
            for (float t = 0; t < simulation_time; t = t+simulation_step)
            {
                population[j]->SimulateNextStep(simulation_step);

                if (draw_simulation)
                {
                    glClear(GL_COLOR_BUFFER_BIT);

                    world->Draw();
                    population[j]->Draw();

                    glfwSwapBuffers(window);

                    glfwPollEvents();
                }
            }
        }
        std::cout << "Done!" << std::endl;
        
/*
        for (int j = 0; j < population_size; ++j)
        {
            std::cout << "  Performance of creature " << j << ": " <<
            population[j]->GetPerformance() << std::endl;
        }
        */
        //std::cout << "Sorting " << std::endl;
        std::sort(std::begin(population), std::end(population), [](Creature* a, Creature* b) { return a->GetPerformance() < b->GetPerformance(); });
/*
        for (int j = 0; j < population_size; ++j)
        {
            std::cout << "  Performance of creature " << j << ": " <<
            population[j]->GetPerformance() << std::endl;
        }
*/

/*
        for (int j = 0; j < population_size; ++j)
        {
            population[j]->Mutate(mutation_rate, mutation_sigma);
        }
*/
        std::cout << "Best creatures performance = " << population[0]->GetPerformance() << std::endl;

        std::uniform_int_distribution<int> index_distribution(0, elitism*population_size);

        for (int j = elitism*population_size + 1; j < population_size ; ++j)
        {
            //int index = index_distribution(generator);
            //std::cout << "index = " << index << std::endl;

            int from_index = index_distribution(generator);
            //std::cout << "Taking creature " << from_index << " to index " << j << std::endl;

            *population[j] = *population[from_index];
            population[j]->Mutate(mutation_rate, mutation_sigma);
        }
    }


    std::cout << "Simulation finished!" << std::endl;
    std::cout << "Now simulating the best creature" << std::endl;


    Creature creature = *population[0];
    //world->SetTargetPosition(glm::vec2(0.5f, -0.5f));
    creature.ResetState();

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT);

		creature.SimulateNextStep(simulation_step);

		world->Draw();
		creature.Draw();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    for (int i = 0; i < population_size; ++i)
    {
        delete population[i];
    }

	return 0;
}