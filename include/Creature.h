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
	FunctionNetwork* brain_;
	SimWorld* world_;

	bool read_buffer_; // 0 or 1
	bool write_buffer_; // 0 or 1

	void AddWorldForces();
	void AddNeuralNetworkForces();
	void UpdateState(float dt);
public:
	Creature();
	~Creature();

	void SetWorld(SimWorld* world);
	void InstantiateSimpleCreature();
	void ZeroTmpVertexData();
	void SimulateNextStep(float dt);
	void Mutate(float mutation_rate);
	float GetPerformance() const;
	void SwapBuffers();
	void Draw();
	void DebugPrint();
	bool operator<(const Creature c) const;
	bool operator>(const Creature c) const;
};

#endif