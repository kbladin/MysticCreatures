#include <GLFW/glfw3.h>

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
	//delete brain_;
}

void Creature::SetWorld(SimWorld* world)
{
	world_ = world;
}

void Creature::InstantiateSimpleCreature()
{
	for (int buffer_index = 0; buffer_index < 2; ++buffer_index)
	{
		edges_[buffer_index].resize(3);
		vertices_[buffer_index].resize(3);

		vertices_[buffer_index][0].SetPosition(glm::vec2(0.1f, 0.1f));
		vertices_[buffer_index][1].SetPosition(glm::vec2(-0.1f, -0.1f));
		vertices_[buffer_index][2].SetPosition(glm::vec2(0.1f, -0.1f));

		for (int i = 0; i < vertices_[buffer_index].size(); ++i)
		{
			vertices_[buffer_index][i].SetMass(0.1f);
			vertices_[buffer_index][i].SetFluidFrictionConstant(1.0f);
		}

		edges_[buffer_index][0].SetVertices(&vertices_[buffer_index][0], &vertices_[buffer_index][1]);
		edges_[buffer_index][1].SetVertices(&vertices_[buffer_index][1], &vertices_[buffer_index][2]);
		edges_[buffer_index][2].SetVertices(&vertices_[buffer_index][2], &vertices_[buffer_index][0]);

		for (int i = 0; i < edges_[buffer_index].size(); ++i)
		{
			edges_[buffer_index][i].SetSpringConstant(50.0f);
			edges_[buffer_index][i].SetDamperConstant(0.0f);
			edges_[buffer_index][i].SetLength(0.2f);
		}
	}
	brain_ = new FunctionNetwork(
		vertices_[read_buffer_].size() + edges_[read_buffer_].size(),
		10,
		edges_[read_buffer_].size());
}

void Creature::SimulateNextStep(float dt)
{
	ZeroTmpVertexData();
	// Forces from the brain
	AddNeuralNetworkForces();
		// Forces from the world
	AddWorldForces();


	UpdateState(dt);

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
	for (int i = 0; i < vertices_[read_buffer_].size(); ++i)
	{
		glm::vec2 start_point = vertices_[read_buffer_][i].GetPosition();
		glm::vec2 end_point = vertices_[read_buffer_][i].GetPosition() + vertices_[read_buffer_][i].GetNormal();
		/*
		glBegin(GL_LINE_STRIP);
			glVertex2f(start_point.x, start_point.y);
			glVertex2f(end_point.x, end_point.y);
		glEnd();
		*/
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

void Creature::ZeroTmpVertexData()
{
	for (int i = 0; i < vertices_[read_buffer_].size(); ++i)
	{
		vertices_[read_buffer_][i].ZeroForce();
		vertices_[read_buffer_][i].ZeroNormal();
	}
}

void Creature::Mutate(float mutation_rate)
{
	brain_->Mutate(mutation_rate);
}

float Creature::GetPerformance() const
{
	// Performance is mean distance from edges to target
	glm::vec2 position_diff(0.0f, 0.0f);
	int n_vertices = vertices_[read_buffer_].size();
	for (int i = 0; i < n_vertices; ++i)
	{
		position_diff += world_->GetTargetPosition() - vertices_[read_buffer_][i].GetPosition();
	}
	return glm::length(position_diff * (1.0f / n_vertices));
}

void Creature::AddWorldForces()
{
	// Add forces to all vertices
	for (int i=0; i < edges_[read_buffer_].size(); ++i)
	{
		glm::vec2 f = edges_[read_buffer_][i].GetSpringForce(world_);
		glm::vec2 fluid_force1 = edges_[read_buffer_][i].GetFluidResistanceForceVert1(world_);
		glm::vec2 fluid_force2 = edges_[read_buffer_][i].GetFluidResistanceForceVert2(world_);

		edges_[read_buffer_][i].AddForceToVertices(f);
		edges_[read_buffer_][i].AddForceToVert1(fluid_force1);
		edges_[read_buffer_][i].AddForceToVert2(fluid_force2);
	}
}

void Creature::AddNeuralNetworkForces() // Should be called update brain instead
{
	int n_edges = edges_[read_buffer_].size();
	int n_vertices = vertices_[read_buffer_].size();

	for (int i = 0; i < n_edges; ++i)
	{
		glm::vec2 n = edges_[read_buffer_][i].GetDirection();
		edges_[read_buffer_][i].AddNormalToVertices(-n);
	}

	for (int i = 0; i < n_vertices; ++i)
	{
		vertices_[read_buffer_][i].NormalizeNormal();
	}

	// Inputs: first direction differences from notmals to target,
	// then spring lengths
	std::vector<float> brain_input(
		vertices_[read_buffer_].size() +
		edges_[read_buffer_].size());
	for (int i = 0; i < n_vertices; ++i)
	{
		glm::vec2 n = vertices_[read_buffer_][i].GetNormal();
		glm::vec2 target_diff = world_->GetTargetPosition() - vertices_[read_buffer_][i].GetPosition();
		glm::vec2 target_dir;
		if (target_diff == glm::vec2(0.0f, 0.0f))
			target_dir = glm::vec2(0.0f, 0.0f);
		else
			target_dir = glm::normalize(target_diff);
		float direction_diff = glm::dot(target_dir, n);

		glm::vec2 start_point = vertices_[read_buffer_][i].GetPosition();
		glm::vec2 end_point = vertices_[read_buffer_][i].GetPosition() + target_dir;
		/*
		glBegin(GL_LINE_STRIP);
			glVertex2f(start_point.x, start_point.y);
			glVertex2f(end_point.x, end_point.y);
		glEnd();
		*/

		brain_input[i] = direction_diff;
	}

	// Should be mean = 0
	for (int i = 0; i < n_edges; ++i)
	{
		float current_length = edges_[read_buffer_][i].GetCurrentLength();
		brain_input[n_vertices + i] =
			(edges_[read_buffer_][i].GetCurrentLength() -
			edges_[read_buffer_][i].GetLength()) * 10/current_length;
	}

	brain_input.push_back(-1.0f); // threshold input

/*
	std::cout << "Brain input" << std::endl;
	for (int i = 0; i < brain_input.size(); ++i)
	{
		std::cout << i << " : " << brain_input[i] << std::endl;
	}
	std::cout << std::endl;
*/

	std::vector<float> brain_output = brain_->CalculateOutput(brain_input);

/*
	std::cout << "Brain output" << std::endl;
	for (int i = 0; i < brain_output.size(); ++i)
	{
		std::cout << i << " : " << brain_output[i] << std::endl;
	}
	std::cout << std::endl;
*/

	// Update spring length according to the brain
	for (int i = 0; i < n_edges; ++i)
	{
		edges_[read_buffer_][i].SetLength(0.2f + ((brain_output[i] - 0.5f)*2) * 0.1f);
	}
}

void Creature::UpdateState(float dt)
{
	for (int i = 0; i < vertices_[read_buffer_].size(); ++i)
	{
		vertices_[read_buffer_][i].SetVelocity(vertices_[read_buffer_][i].GetNextVelocity(dt));
		vertices_[read_buffer_][i].SetPosition(vertices_[read_buffer_][i].GetNextPosition(dt));
	}
}

bool Creature::operator<(const Creature c) const
{
	return GetPerformance() < c.GetPerformance();
}

bool Creature::operator>(const Creature c) const
{
	return GetPerformance() > c.GetPerformance();
}
