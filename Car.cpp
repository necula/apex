// "Car" renders and updates car model.

#include "Car.h"

// Headlights glLight settings
float leftHeadlightPos[] = { -2, 0.1, -5, 1.0 };
float rightHeadlightPos[] = { 2, 0.1, -5, 1.0 };
float headlightAmbient[] = { 0, 0, 0, 0 };
float headlightDiffuse[] = { 3.5, 3.5, 3.5, 1.0 };
float headlightSpecular[] = { 1, 1, 1, 1.0 };
float headlightDirection[] = { 0, -1.05, -1 }; // Slightly downwards

// Stoplights color
float stoplightColor[] = { 3, 0.5, 0.5, 0.5 };


Car::Car() {
	SetColor(new Vector3D(0.3, 0.3, 0.5));
	SetPosition(new Vector3D(-5, 0, 7.5));
	SetRotation(new Vector3D(0, -90, 0));
	scale = Vector3D(1.0, 1.0, 1.0);
	velocity = 0;

	buildCar();
}

void Car::Draw(int showCar) {

	glPushMatrix();

	glTranslatef(translation.x, translation.y, translation.z);

	glRotatef(rotation.x, 1.0, 0.0, 0.0);
	glRotatef(rotation.y, 0.0, 1.0, 0.0);
	glRotatef(rotation.z, 0.0, 0.0, 1.0);

	glScalef(scale.x, scale.y, scale.z);

	glTranslatef(0, 0.45, 0);

	glScalef(0.35, 0.35, 0.35);
	glScalef(0.7, 0.7, 0.7);

	// If NOSECAMERA don't render car model
	if(showCar != 0) {
		// Stoplights
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, stoplightColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, stoplightColor);
		glCallList(carList + 3);
		// Headlights
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (Vector4D(4, 4, 4, 0.5)).Array());
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (Vector4D(3, 3, 3, 0.5)).Array());
		glCallList(carList + 4);
		// Windows
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (Vector4D(0.6, 0.6, 1, 0.5)).Array());
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (Vector4D(0.1, 0.1, 0.1, 0.5)).Array());
		glCallList(carList + 1);
		// Wheels
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (Vector4D(0.1, 0.1, 0.1, 1)).Array());
		glCallList(carList + 2);
		// Body
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (Vector4D(0.1, 0.1, 0.1, 1)).Array());
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (Vector4D(0.5, 0.5, 0.5, 1)).Array());
		glCallList(carList);
	}

	// Headlight spotlights
	glLightfv(GL_LIGHT1, GL_DIFFUSE, headlightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, leftHeadlightPos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, headlightDirection);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 55);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 4.0);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, headlightDiffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, rightHeadlightPos);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, headlightDirection);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 55);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 4.0);
	glEnable(GL_LIGHT2);

	glPopMatrix();
}

// Update car speed and position
void Car::update(int steeringInput, int acc, int brk) {
	float step = 0;

	stoplightColor[0] = 3;
	stoplightColor[1] = 0.5;
	stoplightColor[2] = 0.5;

	// No movement
	if(!acc && !brk && velocity < 3 && velocity > -3)
		velocity = 0;

	if(velocity < 0)
		steeringInput *= -1; // Going into reverse

	// Steer only if moving
	if(velocity != 0) {
		if(steeringInput == -1) {
			rotation.y += STEERANGLE; // Steering LEFT
		}
		else if(steeringInput == 1) {
			rotation.y -= STEERANGLE; // Stering RIGHT
		}
		else {
			// NO steering - do nothing
		}
	}

	if(acc) // Accelerating
	{
		velocity += FORWARDACC;
		// Limit forward speed
		if(velocity > MAXVEL)
			velocity = MAXVEL;
	}
	if(brk)
	{
		// Braking
		if(velocity > 0) {
			velocity -= BRKPOWER;
			// Change stoplights color for braking
			stoplightColor[0] = 6;
			stoplightColor[1] = 0;
			stoplightColor[2] = 0;
		}
		// Reverse
		else {
			velocity -= REVERSEACC;
			// Change stoplights color for reverse
			stoplightColor[0] = 4;
			stoplightColor[1] = 4;
			stoplightColor[2] = 4;
		}
		// Limit reverse speed
		if(velocity < MAXNVEL)
			velocity = MAXNVEL;

	}

	// Get direction vector based on rotation
	direction.x = sin(rotation.y * PI / 180);
	direction.z = cos(rotation.y * PI / 180);

	step = -velocity / 100;
	// Move it
	SetPosition(&(GetPosition() + Vector3D(step * direction.x, 0, step * direction.z)));

	// Steer it
	//rotation.y += steer;

	// Next speed will be lower
	velocity *= DRAG;
}

// Move car to last safe (non-colliding) position
void Car::hasCollided() {
	translation.x = lastSafePos.x;
	translation.z = lastSafePos.z;
	translation.y = lastSafePos.y;
}

// Put car in a slow speed state
void Car::slow() {
	velocity *= 0.5;
}

// Generate car model using display lists
// Noob approach, I know :)
void Car::buildCar() {
	carList = glGenLists(5);

		glNewList(carList, GL_COMPILE);
		// Roof
		glTranslatef(0, 1, 0);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 0, 1);
		glVertex3f(-1, 0, 1);
		glVertex3f(-1, 0, -1);
		glVertex3f(1, 0, -1);
		glEnd();
		glTranslatef(0, -1, 0);

		// Floor
		glTranslatef(0, -1, 0);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 0, 3);
		glVertex3f(-1, 0, 3);
		glVertex3f(-1, 0, -3);
		glVertex3f(1, 0, -3);
		glEnd();
		glTranslatef(0, 1, 0);

		// Hood
		glTranslatef(0, 0, -2);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 0, 0.2);
		glVertex3f(-1, 0, 0.2);
		glVertex3f(-1, 0, -1);
		glVertex3f(1, 0, -1);
		glEnd();
		glTranslatef(0, 0, 2);

		// Trunk
		glRotatef(180, 0, 1, 0);
		glTranslatef(0, 0, -2);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(1, 0, 0.2);
		glVertex3f(-1, 0, 0.2);
		glVertex3f(-1, 0, -1);
		glVertex3f(1, 0, -1);
		glEnd();
		glTranslatef(0, 0, 2);
		glRotatef(180, 0, 1, 0);

		// Middle right side
		glTranslatef(1, -0.5, 0);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-1, 0.5, 0);

		// Front right side
		glTranslatef(1, -0.5, -2);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-1, 0.5, 2);

		// Back right side
		glTranslatef(1, -0.5, 2);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-1, 0.5, -2);

		glRotatef(180, 0, 1, 0);

		// Middle left side
		glTranslatef(1, -0.5, 0);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-1, 0.5, 0);

		// Front left side
		glTranslatef(1, -0.5, -2);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-1, 0.5, 2);

		// Back left side
		glTranslatef(1, -0.5, 2);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-1, 0.5, -2);

		glRotatef(-180, 0, 1, 0);

		// Front piece
		glTranslatef(0, -0.5, -3);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glRotatef(-90, 0, 1, 0);
		glTranslatef(0, 0.5, 3);

		// Back piece
		glRotatef(180, 0, 1, 0);
		glTranslatef(0, -0.5, -3);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glRotatef(-90, 0, 1, 0);
		glTranslatef(0, 0.5, 3);
		glRotatef(-180, 0, 1, 0);


		glEndList();

		// WINDOWS
		glNewList(carList + 1, GL_COMPILE);

		// Left
		glTranslatef(-1, 0.5, 0);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(1, -0.5, 0);

		// Right
		glTranslatef(1, 0.5, 0);
		glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(0.5, 0, 1);
		glVertex3f(-0.5, 0, 1);
		glVertex3f(-0.5, 0, -1);
		glVertex3f(0.5, 0, -1);
		glEnd();
		glRotatef(-90, 0, 0, 1);
		glTranslatef(-1, -0.5, 0);

		// Windscreen
		glTranslatef(0, 1, -1);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, -1);

		glVertex3f(1, 0, 0);
		glVertex3f(-1, 0, 0);
		glVertex3f(-1, -1, -0.8);
		glVertex3f(1, -1, -0.8);
		glEnd();
		glTranslatef(0, -1, 1);

		// Rear window
		glRotatef(180, 0, 1, 0);
		glTranslatef(0, 1, -1);
		glBegin(GL_QUADS);
		glNormal3f(0, 1, -1);

		glVertex3f(1, 0, 0);
		glVertex3f(-1, 0, 0);
		glVertex3f(-1, -1, -0.8);
		glVertex3f(1, -1, -0.8);
		glEnd();
		glTranslatef(0, -1, 1);
		glRotatef(-180, 0, 1, 0);

		glEndList();

		// WHEELS

		glNewList(carList + 2, GL_COMPILE);

		glTranslatef(-1.2, -0.75, -2);
		glScalef(0.25, 0.25, 0.25);
		glRotatef(90, 0, 1, 0);
		glutSolidTorus(0.8, 1.5, 15, 15);

		glRotatef(-90, 0, 1, 0);
		glScalef(1 / 0.25, 1 / 0.25, 1 / 0.25);
		glTranslatef(1.2, 0.75, 2);

		glTranslatef(1.2, -0.75, -2);
		glScalef(0.25, 0.25, 0.25);
		glRotatef(90, 0, 1, 0);
		glutSolidTorus(0.8, 1.5, 15, 15);
		glRotatef(-90, 0, 1, 0);
		glScalef(1 / 0.25, 1 / 0.25, 1 / 0.25);
		glTranslatef(-1.2, 0.75, 2);

		glTranslatef(-1.2, -0.75, 2);
		glScalef(0.25, 0.25, 0.25);
		glRotatef(90, 0, 1, 0);
		glutSolidTorus(0.8, 1.5, 15, 15);
		glRotatef(-90, 0, 1, 0);
		glScalef(1 / 0.25, 1 / 0.25, 1 / 0.25);
		glTranslatef(1.2, 0.75, -2);

		glTranslatef(1.2, -0.75, 2);
		glScalef(0.25, 0.25, 0.25);
		glRotatef(90, 0, 1, 0);
		glutSolidTorus(0.8, 1.5, 15, 15);
		glRotatef(-90, 0, 1, 0);
		glScalef(1 / 0.25, 1 / 0.25, 1 / 0.25);
		glTranslatef(-1.2, 0.75, -2);

		glEndList();

		// STOPLIGHTS
		glNewList(carList + 3, GL_COMPILE);

		glRotatef(180, 0, 1, 0);
		glTranslatef(0.5, -0.5, -3.05);
		glScalef(1, 0.5, 0.1);
		glutSolidCube(0.5);
		glScalef(1, 2, 10);
		glTranslatef(-0.5, 0.5, 3.05);
		glRotatef(180, 0, 1, 0);

		glRotatef(180, 0, 1, 0);
		glTranslatef(-0.5, -0.5, -3.05);
		glScalef(1, 0.5, 0.1);
		glutSolidCube(0.5);
		glScalef(1, 2, 10);
		glTranslatef(0.5, 0.5, 3.05);
		glRotatef(180, 0, 1, 0);

		glEndList();

		// HEADLIGHTS
		glNewList(carList + 4, GL_COMPILE);

		glRotatef(180, 0, 1, 0);
		glTranslatef(0.5, -0.5, 3);
		glScalef(1, 0.5, 0.25);
		glutSolidCube(0.5);
		glScalef(1, 2, 1/0.25);
		glTranslatef(-0.5, 0.5, -3);
		glRotatef(180, 0, 1, 0);

		glRotatef(180, 0, 1, 0);
		glTranslatef(-0.5, -0.5, 3);
		glScalef(1, 0.5, 0.25);
		glutSolidCube(0.5);
		glScalef(1, 2, 4);
		glTranslatef(0.5, 0.5, -3);
		glRotatef(180, 0, 1, 0);

		glEndList();
}

