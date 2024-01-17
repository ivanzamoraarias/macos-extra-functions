#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <stdlib.h>

float spaceshipPosition = -5.0; // Initial spaceship position
float cameraZ = 0.0;           // Initial camera position along the z-axis

#define NUM_STARS 100
float stars[NUM_STARS][3];

void initStars() {
    for (int i = 0; i < NUM_STARS; ++i) {
        stars[i][0] = (rand() % 2000 - 1000) / 100.0; // X-coordinate
        stars[i][1] = (rand() % 2000 - 1000) / 100.0; // Y-coordinate
        stars[i][2] = (rand() % 2000 - 1000) / 100.0; // Z-coordinate
    }
}

void drawSpaceship() {
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);  // White

    // Front
    glVertex3f(-0.5, 0.5, 0.2);
    glVertex3f(0.5, 0.5, 0.2);
    glVertex3f(0.5, -0.5, 0.2);
    glVertex3f(-0.5, -0.5, 0.2);

    // Back
    glVertex3f(-0.5, 0.5, -0.2);
    glVertex3f(0.5, 0.5, -0.2);
    glVertex3f(0.5, -0.5, -0.2);
    glVertex3f(-0.5, -0.5, -0.2);

    // Left
    glVertex3f(-0.5, 0.5, 0.2);
    glVertex3f(-0.5, -0.5, 0.2);
    glVertex3f(-0.5, -0.5, -0.2);
    glVertex3f(-0.5, 0.5, -0.2);

    // Right
    glVertex3f(0.5, 0.5, 0.2);
    glVertex3f(0.5, -0.5, 0.2);
    glVertex3f(0.5, -0.5, -0.2);
    glVertex3f(0.5, 0.5, -0.2);

    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0);  // Blue

    // Top
    glVertex3f(0.0, 0.5, 0.2);
    glVertex3f(-0.5, -0.5, 0.2);
    glVertex3f(0.5, -0.5, 0.2);

    // Bottom
    glVertex3f(0.0, 0.5, -0.2);
    glVertex3f(-0.5, -0.5, -0.2);
    glVertex3f(0.5, -0.5, -0.2);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0);  // Green

    // Left wing
    glVertex3f(-0.7, 0.1, 0.2);
    glVertex3f(-0.5, 0.1, 0.2);
    glVertex3f(-0.5, -0.1, 0.2);
    glVertex3f(-0.7, -0.1, 0.2);

    glVertex3f(-0.7, 0.05, -0.2);
    glVertex3f(-0.5, 0.05, -0.2);
    glVertex3f(-0.5, -0.05, -0.2);
    glVertex3f(-0.7, -0.05, -0.2);

    // Right wing
    glVertex3f(0.5, 0.1, 0.2);
    glVertex3f(0.7, 0.1, 0.2);
    glVertex3f(0.7, -0.1, 0.2);
    glVertex3f(0.5, -0.1, 0.2);

    glVertex3f(0.5, 0.05, -0.2);
    glVertex3f(0.7, 0.05, -0.2);
    glVertex3f(0.7, -0.05, -0.2);
    glVertex3f(0.5, -0.05, -0.2);

    // Head
    glColor3f(1.0, 0.0, 0.0);  // Red
    glVertex3f(0.0, 0.7, 0.2);
    glVertex3f(-0.2, 0.4, 0.2);
    glVertex3f(0.2, 0.4, 0.2);
    glVertex3f(0.0, 0.1, 0.2);

    glEnd();
}

void drawStars() {
    glPointSize(1.0);
    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 1.0);  // White

    for (int i = 0; i < NUM_STARS; ++i) {
        glVertex3f(stars[i][0], stars[i][1], stars[i][2]);
    }

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, cameraZ, 0.0, 0.0, cameraZ - 5.0, 0.0, 1.0, 0.0);

    // Move the spaceship forward
    spaceshipPosition += 0.02;
    glTranslatef(0.0, 0.0, spaceshipPosition);

    drawStars(); // Draw starfield
    drawSpaceship();

    glutSwapBuffers();
}

void update(int value) {
    cameraZ += 0.02; // Move camera forward
    if (spaceshipPosition > 5.0) {
        spaceshipPosition = -5.0; // Reset spaceship position
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Moving Spaceship with Starfield");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    initStars(); // Initialize star positions

    glutDisplayFunc(display);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}
