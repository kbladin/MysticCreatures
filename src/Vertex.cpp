#include "Vertex.h"
#include "glm/glm.hpp"

Vertex::Vertex()
{

}

Vertex::~Vertex()
{

}

glm::vec2 Vertex::GetNextVelocity(float dt)
{
	return velocity_ + (1/mass_) * force_ * dt;
}

glm::vec2 Vertex::GetNextPosition(float dt)
{
	return position_ + velocity_ * dt;
}

void Vertex::NormalizeNormal()
{
	if (normal_ == normal_ && normal_ != glm::vec2(0.0f,0.0f))
		normal_ = glm::normalize(normal_);
}