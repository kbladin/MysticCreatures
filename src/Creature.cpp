#include "../include/Creature.h"
#include "Edge.h"
#include "Vertex.h"
#include "glm/glm.hpp"
#include "FunctionNetwork.h"

Creature::Creature()
{
	read_buffer_ = 0;
	write_buffer_ = 1;

	InstantiateSimpleCreature();
}

Creature::~Creature()
{
	delete brain;
}

void Creature::InstantiateSimpleCreature()
{
	for (int buffer_index = 0; buffer_index < 2; ++buffer_index)
	{
		edges_[buffer_index].resize(3);
		vertices_[buffer_index].resize(3);

		vertices_[buffer_index][0].SetPosition(glm::vec2(0.0f, 0.1f));
		vertices_[buffer_index][1].SetPosition(glm::vec2(-0.1f, -0.1f));
		vertices_[buffer_index][2].SetPosition(glm::vec2(0.1f, -0.1f));

		for (int i = 0; i < vertices_[buffer_index].size(); ++i)
		{
			vertices_[buffer_index][i].SetMass(0.1f);
			vertices_[buffer_index][i].SetFluidFrictionConstant(0.1f);
		}

		edges_[buffer_index][0].SetVertices(&vertices_[buffer_index][0], &vertices_[buffer_index][1]);
		edges_[buffer_index][1].SetVertices(&vertices_[buffer_index][1], &vertices_[buffer_index][2]);
		edges_[buffer_index][2].SetVertices(&vertices_[buffer_index][2], &vertices_[buffer_index][0]);

		for (int i = 0; i < edges_[buffer_index].size(); ++i)
		{
			edges_[buffer_index][i].SetSpringConstant(1.0f);
			edges_[buffer_index][i].SetDamperConstant(0.1f);
			edges_[buffer_index][i].SetLength(0.2f);
		}
	}
	brain = new FunctionNetwork(1,1,1);
}

void Creature::SimulateNextStep(SimWorld* world, float dt)
{
	// Forces from the world
	AddWorldForces(world);
	// Forces from the brain
	AddNeuralNetworkForces();

	UpdateState(dt);

	Draw();

	//DebugPrint();

	//SwapBuffers();
}

void Creature::SwapBuffers()
{
	read_buffer_ = !read_buffer_;
	write_buffer_ = !write_buffer_;
}

void Creature::Draw()
{
	for (int i = 0; i < edges_[read_buffer_].size(); ++i)
	{
		edges_[read_buffer_][i].Draw();
	}
}

void Creature::DebugPrint()
{
	for (int i = 0; i < vertices_[read_buffer_].size(); ++i)
	{
		float x = vertices_[read_buffer_][i].GetPosition().x;
		float y = vertices_[read_buffer_][i].GetPosition().y;

				//glm::vec2 f = vertices_[read_buffer_][i].GetPosition();
		//std::cout << "Force " << i << ": (" << f.x << " , " << f.y << ")" << std::endl;

		std::cout << "Vertex " << i << ": (" << x << " , " << y << ")" << std::endl;
	}
	std::cout << std::endl;
}


void Creature::AddWorldForces(SimWorld* world)
{
	// Add forces to all vertices
	for (int i=0; i < edges_[read_buffer_].size(); ++i)
	{
		glm::vec2 f = edges_[read_buffer_][i].GetForce(world);

		edges_[read_buffer_][i].AddForceToVertices(f);
	}
}

void Creature::AddNeuralNetworkForces()
{

}

void Creature::UpdateState(float dt)
{
	for (int i = 0; i < vertices_[read_buffer_].size(); ++i)
	{

		vertices_[read_buffer_][i].SetVelocity(vertices_[read_buffer_][i].GetNextVelocity(dt));
		vertices_[read_buffer_][i].SetPosition(vertices_[read_buffer_][i].GetNextPosition(dt));

		vertices_[read_buffer_][i].ZeroForce();
	}
}
