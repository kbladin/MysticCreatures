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
