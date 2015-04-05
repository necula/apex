// "Map" class loads level data from a file
// and renders it on the screen.

#include "Map.h"

Map::Map() {
	color = Vector3D(0.15, 0.15, 0.15);
	lastHitX = 0;
	lastHitY = 0;

	readLevelData();
	buildLevel();
}

// Free up some memory
Map::~Map() {
	for(int i = 0; i < levelSize; i++)
		free(levelData[i]);
	free(levelData);
}

void Map::Draw() {

	glPushMatrix();

	glColor3f(color.x, color.y, color.z);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (Vector4D(color.x, color.y, color.z, 1)).Array());
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (Vector4D(0.1, 0.1, 0.1, 1)).Array());
	// Render level map
	glCallList(level);

	glPopMatrix();

}

float distance(float x, float y, float cx, float cy) {
	return sqrt(pow(cx - x, 2) + pow(cy - y, 2));
}

// Overly simplistic (and expensive) collision detection
// First checks if it's close enough to an object and then
// it checks if any of the car parts (front, center, back)
// collides with the object.
bool Map::checkCollision(Car *car) {
	GLfloat cx = car->GetPosition().x;
	GLfloat cy = car->GetPosition().z;

	float rot = car->GetRotation().y * PI / 180;
	float offsetX = 0.9 * sin(rot);
	float offsetY = 0.9 * cos(rot);

	for(int i = 0; i < levelSize; i++) {
		if(distance(levelData[i][0], levelData[i][1], cx, cy) < 3) {

			// Checks the front of the car
			float frontDist = distance(levelData[i][0], levelData[i][1], cx - offsetX, cy - offsetY);
			if(frontDist < 0.3) { // Tire slows down the car
				car->slow();
			}
			if(frontDist < 0.15) { // Tire stops the car
				car->slow();
				car->hasCollided();
			}
			// Checks the back of the car
			float backDist = distance(levelData[i][0], levelData[i][1], cx + offsetX, cy + offsetY);
			if(backDist < 0.3) { // Tire slows down the car
				car->slow();
			}
			if(backDist < 0.15) { // Tire stops the car
				car->slow();
				car->hasCollided();
			}
			// Make sure car doesn't go between 2 adjacent tires
			if(distance(levelData[i][0], levelData[i][1], cx, cy) < 0.6) {
				car->hasCollided();
				car->slow();
			}
		}
	}
	return false;
}

// Read level configuration from file
void Map::readLevelData() {
	dataFile = fopen("data\\map.data", "r");
	if(dataFile == NULL) {
		fprintf(stderr, "ERR: fopen()");
		exit(1);
	}

	fscanf(dataFile, "%i\n", &levelSize);

	levelData = (int**) malloc((levelSize) * sizeof(int*));
	for(int i = 0; i < levelSize; i++) {
		levelData[i] = (int*) malloc(2 * sizeof(int));
	}

	for(int i = 0; i < levelSize; i++) {
		fscanf(dataFile, "%i %i\n", &levelData[i][0], &levelData[i][1]);
		levelData[i][1] -= 10;
	}
}

// Generate level display list
void Map::buildLevel() {
	level = glGenLists(1);
	glNewList(level, GL_COMPILE);

	// Represent level object as solid torus (like a tire)
	for(int i = 0; i < levelSize; i++) {
		glPushMatrix();
		glTranslatef(levelData[i][0], 0.1, levelData[i][1]);
		glRotatef(90, 1, 0, 0);
		glutSolidTorus(0.15, 0.35, 10, 10);
		glPopMatrix();
	}
	glEndList();
}

