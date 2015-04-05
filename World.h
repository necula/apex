#ifndef WORLD_H_
#define WORLD_H_

#include "Plane.h"
#include "BmpToTexture.h"
#include "Vector3D.h"
#include "Map.h"

class World {

private:
	Plane *ground;
	GLuint skybox[6]; // Skybox textures
	GLuint skyboxList;
	Vector3D *skyboxPosition;
	Map *map;

	Vector4D diffuse;
	Vector4D ambient;
	Vector4D specular;

	void initSkyboxList();

public:
	World();
	virtual ~World();
	void draw();
	void setSkyboxPosition(Vector3D *position);
	bool checkCollision(Car *car);
	void renderLight();
};

#endif /* WORLD_H_ */
