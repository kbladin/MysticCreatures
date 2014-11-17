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
	float base_length_;
	float stretch_;
	Vertex* vert1_;
	Vertex* vert2_;

	bool on_edge_;
public:
	Edge();
	~Edge();

	void SetSpringConstant(float k) {spring_constant_ = k;}
	void SetDamperConstant(float d) {damper_constant_ = d;}
	void SetBaseLength(float l) {base_length_ = l; length_ = l;}
	void SetStretch(float s) {stretch_ = s;}
	void SetLength(float l) {length_ = l;}
	void SetVertices(Vertex* v1, Vertex* v2)
		{vert1_ = v1; vert2_ = v2;}
	void SetOnEdge(bool on_edge) {on_edge_ = on_edge;}

	float GetLength() {return length_;}
	float GetBaseLength() {return base_length_;}
	float GetStretch() {return stretch_;}
	float GetOnEdge() {return on_edge_;}
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
