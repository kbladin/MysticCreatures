#include <GLFW/glfw3.h>

#include "../include/Creature.h"
#include "Edge.h"
#include "Vertex.h"
#include "glm/glm.hpp"
#include "FunctionNetwork.h"

PerformanceStats::PerformanceStats()
{
	accum_dist = 0;
	dist = 0;
}

Creature::Creature()
{
	read_buffer_ = 0;
	write_buffer_ = 1;

	InstantiateWormCreature();
	brain_ = FunctionNetwork(
	n_vertices_ + n_edges_,
	10,
	n_edges_);
}
/*
Creature::Creature(const Creature& c)
{
	brain_ = c.brain_;
	world_ = c.world_;
}
*/
Creature::~Creature()
{
	//delete brain_;
}

void Creature::ResetState()
{
	stats_.dist = 0;
	stats_.accum_dist = 0;
	InstantiateWormCreature();
	for (int i = 0; i < n_vertices_; ++i)
	{
		vertices_[read_buffer_][i].ZeroForce();
		vertices_[read_buffer_][i].ZeroNormal();
		vertices_[read_buffer_][i].ZeroVelocity();
	}
}

void Creature::SetWorld(SimWorld* world)
{
	world_ = world;
}

void Creature::InstantiateSimpleCreature()
{
	for (int buffer_index = 0; buffer_index < 2; ++buffer_index)
	{
		n_edges_ = 3;
		n_vertices_ = 3;
		edges_[buffer_index].resize(n_edges_);
		vertices_[buffer_index].resize(n_vertices_);

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

		edges_[buffer_index][0].SetOnEdge(true);
		edges_[buffer_index][1].SetOnEdge(true);
		edges_[buffer_index][2].SetOnEdge(true);


		for (int i = 0; i < edges_[buffer_index].size(); ++i)
		{
			edges_[buffer_index][i].SetSpringConstant(100.0f);
			edges_[buffer_index][i].SetDamperConstant(0.0f);
			edges_[buffer_index][i].SetBaseLength(0.3f);
			edges_[buffer_index][i].SetStretch(0.02f);
		}
	}
}

void Creature::InstantiateWormCreature()
{
	for (int buffer_index = 0; buffer_index < 2; ++buffer_index)
	{
		n_edges_ = 16;
		n_vertices_ = 8;
		edges_[buffer_index].resize(n_edges_);
		vertices_[buffer_index].resize(n_vertices_);

		float scale = 0.1;

		vertices_[buffer_index][0].SetPosition(glm::vec2(0.0f, 0.0f) * scale);
		vertices_[buffer_index][1].SetPosition(glm::vec2(1.0f, 0.0f) * scale);
		vertices_[buffer_index][2].SetPosition(glm::vec2(2.0f, 0.0f) * scale);
		vertices_[buffer_index][3].SetPosition(glm::vec2(3.0f, 0.0f) * scale);
		vertices_[buffer_index][4].SetPosition(glm::vec2(0.0f, -1.0f) * scale);
		vertices_[buffer_index][5].SetPosition(glm::vec2(1.0f, -1.0f) * scale);
		vertices_[buffer_index][6].SetPosition(glm::vec2(2.0f, -1.0f) * scale);
		vertices_[buffer_index][7].SetPosition(glm::vec2(3.0f, -1.0f) * scale);

		for (int i = 0; i < vertices_[buffer_index].size(); ++i)
		{
			vertices_[buffer_index][i].SetMass(0.1f);
			vertices_[buffer_index][i].SetFluidFrictionConstant(0.3f);
		}

		edges_[buffer_index][0].SetVertices(&vertices_[buffer_index][0], &vertices_[buffer_index][1]);
		edges_[buffer_index][1].SetVertices(&vertices_[buffer_index][1], &vertices_[buffer_index][2]);
		edges_[buffer_index][2].SetVertices(&vertices_[buffer_index][2], &vertices_[buffer_index][3]);
		edges_[buffer_index][3].SetVertices(&vertices_[buffer_index][4], &vertices_[buffer_index][5]);
		edges_[buffer_index][4].SetVertices(&vertices_[buffer_index][5], &vertices_[buffer_index][6]);
		edges_[buffer_index][5].SetVertices(&vertices_[buffer_index][6], &vertices_[buffer_index][7]);
		edges_[buffer_index][6].SetVertices(&vertices_[buffer_index][0], &vertices_[buffer_index][4]);
		edges_[buffer_index][7].SetVertices(&vertices_[buffer_index][1], &vertices_[buffer_index][5]);
		edges_[buffer_index][8].SetVertices(&vertices_[buffer_index][2], &vertices_[buffer_index][6]);
		edges_[buffer_index][9].SetVertices(&vertices_[buffer_index][3], &vertices_[buffer_index][7]);
		edges_[buffer_index][10].SetVertices(&vertices_[buffer_index][1], &vertices_[buffer_index][4]);
		edges_[buffer_index][11].SetVertices(&vertices_[buffer_index][2], &vertices_[buffer_index][5]);
		edges_[buffer_index][12].SetVertices(&vertices_[buffer_index][3], &vertices_[buffer_index][6]);
		edges_[buffer_index][13].SetVertices(&vertices_[buffer_index][0], &vertices_[buffer_index][5]);
		edges_[buffer_index][14].SetVertices(&vertices_[buffer_index][1], &vertices_[buffer_index][6]);
		edges_[buffer_index][15].SetVertices(&vertices_[buffer_index][2], &vertices_[buffer_index][7]);

		edges_[buffer_index][0].SetBaseLength(1 * scale);
		edges_[buffer_index][1].SetBaseLength(1 * scale);
		edges_[buffer_index][2].SetBaseLength(1 * scale);
		edges_[buffer_index][3].SetBaseLength(1 * scale);
		edges_[buffer_index][4].SetBaseLength(1 * scale);
		edges_[buffer_index][5].SetBaseLength(1 * scale);
		edges_[buffer_index][6].SetBaseLength(1 * scale);
		edges_[buffer_index][7].SetBaseLength(1 * scale);
		edges_[buffer_index][8].SetBaseLength(1 * scale);
		edges_[buffer_index][9].SetBaseLength(1 * scale);
		edges_[buffer_index][10].SetBaseLength(sqrt(2) * scale);
		edges_[buffer_index][11].SetBaseLength(sqrt(2) * scale);
		edges_[buffer_index][12].SetBaseLength(sqrt(2) * scale);
		edges_[buffer_index][13].SetBaseLength(sqrt(2) * scale);
		edges_[buffer_index][14].SetBaseLength(sqrt(2) * scale);
		edges_[buffer_index][15].SetBaseLength(sqrt(2) * scale);

		edges_[buffer_index][0].SetOnEdge(true);
		edges_[buffer_index][1].SetOnEdge(true);
		edges_[buffer_index][2].SetOnEdge(true);
		edges_[buffer_index][3].SetOnEdge(true);
		edges_[buffer_index][4].SetOnEdge(true);
		edges_[buffer_index][5].SetOnEdge(true);
		edges_[buffer_index][6].SetOnEdge(true);
		edges_[buffer_index][9].SetOnEdge(true);

		for (int i = 0; i < edges_[buffer_index].size(); ++i)
		{
			edges_[buffer_index][i].SetSpringConstant(50.0f);
			edges_[buffer_index][i].SetDamperConstant(0.1f);
			//edges_[buffer_index][i].SetBaseLength(0.3f);
			edges_[buffer_index][i].SetStretch(0.015f);
		}
	}
}

void Creature::SimulateNextStep(float dt)
{
	ZeroTmpVertexData();
	// Forces from the brain
	AddNeuralNetworkForces();
		// Forces from the world
	AddWorldForces();

	UpdateState(dt);

	UpdateStats(dt);

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
	for (int i = 0; i < n_edges_; ++i)
	{
		edges_[read_buffer_][i].Draw();
	}
	for (int i = 0; i < n_vertices_; ++i)
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
	for (int i = 0; i < n_vertices_; ++i)
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
	for (int i = 0; i < n_vertices_; ++i)
	{
		vertices_[read_buffer_][i].ZeroForce();
		vertices_[read_buffer_][i].ZeroNormal();
	}
}

void Creature::Mutate(float mutation_rate, float mutation_sigma)
{
	brain_.Mutate(mutation_rate, mutation_sigma);
}

float Creature::GetPerformance() const
{
	return stats_.accum_dist;
}

void Creature::AddWorldForces()
{
	// Add forces to all vertices
	for (int i=0; i < n_edges_; ++i)
	{
		glm::vec2 f = edges_[read_buffer_][i].GetSpringForce(world_);
		edges_[read_buffer_][i].AddForceToVertices(f);

		if (edges_[read_buffer_][i].GetOnEdge())
		{
			glm::vec2 fluid_force1 = edges_[read_buffer_][i].GetFluidResistanceForceVert1(world_);
			glm::vec2 fluid_force2 = edges_[read_buffer_][i].GetFluidResistanceForceVert2(world_);
			edges_[read_buffer_][i].AddForceToVert1(fluid_force1);
			edges_[read_buffer_][i].AddForceToVert2(fluid_force2);
		}
	}
}

void Creature::AddNeuralNetworkForces() // Should be called update brain instead
{
	for (int i = 0; i < n_edges_; ++i)
	{
		glm::vec2 n = edges_[read_buffer_][i].GetDirection();
		edges_[read_buffer_][i].AddNormalToVertices(-n);
	}

	for (int i = 0; i < n_vertices_; ++i)
	{
		vertices_[read_buffer_][i].NormalizeNormal();
	}

	// Inputs: first direction differences from notmals to target,
	// then spring lengths
	//std::cout << "n_vertices_ = " << n_vertices_ << std::endl;
	//std::cout << "n_edges_ = " << n_edges_ << std::endl;
	std::vector<float> brain_input(
		n_vertices_ +
		n_edges_);
	for (int i = 0; i < n_vertices_; ++i)
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
	for (int i = 0; i < n_edges_; ++i)
	{
		float current_length = edges_[read_buffer_][i].GetCurrentLength();
		brain_input[n_vertices_ + i] =
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

	std::vector<float> brain_output = brain_.CalculateOutput(brain_input);

/*
	std::cout << "Brain output" << std::endl;
	for (int i = 0; i < brain_output.size(); ++i)
	{
		std::cout << i << " : " << brain_output[i] << std::endl;
	}
	std::cout << std::endl;
*/

	// Update spring length according to the brain
	for (int i = 0; i < n_edges_; ++i)
	{
		edges_[read_buffer_][i].SetLength(edges_[read_buffer_][i].GetBaseLength() + ((brain_output[i] - 0.5f)*2) * edges_[read_buffer_][i].GetStretch());
	}
}

void Creature::UpdateState(float dt)
{
	for (int i = 0; i < n_vertices_; ++i)
	{
		vertices_[read_buffer_][i].SetVelocity(vertices_[read_buffer_][i].GetNextVelocity(dt));
		vertices_[read_buffer_][i].SetPosition(vertices_[read_buffer_][i].GetNextPosition(dt));
	}
}

void Creature::UpdateStats(float dt)
{
	glm::vec2 position_diff(0.0f, 0.0f);
	for (int i = 0; i < n_vertices_; ++i)
	{
		position_diff += world_->GetTargetPosition() - vertices_[read_buffer_][i].GetPosition();
	}
	stats_.dist = glm::length(position_diff * (1.0f / n_vertices_));
	stats_.accum_dist += stats_.dist * dt;
}