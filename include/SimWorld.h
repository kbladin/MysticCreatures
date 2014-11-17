#ifndef SIMWORLD_H
#define SIMWORLD_H

#include "glm/glm.hpp"

class SimWorld
{
private:
	glm::vec2 current_;
	glm::vec2 target_position_;
public:
	SimWorld();
	~SimWorld();

	glm::vec2 GetCurrent(glm::vec2 position);
	glm::vec2 GetTargetPosition();
	void SetTargetPosition(glm::vec2 pos);
	void Draw();
};

#endif