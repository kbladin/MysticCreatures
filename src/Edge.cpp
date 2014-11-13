#include <iostream>

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Edge.h"
#include "SimWorld.h"

Edge::Edge()
{

}

Edge::~Edge()
{

}

glm::vec2 Edge::GetForce(SimWorld* world)
{
	// Force goes from vert1_ to vert2_
	glm::vec2 position_diff = vert2_->GetPosition() - vert1_->GetPosition();
	glm::vec2 velocity_diff = vert2_->GetVelocity() - vert1_->GetVelocity();
	glm::vec2 direction = glm::normalize(position_diff);
	float current_length = glm::length(position_diff);
	// projection on direction vector
	glm::vec2 damper_force = damper_constant_ * glm::dot(velocity_diff, direction) * direction;
	// Hookes law
	glm::vec2 spring_force = spring_constant_ * (current_length - length_) * direction;

	return damper_force + spring_force;
}

void Edge::AddForceToVertices(glm::vec2 force)
{
	vert1_->AddForce(force);
	vert2_->AddForce(-force);
}

void Edge::Draw()
{
	glBegin(GL_LINE_STRIP);
		glVertex2f(vert1_->GetPosition().x, vert1_->GetPosition().y);
		glVertex2f(vert2_->GetPosition().x, vert2_->GetPosition().y);
	glEnd();
}