// Rachel Collier
// Bus Project 3

#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <GLUT/glut.h>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>

#define GLEW_STATIC
#include <math.h>
#include <stdlib.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GLUT/glut.h>
#endif

int vec3_inPosition; // Vertex position
int vec3_inNormal;   // Vertex normal


/* ascii value for the escape key */
const int KEY_ESC = 27;
const int MOVE_EYE = 0;
const int TWIST_EYE = 1;
const int ZOOM = 2;
const int MOVE_NONE = 3;
 
/* Global Variables */

// For the keys
enum ViewMode {
    FRONT_VIEW,
    BACK_VIEW,
    RIGHT_VIEW,
    LEFT_VIEW,
    TOP_VIEW,
    UNDER_VIEW,
    ISOMETRIC_VIEW,
    MIDDLEDOOR,
    MIDDLEBACK
};

ViewMode currentView = FRONT_VIEW;
 
const char* progname = "CollierRachelBusVersion3";
static int width, height;
//enum actions { MOVE_EYE, TWIST_EYE, ZOOM, MOVE_NONE };
static int eye = 1, flat = 0;
static int action;
static double xStart = 0.0, yStart = 0.0;
static float nearClip, farClip, distance, twistAngle, incAngle, azimAngle;
 
static float vertices[22][3] = {
    {-1.0f, -1.0f, -1.0f}, // vertex 0
    { 1.0f, -1.0f, -1.0f},
    { 1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},
    {-1.0f, -1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f}, // vertex 5
    { 1.0f,  1.0f,  1.0f},
    {-1.0f,  1.0f,  1.0f},
    { 1.0f, -1.0f,  0.3f},
    { 1.0f,  1.0f,  0.3f},
    { 1.0f,  1.0f, -0.3f}, // vertex 10
    { 1.0f, -1.0f, -0.3f},
    { 1.0f,  0.3f,  0.3f},
    { 1.0f,  0.3f, -0.3f},
    {-0.3f, -1.0f,  1.0f},
    {-0.3f,  1.0f,  1.0f}, // vertex 15
    { 0.3f,  1.0f,  1.0f},
    { 0.3f, -1.0f,  1.0f},
    {-0.3f,  0.5f,  1.0f},
    { 0.3f,  0.5f,  1.0f},
    { 0.3f, -0.5f,  1.0f}, // vertex 20
    {-0.3f, -0.5f,  1.0f}
};
 
static float normals[22][3] = {
    {-1.0f, -1.0f, -1.0f}, // normal 0
    { 1.0f, -1.0f, -1.0f},
    { 1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},
    {-1.0f, -1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f}, // normal 5
    { 1.0f,  1.0f,  1.0f},
    {-1.0f,  1.0f,  1.0f},
    { 1.0f, -1.0f,  0.3f},
    { 1.0f,  0.3f,  0.3f},
    { 1.0f,  0.3f, -0.3f}, // normal 10
    { 1.0f, -1.0f, -0.3f},
    { 1.0f,  0.3f,  0.3f},
    { 1.0f,  0.3f, -0.3f},
    {-0.3f, -1.0f,  1.0f},
    {-0.3f,  1.0f,  1.0f}, // normal 15
    { 0.3f,  1.0f,  1.0f},
    { 0.3f, -1.0f,  1.0f},
    {-0.3f,  0.5f,  1.0f},
    { 0.3f,  0.5f,  1.0f},
    { 0.3f, -0.5f,  1.0f}, // normal 20
    {-0.3f, -0.5f,  1.0f}
};
 
static float colors[22][3] = {
    { 0.0f,  0.0f,  0.0f}, // color 0
    { 1.0f,  0.0f,  0.0f},
    { 1.0f,  1.0f,  0.0f},
    { 0.0f,  1.0f,  0.0f},
    { 0.0f,  0.0f,  1.0f},
    { 1.0f,  0.0f,  1.0f}, // color 5
    { 1.0f,  1.0f,  1.0f},
    { 0.0f,  1.0f,  1.0f},
    { 1.0f,  0.5f,  0.0f},
    { 1.0f,  0.5f,  0.0f},
    { 1.0f,  0.5f,  0.0f}, // color 10
    { 1.0f,  0.5f,  0.0f},
    { 1.0f,  0.5f,  0.0f},
    { 1.0f,  0.5f,  0.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f}, // color 15
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f}, // color 20
    { 1.0f,  1.0f,  1.0f}
};
 

// Commands for looking at the bus :)
static void printHelp() {
    printf("\n%s - Move around in space\n\n"
        "F1 key        - print help information\n"
        "Left Mousebutton    - move eye position\n"
        "Middle Mousebutton    - change twist angle\n"
        "Right Mousebutton    - move up / down to zoom in / out\n"
        "Space Key            - change the color of the background\n"
        "<f> Key        - front of the bus\n"
        "<b> Key        - back of the bus\n"
        "<r> Key        - reset viewpoint\n"
        "<r> Key        - right side of bus\n"
        "<l> Key        - left side of bus\n"
        "<u> Key        - under the bus\n"
        "<t> Key        - top of the bus\n"
        "<i> Key        - isometric view of the bus\n"
        "<m> Key        - at the DOOR looking toward the middle back BUS\n"
        "<n> Key        - in the middle of the BUS looking at the DOOR\n"
        "<0> Key        - bus before lighting\n"
        "<1> Key        - places the spotlight to the front right\n"
        "<2> Key        - places the spotlight to the front left\n"
        "<3> Key        - places the spotlight to the front middle\n"
        "<4> Key        - places the spotlight to the middle right\n"
        "<5> Key        - places the spotlight to the middle side left\n"
        "<6> Key        - places the spotlight to the middle side right\n"
        "<7> Key        - places the spotlight to the middle side bottom\n"
        "<8> Key        - no lighting\n"
        "Escape Key     - exit the program\n\n", progname);
}
 

static void resetView() {
    distance = (float)nearClip + (float)((farClip - nearClip) / 2.0);
    
    // Reset rotation angles based on the selected view
    switch (currentView) {
        case FRONT_VIEW:
            twistAngle = 0.0f;
            incAngle = 0.0f;
            azimAngle = 90.0f;
            break;
        case BACK_VIEW:
            twistAngle = 0.0f;
            incAngle = 0.0f;
            azimAngle = -90.0f;
            break;
        case RIGHT_VIEW:
            twistAngle = 0.0f;
            incAngle = 0.0f;
            azimAngle = 360.0f;
            break;
        case LEFT_VIEW:
            twistAngle = 0.0f;
            incAngle = 0.0f;
            azimAngle = 180.0f;
            break;
        case TOP_VIEW:
            twistAngle = 0.0f;
            incAngle = -90.0f;
            azimAngle = 0.0f;
            break;
        case UNDER_VIEW:
            twistAngle = 0.0f;
            incAngle = 90.0f;
            azimAngle = 0.0f;
            break;
        case ISOMETRIC_VIEW:
            twistAngle = 45.0f;
            incAngle = 30.0f;
            azimAngle = 45.0f;
            break;
        case MIDDLEDOOR:
            twistAngle = 0.0f;
            incAngle = 0.0f;
            azimAngle = 70.0f;
            distance = 1.5f;
            break;
        case MIDDLEBACK:
            twistAngle = 0.0f;    // No twist
            incAngle = 0.0f;      // No incidence angle
            azimAngle = 210.0f;    // Inside the bus
            distance = 1.0f;      // Distance from the center of the bus
            break;
        default:
            break;
    }
}

 
static void initgfx() {
    /* set clear color to black */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
    /* enable the depth buffer */
    glEnable(GL_DEPTH_TEST);
 
    nearClip = 1.0f;
    farClip = nearClip + 10.0f;
 
    resetView();
}
 
static void reshape(int width, int height) {
 
    glViewport(0, 0, width, height);
 
    /* compute aspect ratio */
    double aspect = (double)width / (double)height;
    if (height > width) aspect = 1.0f;
    glMatrixMode(GL_PROJECTION);
 
    /* Reset world coordinates first ... */
    glLoadIdentity();
 
    /* Reset the viewing volume based on the new aspect ratio */
    gluPerspective(45.0f, aspect, nearClip, farClip);
 
    glMatrixMode(GL_MODELVIEW);
}
 
 
static void keyboard(unsigned char key, int x, int y) {
    if (key == 'e') {
        eye = eye ? 0 : 2;
    }
    else if (key == 'z') {
        eye = 1;
    }
    else if (key == ' ') {
        /* generate a random background color */
        float r = (float)(rand() % 32768) / 32768;
        float g = (float)(rand() % 32768) / 32768;
        float b = (float)(rand() % 32768) / 32768;
        glClearColor(r, g, b, 1.0f);
        glutPostRedisplay();
    }
    else if (key == 's') {
        /* toggle between smooth and flat shading */
        flat = flat ? 0 : 1;
        glShadeModel(flat ? GL_FLAT : GL_SMOOTH);
        glutPostRedisplay();
    }
    
    else if (key == KEY_ESC) {
        exit(0);
    }
    
    
    // Add view change keys
    else if (key == 'f') {
        currentView = FRONT_VIEW;
        resetView();
        glutPostRedisplay();
    }
    else if (key == 'b') {
        currentView = BACK_VIEW;
        resetView();
        glutPostRedisplay();
    }
    else if (key == 'r') {
        currentView = RIGHT_VIEW;
        resetView();
        glutPostRedisplay();
    }
    else if (key == 'l') {
        currentView = LEFT_VIEW;
        resetView();
        glutPostRedisplay();
    }
    else if (key == 't') {
        currentView = TOP_VIEW;
        resetView();
        glutPostRedisplay();
    }
    else if (key == 'u') {
        currentView = UNDER_VIEW;
        resetView();
        glutPostRedisplay();
    }
    else if (key == 'i') {
        currentView = ISOMETRIC_VIEW;
        resetView();
        glutPostRedisplay();
    }
    // start of lighting
    else if (key == '0') {
        glDisable(GL_LIGHTING); // Disable lighting
        glDisable(GL_COLOR_MATERIAL); // Disable color
        }
    else if (key == '1') {
        
        // ALL OF THE DIFFERENT LIGHTING POSITIONS
        GLfloat light1_pos[] = {2.0, 5.0, 10.0, 0.0};
        GLfloat light2_pos[] = {-2.0, 5.0, 10.0, 0.0};
        GLfloat light3_pos[] = {0.0, 5.0, 10.0, 0.0};
        GLfloat light4_pos[] = {2.0, 5.0, 10.0, 0.0};
        GLfloat light5_pos[] = {-5.0, 0.0, 0.0, 0.0};
        GLfloat light6_pos[] = {5.0, 0.0, 0.0, 0.0};
        GLfloat light7_pos[] = {0.0, -5.0, 0.0, 0.0};
        
        glEnable(GL_LIGHTING); // Enable lighting
        glEnable(GL_COLOR_MATERIAL); // Enable color
        float light_diffuse = {1.0};
        float light_ambient = {1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, light1_pos);
        glLightf(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightf(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT0);
        
    }
    else if (key == '2') {
        GLfloat light2_pos[] = {-2.0, 5.0, 10.0, 0.0};
        glEnable(GL_LIGHTING); // Enable lighting
        glEnable(GL_COLOR_MATERIAL); // Enable color
        float light_diffuse = {1.0};
        float light_ambient = {1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, light2_pos);
        glLightf(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightf(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT0);
        }
    else if (key == '3') {
        GLfloat light3_pos[] = {0.0, 5.0, 10.0, 0.0};
        glEnable(GL_LIGHTING); // Enable lighting
        glEnable(GL_COLOR_MATERIAL); // Enable color
        float light_diffuse = {1.0};
        float light_ambient = {1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, light3_pos);
        glLightf(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightf(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT0);
        }
    else if (key == '4') {
        GLfloat light4_pos[] = {2.0, 5.0, 10.0, 0.0};
        glEnable(GL_LIGHTING); // Enable lighting
        glEnable(GL_COLOR_MATERIAL); // Enable color
        float light_diffuse = {1.0};
        float light_ambient = {1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, light4_pos);
        glLightf(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightf(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT0);
        }
    else if (key == '5') {
        GLfloat light5_pos[] = {-5.0, 0.0, 0.0, 0.0};
        glEnable(GL_LIGHTING); // Enable lighting
        glEnable(GL_COLOR_MATERIAL); // Enable color
        float light_diffuse = {1.0};
        float light_ambient = {1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, light5_pos);
        glLightf(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightf(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT0);
        }
    else if (key == '6') {
        GLfloat light6_pos[] = {5.0, 0.0, 0.0, 0.0};
        glEnable(GL_LIGHTING); // Enable lighting
        glEnable(GL_COLOR_MATERIAL); // Enable color
        float light_diffuse = {1.0};
        float light_ambient = {1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, light6_pos);
        glLightf(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightf(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT0);
        }
    else if (key == '7') {
        GLfloat light7_pos[] = {0.0, -5.0, 0.0, 0.0};
        glEnable(GL_LIGHTING); // Enable lighting
        glEnable(GL_COLOR_MATERIAL); // Enable color
        float light_diffuse = {1.0};
        float light_ambient = {1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, light7_pos);
        glLightf(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightf(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glEnable(GL_LIGHT0);
        }
    else if (key == '8') {
        glEnable(GL_POSITION); // Enable lighting
        glEnable(GL_LIGHTING); // Enable lighting
        glEnable(GL_COLOR_MATERIAL); // Enable color
        glEnable(GL_LIGHT3);
        }
    else if (key == 'm') {
        currentView = MIDDLEDOOR;
        resetView();
        glutPostRedisplay();
    }
    else if (key == 'n') {
        currentView = MIDDLEBACK;
        resetView();
        glutPostRedisplay();
    }

}

static void specialkeys(int key, int x, int y) {
    if (key == GLUT_KEY_F1) {
        printHelp();
    }
}
 
static void mouse(int button, int state, int x, int y) {
 
    if (state != GLUT_DOWN) {
        action = MOVE_NONE;
        return;
    }
 
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (eye == 0)      action = TWIST_EYE;
            else if (eye == 1) action = MOVE_EYE;
            else if (eye == 2) action = ZOOM;
            break;
        case GLUT_RIGHT_BUTTON:
            action = ZOOM;
            break;
        default:
            break;
    }
 
    /* Update the saved mouse position */
    xStart = x;
    yStart = y;
}
 
static void motion(int x, int y) {
 
    switch (action) {
        case MOVE_EYE:
            /* Adjust the eye position based on the mouse position */
            azimAngle += (float) (x - xStart);
            incAngle  -= (float) (y - yStart);
            break;
        case TWIST_EYE:
            /* Adjust the eye twist based on the mouse position */
            //twistAngle = mod(twistAngle+(x - xStart), 360.0);
            twistAngle = (float) (twistAngle + (int) (x - xStart) % 360);
            break;
        case ZOOM:
            /* Adjust the eye distance based on the mouse position */
            distance -= (float) (10.0 * (y - yStart) / width);
            break;
        default:
            break;
    }
 
    /* Update the stored mouse position for later use */
    xStart = x;
    yStart = y;
 
    glutPostRedisplay();
}
 
static void flythru1(float distance, float azimuth, float incidence, float twist) {
    glTranslatef(0.0f, 0.0f, -distance);
    glRotatef(-twist,     0.0f, 0.0f, 1.0f);
    glRotatef(-incidence, 1.0f, 0.0f, 0.0f);
    glRotatef(-azimuth,   0.0f, 1.0f, 0.0f);
}
 
static void polygon(int a, int b, int c, int d) {
 
    /* draw a polygon via list of vertices */
    glBegin(GL_POLYGON);
    glColor3fv(colors[a]);
    glNormal3fv(normals[a]);
    glVertex3fv(vertices[a]);
 
    glColor3fv(colors[b]);
    glNormal3fv(normals[b]);
    glVertex3fv(vertices[b]);
 
    glColor3fv(colors[c]);
    glNormal3fv(normals[c]);
    glVertex3fv(vertices[c]);
 
    glColor3fv(colors[d]);
    glNormal3fv(normals[d]);
    glVertex3fv(vertices[d]);
    glEnd();
}


static void colorcube() {

    
    // Bus body
    glColor3f(1.0f, 0.8f, 0.0f); // Yellow color
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(2.0f, 1.0f, 1.0f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the main body of the bus
    glPopMatrix();
    
    // Draw the "RECORD" text along the right side of the bus
    glPushMatrix();
    glColor3f(0.5f, 0.2f, 0.3f); // Pink color
    glTranslatef(-0.8f, -0.3f, 0.55f); // Adjust the position as needed
    glScalef(0.002f, 0.002f, 0.002f); // Adjust the scale as needed
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'R');
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'E');
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'C');
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'O');
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'R');
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'D');
    glPopMatrix();
    
    
    // Front of the bus
    glPushMatrix();
    glColor3f(1.0f, 0.8f, 0.0f); // Yellow color
    glTranslatef(1.0f, -0.2f, 0.0f); // Adjust position as needed
    glScalef(0.5f, 0.5f, 1.0f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the front of the bus
    glPopMatrix();
    
    // Windshield
    glColor3f(0.02f, 0.02f, 0.5f); // Dark blue
    glPushMatrix();
    glTranslatef(1.0f, 0.2f, 0.0f); // Adjust position as needed
    glScalef(0.1f, 0.5f, 0.98f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the windshield
    glPopMatrix();
    
    // Square wheels - Left side
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray
    glPushMatrix();
    glTranslatef(-1.0f, -0.5f, -0.6f); // Adjust position as needed
    glutSolidCube(0.2); // Create the first square wheel on the left side
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, -0.5f, 0.6f); // Adjust position as needed
    glutSolidCube(0.2); // Create the second square wheel on the left side
    glPopMatrix();

    // Square wheels - Right side
    glPushMatrix();
    glTranslatef(1.0f, -0.5f, -0.6f); // Adjust position as needed
    glutSolidCube(0.2); // Create the third square wheel on the right side
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0f, -0.5f, 0.6f); // Adjust position as needed
    glutSolidCube(0.2); // Create the fourth square wheel on the right side
    glPopMatrix();
    
    // Windows
    glColor3f(0.02f, 0.02f, 0.5f);

    // Right side windows
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.5f); // Adjust position as needed
    glScalef(1.7f, 0.35f, 0.05f); // Adjust dimensions
    glutSolidCube(1.0); // Create the first right side window
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.2f, -0.5f); // Adjust position as needed
    glScalef(1.7f, 0.35f, 0.05f); // Adjust dimensions
    glutSolidCube(1.0); // Create the second right side window
    glPopMatrix();

    // Headlights
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glPushMatrix();
    glTranslatef(1.2f, -0.2f, 0.25f); // Adjust position as needed
    glScalef(0.2f, 0.1f, 0.1f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the first headlight
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.2f, -0.2f, -0.25f); // Adjust position as needed
    glScalef(0.2f, 0.1f, 0.1f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the second headlight
    glPopMatrix();
    
    // Tail lights
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glPushMatrix();
    glTranslatef(-1.0f, -0.2f, 0.25f); // Adjust position as needed
    glScalef(0.1f, 0.1f, 0.1f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the first tail light
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0f, -0.2f, -0.25f); // Adjust position as needed
    glScalef(0.1f, 0.1f, 0.1f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the second tail light
    glPopMatrix();
    
    // License plate
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glPushMatrix();
    glTranslatef(-1.0f, -0.3f, 0.0f); // Adjust position as needed
    glScalef(0.1f, 0.1f, 0.2f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the license plate
    glPopMatrix();
    
    // Rear window
    glColor3f(0.02f, 0.02f, 0.5f); // Dark blue
    glPushMatrix();
    glTranslatef(-1.0f, 0.2f, 0.0f); // Adjust position as needed
    glScalef(0.05f, 0.35f, 0.98f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the rear window
    glPopMatrix();
    
    // Gray door
    glColor3f(0.05f, 0.05f, 0.05f); // Dark gray
    glPushMatrix();
    glTranslatef(0.62f, -0.05f, 0.55f); // Adjust position as needed
    glScalef(0.45f, 0.88f, 0.05f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the door
    glPopMatrix();
    
    // Red Stop Sign
    glColor3f(1.0f, 0.0f, 0.0f); // Dark gray
    glPushMatrix();
    glTranslatef(0.62f, -0.15f, -0.50f); // Adjust position as needed
    glScalef(0.20f, 0.20f, 0.05f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the door
    glPopMatrix();
    
    // TEXTURE ON THE BUS
    //glColor3f(1.0f, 0.65f, 0.0f); // Yellow color
    glColor3f(1.0f, 0.9f, 0.0f);
    glPushMatrix();
    //glTranslatef(1.f, -0.12f, -0.50f); // Adjust position as needed
    //glScalef(0.20f, 0.10f, 0.02f); // Adjust dimensions as needed
    glScalef(2.0f, 1.0f, 1.0f);
    glutSolidCube(1.000001); // Create the door
    glPopMatrix();
    
    
    // TEXTURE ON THE BUS
    glPushMatrix();
    glColor3f(1.0f, 0.9f, 0.0f);
    glTranslatef(1.0f, -0.2f, 0.0f); // Adjust position as needed
    glScalef(0.5f, 0.5f, 1.0f); // Adjust dimensions as needed
    glutSolidCube(1.000001); // Create the door
    glPopMatrix();
    
    
    // TEXTURE ON THE BUS
    //glColor3f(0.8f, 0.6f, 0.1f);
    glColor3f(0.8f, 0.7f, 0.0f);
    glPushMatrix();
    //glTranslatef(1.f, -0.12f, -0.50f); // Adjust position as needed
    //glScalef(0.20f, 0.10f, 0.02f); // Adjust dimensions as needed
    glScalef(2.0f, 1.0f, 1.0f);
    glutSolidCube(1.000002); // Create the door
    glPopMatrix();
    
    // TEXTURE ON THE BUS
    glPushMatrix();
    glColor3f(0.8f, 0.7f, 0.0f);
    glTranslatef(1.0f, -0.2f, 0.0f); // Adjust position as needed
    glScalef(0.5f, 0.5f, 1.0f); // Adjust dimensions as needed
    glutSolidCube(1.000003); // Create the door
    glPopMatrix();

    
    
    
    // TEXTURE ON THE BUS
    //glColor3f(0.8f, 0.6f , 0.1f);
    glColor3f(0.8f, 0.7f, 0.0f);
    glPushMatrix();
    //glTranslatef(1.f, -0.12f, -0.50f); // Adjust position as needed
    //glScalef(0.20f, 0.10f, 0.02f); // Adjust dimensions as needed
    glScalef(2.0f, 1.0f, 1.0f);
    glutSolidCube(1.000002); // Create the door
    glPopMatrix();
    
    // TEXTURE ON THE BUS
    glPushMatrix();
    glColor3f(0.8f, 0.7f, 0.0f);
    glTranslatef(1.0f, -0.2f, 0.0f); // Adjust position as needed
    glScalef(0.5f, 0.5f, 1.0f); // Adjust dimensions as needed
    glutSolidCube(1.000003); // Create the door
    glPopMatrix();

    
 
}


 
static void drawScene() {
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glPushMatrix();
 
    /* set up viewing transformation */
    flythru1(distance, azimAngle, incAngle, twistAngle);
 
    /* draw cube */
    colorcube();
 
    glPopMatrix();
 
    glutSwapBuffers();
}

 
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    /* create a window that is 1/4 the size of the screen,
    * and position it in the middle of the screen.
    */
    width = glutGet(GLUT_SCREEN_WIDTH);
    height = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowPosition(width / 4, height / 4);
    glutInitWindowSize(width / 2, height / 2);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow(progname);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialkeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(drawScene);
    printHelp();
    initgfx();
    glutMainLoop();
   
    
    return 0;
}

