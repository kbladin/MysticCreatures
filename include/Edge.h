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

	glm::vec2 GetForce(SimWorld* world);
	void AddForceToVertices(glm::vec2 force);

};

#endif
