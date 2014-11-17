#include <iostream>

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Edge.h"
#include "SimWorld.h"

Edge::Edge()
{
	on_edge_ = false;
}

Edge::~Edge()
{

}

glm::vec2 Edge::GetSpringForce(SimWorld* world)
{
	// Force goes from vert1_ to vert2_
	glm::vec2 position_diff = vert2_->GetPosition() - vert1_->GetPosition();
	glm::vec2 velocity_diff = vert2_->GetVelocity() - vert1_->GetVelocity();
	glm::vec2 direction = GetDirection();
	float current_length = glm::length(position_diff);
	// projection on direction vector
	glm::vec2 damper_force = damper_constant_ * glm::dot(velocity_diff, direction) * direction;
	// Hookes law
	glm::vec2 spring_force = spring_constant_ * (current_length - length_) * direction;

	return damper_force + spring_force;
}

glm::vec2 Edge::GetFluidResistanceForceVert1(SimWorld* world)
{
	glm::vec2 dir = GetDirection();
	glm::vec2 force_dir(dir.y, -dir.x);
	glm::vec2 total_flow = world->GetCurrent(vert1_->GetPosition()) - vert1_->GetVelocity();
	return vert1_->GetFluidFrictionConstant() * glm::dot(total_flow, force_dir) * force_dir;
}

glm::vec2 Edge::GetFluidResistanceForceVert2(SimWorld* world)
{
	glm::vec2 dir = GetDirection();
	glm::vec2 force_dir(dir.y, -dir.x);
	glm::vec2 total_flow = world->GetCurrent(vert2_->GetPosition()) - vert2_->GetVelocity();
	return vert2_->GetFluidFrictionConstant() * glm::dot(total_flow, force_dir) * force_dir;
}

glm::vec2 Edge::GetDirection()
{
	glm::vec2 position_diff = vert2_->GetPosition() - vert1_->GetPosition();
	if (position_diff == glm::vec2(0.0f,0.0f))
		return glm::vec2(0.0f,0.0f);
	else
		return glm::normalize(position_diff);
}

float Edge::GetCurrentLength()
{
	return glm::length(vert2_->GetPosition() - vert1_->GetPosition());
}

void Edge::AddForceToVertices(glm::vec2 force)
{
	vert1_->AddForce(force);
	vert2_->AddForce(-force);
}

void Edge::AddForceToVert1(glm::vec2 force)
{
	vert1_->AddForce(force);
}

void Edge::AddForceToVert2(glm::vec2 force)
{
	vert2_->AddForce(force);
}

void Edge::AddNormalToVertices(glm::vec2 n)
{
	vert1_->AddNormal(n);
	vert2_->AddNormal(-n);
}

void Edge::Draw()
{
	if (on_edge_)
	{
		glColor3f(0.0f,1.0f,1.0f);
		glBegin(GL_LINE_STRIP);
			glVertex2f(vert1_->GetPosition().x, vert1_->GetPosition().y);
			glVertex2f(vert2_->GetPosition().x, vert2_->GetPosition().y);
		glEnd();
	}
	else
		glColor3f(1.0f,1.0f,1.0f);

}