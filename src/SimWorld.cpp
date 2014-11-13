#include "SimWorld.h"
#include <GLFW/glfw3.h>

SimWorld::SimWorld()
{
	current_ = glm::vec2(0.0f, 0.0f);
	target_position_ = glm::vec2(-0.5f, -0.5f);
}

SimWorld::~SimWorld()
{

}

glm::vec2 SimWorld::GetCurrent(glm::vec2 position)
{
	return current_;
}

glm::vec2 SimWorld::GetTargetPosition()
{
	return target_position_;
}

void SimWorld::Draw()
{
	glPointSize(10);
	glBegin(GL_POINTS);
		glVertex2f(target_position_.x, target_position_.y);
	glEnd();
}