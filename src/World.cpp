// "World" class handles skybox, level and groundfloor rendering

#include "World.h"

World::World() {
	ground = new Plane(25.0f);
	ground->SetColor(new Vector3D(0.5, 0.5, 0.5));

	map = new Map();

	LoadBmpTexture("data\\skybox\\left.bmp", &skybox[0]);
	LoadBmpTexture("data\\skybox\\front.bmp", &skybox[1]);
	LoadBmpTexture("data\\skybox\\right.bmp", &skybox[2]);
	LoadBmpTexture("data\\skybox\\back.bmp", &skybox[3]);
	LoadBmpTexture("data\\skybox\\down.bmp", &skybox[4]);
	LoadBmpTexture("data\\skybox\\up.bmp", &skybox[5]);

	initSkyboxList();
}

// Let's free up memory
World::~World() {
	delete map;
	delete ground;
	delete [] &skybox;
}

void World::draw() {

	// Render skybox.
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0,1.0f);
	glTranslatef(skyboxPosition->x, skyboxPosition->y, skyboxPosition->z);
	glScalef(20, 20, 20);
	glCallList(skyboxList);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	ground->Draw();
	map->Draw();

	// Start/finish box
	glPushMatrix();
	glTranslatef(0, 0.5, 7.5);
	glScalef(0.25, 0.3, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (Vector4D(0.8, 0.8, 1, 0.5)).Array());
	glutSolidCube(4);
	glPopMatrix();

}
// Binds textures to our skybox cube
void World::initSkyboxList() {
	float OFFSET = 1/512.0; // Used for seamless edges

	skyboxList = glGenLists(1);
	glNewList(skyboxList, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	glBegin (GL_QUADS);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET,0.0 + OFFSET);	glVertex3d (-1.0,-1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET,0.0 + OFFSET);	glVertex3d (-1.0,-1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET,1.0 - OFFSET);	glVertex3d (-1.0,1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET,1.0 - OFFSET);	glVertex3d (-1.0,1.0,1.0);
	glEnd ();

	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	glBegin (GL_QUADS);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET,0.0 + OFFSET);	glVertex3d (-1.0,-1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET,0.0 + OFFSET);	glVertex3d (1.0,-1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET,1.0 - OFFSET);	glVertex3d (1.0,1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET,1.0 - OFFSET);	glVertex3d (-1.0,1.0,-1.0);
	glEnd ();

	glBindTexture(GL_TEXTURE_2D, skybox[2]);
	glBegin (GL_QUADS);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET ,0.0 + OFFSET); glVertex3d (1.0,-1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET ,0.0 + OFFSET); glVertex3d (1.0,-1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET ,1.0 - OFFSET); glVertex3d (1.0,1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET ,1.0 - OFFSET); glVertex3d (1.0,1.0,-1.0);
	glEnd ();

	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	glBegin (GL_QUADS);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET ,0.0 + OFFSET); glVertex3d (1.0,-1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET ,0.0 + OFFSET); glVertex3d (-1.0,-1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET ,1.0 - OFFSET); glVertex3d (-1.0,1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET ,1.0 - OFFSET); glVertex3d (1.0,1.0,1.0);
	glEnd ();

	glBindTexture(GL_TEXTURE_2D, skybox[4]);
	glBegin (GL_QUADS);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET ,0.0 + OFFSET); glVertex3d (-1.0,-1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET ,0.0 + OFFSET); glVertex3d (1.0,-1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET ,1.0 - OFFSET); glVertex3d (1.0,-1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET ,1.0 - OFFSET); glVertex3d (-1.0,-1.0,-1.0);
	glEnd ();

	glBindTexture(GL_TEXTURE_2D, skybox[5]);
	glBegin (GL_QUADS);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET ,0.0 + OFFSET); glVertex3d (-1.0,1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET ,0.0 + OFFSET); glVertex3d (1.0,1.0,-1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (1.0 - OFFSET ,1.0 - OFFSET); glVertex3d (1.0,1.0,1.0);
		glNormal3d (0.0,0.0,1.0); glTexCoord2d (0.0 + OFFSET ,1.0 - OFFSET); glVertex3d (-1.0,1.0,1.0);
	glEnd ();

	glEndList();
}

void World::setSkyboxPosition(Vector3D *position) {
	skyboxPosition = position;
}

bool World::checkCollision(Car *car) {
	return map->checkCollision(car);
}

// Global world light
void World::renderLight() {
	diffuse = Vector4D(5, 5, 5, 1);
	ambient = Vector4D(0, 0, 0, 0);
	specular = Vector4D(1, 1, 1, 1);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2f);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, Vector4D(diffuse.x, diffuse.y,	diffuse.z, diffuse.a).Array());
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient.Array());
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular.Array());
	glLightfv(GL_LIGHT0, GL_POSITION, Vector4D(0, 20, 0, 1).Array());

	glEnable(GL_LIGHT0);
}
