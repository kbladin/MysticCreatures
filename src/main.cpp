#include <iostream>
#include "Creature.h"
#include "Edge.h"
#include "SimWorld.h"

int main()
{
	SimWorld world;
	Creature creature;

	do
	{
		creature.SimulateNextStep(&world, 0.001);
	}
	while(true);

	return 0;
}