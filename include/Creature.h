#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include "SimWorld.h"
#include "Edge.h"
#include "Vertex.h"
#include "FunctionNetwork.h"

class Creature
{
private:
	std::vector<Edge> edges_[2]; // Muscles
	std::vector<Vertex> vertices_[2]; // Muscle connections
	FunctionNetwork* brain;

	bool read_buffer_; // 0 or 1
	bool write_buffer_; // 0 or 1

	void InstantiateSimpleCreature();
	void AddWorldForces(SimWorld* world);
	void AddNeuralNetworkForces();
	void UpdateState(float dt);
public:
	Creature();
	~Creature();

	void SimulateNextStep(SimWorld* world, float dt);
	void SwapBuffers();
	void Draw();
	void DebugPrint();
};

#endif