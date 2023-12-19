// Rachel Collier
// Bus Project 1

#include <GLUT/glut.h>
#include <string>
 
const int KEY_ESC = 27;    /* ascii value for the escape key */
const int MOVE_EYE = 0;
const int TWIST_EYE = 1;
const int ZOOM = 2;
const int MOVE_NONE = 3;
 
/* Global Variables */
 
const char* progname = "Bus";
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
 
/*  Moving around in space
    *    Left Mouse Button, eye=true     - change incidence and azimuth angles
    *    Left Mouse Button, eye=false - change the twist angle based on
    *                  horizontal mouse movement
    *    Right Mouse Button    - zoom in and out based on vertical
    *                  mouse movement
    *    <R> Key            - reset viewpoint
    *   'e'            - toggle "eye" off/on
    *    Escape Key        - exit program
    */
 
static void printHelp() {
    printf("\n%s - Move around in space\n\n"
        "F1 key        - print help information\n"
        "Left Mousebutton    - move eye position\n"
        "Middle Mousebutton    - change twist angle\n"
        "Right Mousebutton    - move up / down to zoom in / out\n"
        "<R> Key        - reset viewpoint\n"
        "Escape Key    - exit the program\n\n", progname);
}
 
static void resetView() {
    distance = (float) nearClip + (float) ((farClip - nearClip) / 2.0);
    /* rotation of viewing volume (camera) */
    twistAngle = 0.0f;
    incAngle = 0.0f;
    azimAngle = 0.0f;
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
        float r = (float) (rand() % 32768) / 32768;
        float g = (float) (rand() % 32768) / 32768;
        float b = (float) (rand() % 32768) / 32768;
        glClearColor(r, g, b, 1.0f);
        glutPostRedisplay();
    }
    else if (key == 's') {
        /* toggle between smooth and flat shading */
        flat = flat ? 0 : 1;
        glShadeModel(flat ? GL_FLAT : GL_SMOOTH);
        glutPostRedisplay();
    }
    else if (key == 'r') {
        resetView();
        glutPostRedisplay();
    }
    else if (key == KEY_ESC) {
        exit(0);
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

    // Bus body (yellow)
    glColor3f(1.0f, 0.9f, 0.0f); // Yellow color
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(2.0f, 1.0f, 1.0f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the main body of the bus
    glPopMatrix();

    // Front of the bus (yellow)
    glPushMatrix();
    glColor3f(1.0f, 0.9f, 0.0f); // Yellow color
    glTranslatef(1.0f, -0.2f, 0.0f); // Adjust position as needed
    glScalef(0.5f, 0.5f, 1.0f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the front of the bus
    glPopMatrix();

    // Windshield
    glColor3f(0.5f, 0.7f, 0.9f); // Dark blue color (same as wheels)
    glPushMatrix();
    glTranslatef(1.0f, 0.2f, 0.0f); // Adjust position as needed
    glScalef(0.1f, 0.5f, 0.98f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the windshield
    glPopMatrix();
    
    // Square wheels - Left side
    glColor3f(0.2f, 0.2f, 0.2f); // Dark blue color
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
    glColor3f(0.5f, 0.7f, 0.9f);

    // Right side windows
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.5f); // Adjust position as needed
    glScalef(1.7f, 0.35f, 0.05f); // Adjust dimensions to match the windshield
    glutSolidCube(1.0); // Create the first right side window
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.2f, -0.5f); // Adjust position as needed
    glScalef(1.7f, 0.35f, 0.05f); // Adjust dimensions to match the windshield
    glutSolidCube(1.0); // Create the second right side window
    glPopMatrix();

    // Headlights (white)
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
    
    // Tail lights (red)
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
    
    // License plate (black)
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glPushMatrix();
    glTranslatef(-1.0f, -0.3f, 0.0f); // Adjust position as needed
    glScalef(0.1f, 0.1f, 0.2f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the license plate
    glPopMatrix();
    
    // Rear window
    glColor3f(0.5f, 0.7f, 0.9f); // blue color
    glPushMatrix();
    glTranslatef(-1.0f, 0.2f, 0.0f); // Adjust position as needed
    glScalef(0.05f, 0.35f, 0.98f); // Adjust dimensions as needed
    glutSolidCube(1.0); // Create the rear window
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

