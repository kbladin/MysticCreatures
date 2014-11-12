#ifndef VERTEX_H
#define VERTEX_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Vertex
{
private:
	glm::vec2 position_;
	glm::vec2 velocity_;
	glm::vec2 force_;
	glm::vec2 normal_;
	float mass_;
	float fluid_friction_constant_;

public:
	Vertex();
	~Vertex();

	glm::vec2 GetPosition() { return position_; }
	glm::vec2 GetVelocity() { return velocity_; }
	glm::vec2 GetForce() { return force_; }
	float GetMass() { return mass_; }
	float GerFluidFrictionConstant() { return fluid_friction_constant_; }

	void SetPosition(glm::vec2 position) { position_ = position; }
	void SetVelocity(glm::vec2 velocity) { velocity_ = velocity; }
	void SetMass(float mass) { mass_ = mass; }
	void SetFluidFrictionConstant(float fluid_friction_constant)
		{ fluid_friction_constant_ = fluid_friction_constant; }

	void ZeroForce() {force_ = glm::vec2(0.0f, 0.0f);}
	void AddForce(glm::vec2 force) {force_ += force;}
	glm::vec2  GetNextVelocity(float dt);
	glm::vec2  GetNextPosition(float dt);
};

#endif