#include <stdlib.h>
#include <glut.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>

#include "Main.h"

CameraSet *cameraSet;
Car *car;
World *world;

void init(void) {
	Vector3D::arr = new float[3];
	Vector4D::arr = new float[4];

	car = new Car();
	cameraSet = new CameraSet(car); // Bind the camera system to our car
	world = new World();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

}

void drawScene() {
	world->draw();
	// If NOSECAMERA is active don't render car model
	(cameraSet->getCameraType() != NOSECAMERA)?(car->Draw(1)):(car->Draw(0));
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraSet->render(); // Places the observer
	world->renderLight(); // Global light
	
	
	if(cameraSet->isFreeCamActive())
		world->setSkyboxPosition(&(cameraSet->getPosition())); // Skybox follows free camera
	else
		world->setSkyboxPosition(&(car->GetPosition())); // Skybox follows the car 

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL, 1);
	drawScene(); // Draws only opaque objects

	// Checks if player passed the start/finish at least once
	// and displays various race information
	
	if(lapCount)	drawText(1, 45, "  Lap %i  %.2fs", lapCount, currentLapTime/1000);
	else			drawText(1, 45, "Timer not started", lapCount, currentLapTime/1000);

	if(lastLapTime) drawText(1, 25, "Last Lap %.2fs", lastLapTime/1000);
	else 			drawText(1, 25, "Last Lap   -");

	if(bestLapTime) drawText(1,  5, "Best Lap %.2fs", bestLapTime/1000);
	else			drawText(1,  5, "Best Lap   -");

	glAlphaFunc(GL_LESS, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	drawScene(); // Draws transparent objects
	glDisable(GL_BLEND);

	glutSwapBuffers();
	glutPostRedisplay();
}

void update(int value) {

	glutTimerFunc(TIMERMSECS, update, 0);

	// Measure the elapsed time
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int timeSincePrevFrame = currTime - prevTime;

	// FPS Counter
	fpsTimer += timeSincePrevFrame;
	if(fpsTimer > 1000) {
		fpsTimer = 0;
		printf("FPS= %i\n", fps);
		fps = 0;
	}
	fps++;

	// Collision detection
	if(!world->checkCollision(car)) {
		car->lastSafePos = car->GetPosition(); // Update safe position
	}


	steering = 0;
	// Do we need to steer?
	if(rightPressed) steering = 1;
	if(leftPressed) steering = -1;

	// Car speed/position update
	car->update(steering, upPressed, downPressed);

	// Camera update
	cameraSet->update();

	// Lap timing updates
	currentLapTime = currTime - lapStartTime;
	if(distTwoPoints(car->GetPosition().x, car->GetPosition().z, 0, 7) < 2.5) {
		if(!onStart) { // Is the player near start/finish ?
			if(lapCount)
				lastLapTime = currentLapTime;

			// Checks if last lap was the fastest
			if(lapCount && (lastLapTime < bestLapTime || !bestLapTime)) {
				bestLapTime = lastLapTime;
			}

			lapStartTime = currTime;
			lapCount++;
			onStart = true;
		}
	}
	else { // Player's not near start/finish. Can record lap time.
		onStart = false;
	}
	prevTime = currTime;

	glutPostRedisplay();
}

// When the window is resized we update the projection
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	GLfloat aspect = (GLfloat) w / (GLfloat) h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, aspect, 1.0, 60);
}

// Normal keyboard presses
void keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case ESC:
			exit(0);
		case 'c':
		case 'C': {
			cameraSet->nextCamera();
		} break;
		default:
			break;
	}

	if(cameraSet->isFreeCamActive())
		cameraSet->updateFreeCam(key);
}

// Special keyboard presses
void keyboard(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_RIGHT :
			rightPressed = 1;
			break;
		case GLUT_KEY_LEFT :
			leftPressed = 1;
			break;
		case GLUT_KEY_DOWN :
			downPressed = 1;
			break;
		case GLUT_KEY_UP :
			upPressed = 1;
			break;
	}
}

// Special keyboard releases
void keyboardUp(int key, int x, int y) {
	switch(key) {
		case GLUT_KEY_RIGHT :
			rightPressed = 0;
			break;
		case GLUT_KEY_LEFT :
			leftPressed = 0;
			break;
		case GLUT_KEY_DOWN :
			downPressed = 0;
			break;
		case GLUT_KEY_UP :
			upPressed = 0;
			break;
	}
}

// This function is activated when we click inside
// the window and drag the mouse.
// If freecam is active we can rotate the camera
// similar to FPS.
void freecam(int x, int y) {
	if(cameraSet->isFreeCamActive()) {
		Camera *freeCam = cameraSet->getFreeCam();

		// Horizontal drag
		if(y - oldY > 2) {
			freeCam->RotateX(-CAMSPEED);
		}
		else if(y - oldY < -2) {
			freeCam->RotateX(CAMSPEED);
		}
		// Vertical drag
		if(x - oldX > 2) {
			float rotat = freeCam->rotation.x;
			freeCam->RotateX(-rotat);
			freeCam->RotateY(CAMSPEED);
			freeCam->RotateX(rotat);
		}
		else if(x - oldX < -2) {
			float rotat = freeCam->rotation.x;
			freeCam->RotateX(-rotat);
			freeCam->RotateY(-CAMSPEED);
			freeCam->RotateX(rotat);
		}
	}

	oldX = x;
	oldY = y;
}

// Draws 2D text on screen
void drawText(GLfloat x, GLfloat y, char *format, ...) {
	va_list args;
	char buffer[1024], *p;

	va_start(args,format);
	vsprintf(buffer, format, args);
	va_end(args);

	glDisable(GL_LIGHTING);
	glDisable (GL_DEPTH_TEST);
	glLineWidth(2.0);

	// We need orthographic projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, 1200, 0.0f, 800, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1, 0, 0);
	glTranslatef(x, y, 0);
	glScalef(0.1, 0.1, 0.0);

	// Draw text
	for(p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);

	// We need perpective projection back
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Apex");
	init();
	glutTimerFunc(TIMERMSECS, update, 0);
	startTime = glutGet(GLUT_ELAPSED_TIME);
	prevTime = startTime;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard);
	glutSpecialUpFunc(keyboardUp);
	glutMotionFunc(freecam);
	glutMainLoop();
	return 0;
}
