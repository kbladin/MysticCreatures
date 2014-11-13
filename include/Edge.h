#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"
#include "SimWorld.h"
#include "glm/glm.hpp"

class Edge
{
private:
	float spring_constant_;
	float damper_constant_;
	float length_;
	Vertex* vert1_;
	Vertex* vert2_;
public:
	Edge();
	~Edge();

	void SetSpringConstant(float k) {spring_constant_ = k;}
	void SetDamperConstant(float d) {damper_constant_ = d;}
	void SetLength(float l) {length_ = l;}
	void SetVertices(Vertex* v1, Vertex* v2)
		{vert1_ = v1; vert2_ = v2;}

	float GetLength(){ return length_;}
	float GetCurrentLength();
	glm::vec2 GetSpringForce(SimWorld* world);
	glm::vec2 GetFluidResistanceForceVert1(SimWorld* world);
	glm::vec2 GetFluidResistanceForceVert2(SimWorld* world);
	glm::vec2 GetDirection();
	void AddForceToVertices(glm::vec2 force);
	void AddForceToVert1(glm::vec2 force);
	void AddForceToVert2(glm::vec2 force);
	void AddNormalToVertices(glm::vec2 n);

	void Draw();

};

#endif
