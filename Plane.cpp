#include <stdio.h>
#include "Plane.h"
#include "BmpToTexture.h"

Plane::Plane(float size) : Object3D() {
	float leftMargin = -size, rightMargin = size;
	float factor = 0.1f;
	levelOfDetail = 1;

	planeList = glGenLists(1);

	glNewList(planeList, GL_COMPILE);
	//glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	// Generate the ground floor
	for(float i = leftMargin+4; i + (levelOfDetail * factor) <= rightMargin-4; i += (levelOfDetail * factor))
		for(float j = leftMargin+14; j + (levelOfDetail * factor) <= rightMargin-14; j += (levelOfDetail * factor)) {
			glBegin(GL_QUADS);

			glNormal3f(0.0, 1.0, 0.0);
			glTexCoord2d(0, 0);
			glVertex3f(i, 0, j);
			glTexCoord2d(0, 1);
			glVertex3f(i, 0, j + (levelOfDetail * factor));
			glTexCoord2d(1, 1);
			glVertex3f(i + (levelOfDetail * factor), 0, j + (levelOfDetail
					* factor));
			glTexCoord2d(1, 0);
			glVertex3f(i + (levelOfDetail * factor), 0, j);

			glEnd();
		}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEndList();
}

void Plane::customDraw() {
	glPushMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (Vector4D(color.x , color.y , color.z , 0.2)).Array());
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (Vector4D(0.1, 0.1, 0.1, 0)).Array());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (Vector4D(0.1, 0.1, 0.1, 0)).Array());

	glCallList(planeList);

	glPopMatrix();
}
