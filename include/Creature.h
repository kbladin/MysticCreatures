#ifndef CREATURE_H
#define CREATURE_H

#include <vector>
#include "SimWorld.h"
#include "Edge.h"
#include "Vertex.h"
#include "FunctionNetwork.h"

struct PerformanceStats
{
	PerformanceStats();
	float accum_dist;
	float dist;
};

class Creature
{
private:
	int n_edges_;
	int n_vertices_;
	std::vector<Edge> edges_[2]; // Muscles
	std::vector<Vertex> vertices_[2]; // Muscle connections
	FunctionNetwork brain_;
	SimWorld* world_;
	PerformanceStats stats_;

	bool read_buffer_; // 0 or 1
	bool write_buffer_; // 0 or 1

	void AddWorldForces();
	void AddNeuralNetworkForces();
	void UpdateState(float dt);
	void UpdateStats(float dt);
	void InstantiateSimpleCreature();
	void InstantiateWormCreature();

public:
	Creature();
	//Creature(const Creature& c);
	~Creature();

	void SetWorld(SimWorld* world);
	void ResetState();
	void ZeroTmpVertexData();
	void SimulateNextStep(float dt);
	void Mutate(float mutation_rate, float mutation_sigma);
	float GetPerformance() const;
	void SwapBuffers();
	void Draw();
	void DebugPrint();
};

#endif