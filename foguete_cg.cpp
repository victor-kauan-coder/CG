// This preprocessor directive checks if the code is being compiled on a macOS system.
#ifdef __APPLE__ // For macOS
    #define GL_SILENCE_DEPRECATION // Silences warnings about deprecated OpenGL functions on Mac.
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else // For Windows and Linux
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
    // The windows.h header is generally not necessary for basic GLUT/OpenGL applications,
    // but it's kept here as it was in the original code.
    #include <windows.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "circulo.h" // Assumes this header contains a function to draw circles, like drawCircle().

// Define the ASCII value for the ESC key for better readability.
#define ESC 27

using namespace std;

// --- Global Variables ---

// Vectors to store the coordinates of the rockets' paths (trails).
vector<pair<float, float>> rocket1_path;
vector<pair<float, float>> rocket2_path;

// --- Color Definitions (in RGB format) ---

// Colors for the rocket trails.
GLfloat path1_color[3] = {1.0, 0.0, 1.0}; // Pink
GLfloat path2_color[3] = {1.0, 0.0, 0.0}; // Red

// Color for the stars.
GLfloat star_color[3] = {0.988, 0.988, 0.063}; // Yellow

// Colors for the moon (primary body color and crater color).
GLfloat moon_colors[2][3] = {
    {0.870, 0.851, 0.765}, // Primary moon color
    {0.772, 0.741, 0.59}  // Secondary color for craters
};

// Color palette for the first rocket.
GLfloat rocket1_colors[6][3] = {
    {1.0, 0.75, 0.80},  // Light Pink (for flame)
    {1.0, 0.43, 0.71},  // Shocking Pink (for flame)
    {0.86, 0.44, 0.58}, // Medium Pink (for window)
    {0.74, 0.20, 0.64}, // Rosy Purple (for window)
    {1.0, 0.63, 0.63},  // Peach Pink (for body)
    {0.94, 0.0, 0.5}    // Strong Pink (for fins)
};

// Color palette for the second rocket.
GLfloat rocket2_colors[6][3] = {
    {1.0, 1.0, 0},           // Yellow
    {1.0, 0.557, 0.004},     // Orange
    {0.3098, 0.5059, 0.7373},// Blue
    {0.106, 0.2, 0.309},     // Dark Blue
    {0.749, 0.749, 0.749},   // Gray
    {1.0, 0, 0}              // Red
};

// --- Animation and Position Parameters ---

float rocket2_scale = 0.8f;      // Scale of the second rocket.
float time_param_1 = 0.0f;       // Parameter 't' for the first rocket's heart curve equation.
float time_step = 0.02f;         // Increment step for the time parameter, controls animation speed.
float path_scale = 10.0f;        // Scalar to adjust the overall size of the path.
float path_centerX = 400.0f;     // X-coordinate for the center of the path.
float path_centerY = 200.0f;     // Y-coordinate for the center of the path.
float time_param_2 = 2 * M_PI;   // Parameter 't' for the second rocket's heart curve equation.

// Initial positions and angle for the first rocket. These will be updated by the 'update' function.
GLfloat rocket1_posX = 420.0f;
GLfloat rocket1_posY = 20.0f;
GLfloat rocket1_angle = 45.0f;

// Initial positions and angle for the second rocket.
GLfloat rocket2_posX = 420.0f;
GLfloat rocket2_posY = 20.0f;
GLfloat rocket2_angle = 45.0f;

// --- Function Prototypes ---
void initialize(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void update(int value);
void display(void);
void drawPath(vector<pair<float, float>> path, GLfloat color[3]);

// Represents a single star in the scene.
class Star {
private:
    GLfloat vertices[3][2]; // Vertices for one triangle of the star.
    GLfloat x, y, scale;    // Position and scale.
    GLfloat color[3];       // Star's color.

public:
    // Constructor to initialize the star's properties.
    Star(GLfloat posX, GLfloat posY, GLfloat scale, GLfloat star_color[3]) {
        x = posX;
        y = posY;
        this->scale = scale;
        for (int i = 0; i < 3; i++) this->color[i] = star_color[i];

        // Define the vertices for an upward-pointing triangle.
        vertices[0][0] = 0;    vertices[0][1] = 100;
        vertices[1][0] = -100; vertices[1][1] = -50;
        vertices[2][0] = 100;  vertices[2][1] = -50;
    }

    // Renders the star on the screen.
    void draw() const {
        glColor3fv(color);

        // Draw the first (upward) triangle.
        glPushMatrix();
            glTranslatef(x, y, 0);
            glScalef(scale, scale, 1.0);
            glBegin(GL_TRIANGLES);
                glVertex2fv(vertices[0]);
                glVertex2fv(vertices[1]);
                glVertex2fv(vertices[2]);
            glEnd();
        glPopMatrix();

        // Draw the second (downward) triangle by rotating 180 degrees.
        glPushMatrix();
            glTranslatef(x, y, 0);
            glRotatef(180, 0, 0, 1);
            glScalef(scale, scale, 1.0);
            glBegin(GL_TRIANGLES);
                glVertex2fv(vertices[0]);
                glVertex2fv(vertices[1]);
                glVertex2fv(vertices[2]);
            glEnd();
        glPopMatrix();
    }
};

// Represents the moon in the scene.
class Moon {
private:
    GLfloat x, y, radius;             // Position and radius.
    GLfloat primary_color[3];         // Main color of the moon.
    GLfloat secondary_color[3];       // Color for the craters.

public:
    // Constructor to initialize the moon's properties.
    Moon(GLfloat posX, GLfloat posY, GLfloat moonRadius, GLfloat moon_colors[2][3]) {
        x = posX;
        y = posY;
        radius = moonRadius;
        for (int i = 0; i < 3; i++) primary_color[i] = moon_colors[0][i];
        for (int i = 0; i < 3; i++) secondary_color[i] = moon_colors[1][i];
    }

    // Renders the moon with its craters.
    void draw() const {
        // NOTE: Assumes a function `drawCircle(radius, segments, isFilled)` exists in "circulo.h".
        glPushMatrix();
            glTranslatef(x, y, 0);

            // Draw the main body of the moon.
            glColor3fv(primary_color);
            drawCircle(radius, 360, true);

            // Set color for the craters.
            glColor3fv(secondary_color);

            // Draw Crater 1.
            glPushMatrix();
                glTranslatef(radius * 0.5f, radius * 0.2f, 0);
                drawCircle(radius / 3.5, 360, true);
            glPopMatrix();

            // Draw Crater 2.
            glPushMatrix();
                glTranslatef(radius * -0.4f, radius * 0.5f, 0);
                drawCircle(radius / 3.5, 360, true);
            glPopMatrix();

            // Draw Crater 3.
            glPushMatrix();
                glTranslatef(radius * -0.2f, radius * -0.6f, 0);
                drawCircle(radius / 3.5, 360, true);
            glPopMatrix();
        glPopMatrix();
    }
};

// Represents a rocket.
class Rocket {
private:
    GLfloat x, y, scale, angle;         // Position, scale, and angle of inclination.
    GLfloat flame_color_primary[3];
    GLfloat flame_color_secondary[3];
    GLfloat window_color_primary[3];
    GLfloat window_color_secondary[3];
    GLfloat body_color[3];
    GLfloat fin_color[3];

public:
    // Constructor to initialize the rocket's state and colors.
    Rocket(GLfloat posX, GLfloat posY, GLfloat initial_angle, GLfloat scale, GLfloat rocket_colors[6][3]) {
        x = posX;
        y = posY;
        this->scale = scale;
        angle = initial_angle;
        // Assign colors from the provided color palette.
        for (int i = 0; i < 3; i++) flame_color_primary[i] = rocket_colors[0][i];
        for (int i = 0; i < 3; i++) flame_color_secondary[i] = rocket_colors[1][i];
        for (int i = 0; i < 3; i++) window_color_primary[i] = rocket_colors[2][i];
        for (int i = 0; i < 3; i++) window_color_secondary[i] = rocket_colors[3][i];
        for (int i = 0; i < 3; i++) body_color[i] = rocket_colors[4][i];
        for (int i = 0; i < 3; i++) fin_color[i] = rocket_colors[5][i];
    }

    // Renders the entire rocket.
    void draw() const {
        // Isolate transformations for this rocket.
        glPushMatrix();
            // Apply transformations: move, scale, and rotate.
            glTranslatef(x, y, 0);
            glScalef(scale, scale, 0);
            glRotatef(angle, 0, 0, 1);

            // --- Draw Rocket Body ---
            glColor3fv(body_color);
            glBegin(GL_QUADS);
                glVertex2f(0, 0);
                glVertex2f(40, 0);
                glVertex2f(40, 90);
                glVertex2f(0, 90);
            glEnd();
            
            // --- Draw Top Cone (as a triangle) ---
            glColor3fv(fin_color); // Using fin color for the cone
            glBegin(GL_TRIANGLES);
                glVertex2f(0, 90);
                glVertex2f(40, 90);
                glVertex2f(20, 120); // Tip of the cone
            glEnd();

            // --- Draw Bottom Part ---
            glColor3fv(window_color_primary);
            glBegin(GL_QUADS);
                glVertex2f(0, 0);
                glVertex2f(40, 0);
                glVertex2f(40, 8);
                glVertex2f(0, 8);
            glEnd();

            // --- Draw Fins ---
            glColor3fv(fin_color);
            // Left Fin
            glPushMatrix();
                glBegin(GL_TRIANGLES);
                    glVertex2f(0, 20);
                    glVertex2f(0, 0);
                    glVertex2f(-25, 10);
                glEnd();
            glPopMatrix();
            // Right Fin
            glPushMatrix();
                glBegin(GL_TRIANGLES);
                    glVertex2f(40, 20);
                    glVertex2f(40, 0);
                    glVertex2f(65, 10);
                glEnd();
            glPopMatrix();

            // --- Draw Window ---
            glPushMatrix();
                glTranslatef(20, 67, 0);
                glColor3fv(window_color_secondary);
                drawCircle(15, 360, true); // Outer ring
                glColor3fv(window_color_primary);
                drawCircle(12, 360, true); // Inner glass
            glPopMatrix();

            // --- Draw Exhaust Flames ---
            // Outer flame
            glPushMatrix();
                glTranslatef(20, -25, 0); // Position below the rocket
                glScalef(0.8, 1.5, 0);
                glColor3fv(flame_color_secondary);
                glBegin(GL_TRIANGLES);
                    glVertex2f(0, 0);
                    glVertex2f(-20, 0);
                    glVertex2f(0, -35); // Longest flame part
                glEnd();
                glBegin(GL_TRIANGLES);
                    glVertex2f(0, 0);
                    glVertex2f(20, 0);
                    glVertex2f(0, -35);
                glEnd();
            glPopMatrix();
            // Inner flame
            glPushMatrix();
                glTranslatef(20, -15, 0);
                glScalef(0.6, 1.0, 0);
                glColor3fv(flame_color_primary);
                glBegin(GL_TRIANGLES);
                    glVertex2f(0, 0);
                    glVertex2f(-15, 0);
                    glVertex2f(0, -25);
                glEnd();
                glBegin(GL_TRIANGLES);
                    glVertex2f(0, 0);
                    glVertex2f(15, 0);
                    glVertex2f(0, -25);
                glEnd();
            glPopMatrix();

        // Restore the previous matrix state.
        glPopMatrix();
    }
};

// Main function - entry point of the application.
int main(int argc, char** argv) {
    glutInit(&argc, argv);                                  // Initialize GLUT.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);            // Set display mode: single buffer and RGB color.
    glutInitWindowSize(800, 400);                           // Set the window's initial width and height.
    glutInitWindowPosition(100, 100);                       // Set the window's initial position.
    glutCreateWindow("Rocket Animation - CG Project"); // Create a window with the given title.

    initialize();                               // Call the initialization function.
    glutReshapeFunc(reshape);                   // Register the reshape callback function.
    glutDisplayFunc(display);                   // Register the display callback function.
    glutKeyboardFunc(keyboard);                 // Register the keyboard callback function.
    glutTimerFunc(16, update, 0);               // Set a timer to call the 'update' function every ~16ms (~60 FPS).
    
    glutMainLoop();                             // Enter the GLUT event processing loop.
    return EXIT_SUCCESS;
}

// This function is called periodically to update the animation state.
void update(int value) {
    // --- Rocket 1 Movement (Heart Curve) ---
    // Parametric equation for a heart shape.
    float x1 = 16 * pow(sin(time_param_1), 3);
    float y1 = 13 * cos(time_param_1) - 5 * cos(2 * time_param_1) - 2 * cos(3 * time_param_1) - cos(4 * time_param_1);

    // --- Rocket 2 Movement (Heart Curve, opposite direction) ---
    float x2 = 16 * pow(sin(time_param_2), 3);
    float y2 = 13 * cos(time_param_2) - 5 * cos(2 * time_param_2) - 2 * cos(3 * time_param_2) - cos(4 * time_param_2);

    // Update rocket positions by scaling and translating the curve to the center of the path.
    rocket1_posX = path_centerX + path_scale * x1;
    rocket1_posY = path_centerY + path_scale * y1;
    rocket2_posX = path_centerX + path_scale * x2;
    rocket2_posY = path_centerY + path_scale * y2;

    // --- Calculate Rocket Orientation ---
    // To make the rockets point in their direction of travel, we calculate the derivative (tangent) of the path.
    // Derivative of the x-component of the heart equation w.r.t. the parameter 't'.
    float dx1 = 48 * pow(sin(time_param_1), 2) * cos(time_param_1);
    // Derivative of the y-component.
    float dy1 = -13 * sin(time_param_1) + 10 * sin(2 * time_param_1) + 6 * sin(3 * time_param_1) + 4 * sin(4 * time_param_1);
    
    float dx2 = 48 * pow(sin(time_param_2), 2) * cos(time_param_2);
    float dy2 = -13 * sin(time_param_2) + 10 * sin(2 * time_param_2) + 6 * sin(3 * time_param_2) + 4 * sin(4 * time_param_2);

    // Calculate the angle of the tangent vector using atan2 and convert it from radians to degrees.
    rocket1_angle = atan2(dy1, dx1) * 180.0f / M_PI - 90; // Adjust by -90 because the rocket is drawn facing up.
    rocket2_angle = atan2(dy2, dx2) * 180.0f / M_PI - 90;

    // Store the current position to draw the trail.
    rocket1_path.push_back({rocket1_posX, rocket1_posY});
    rocket2_path.push_back({rocket2_posX, rocket2_posY});

    // Increment/decrement the time parameters to move the rockets along the path.
    time_param_1 += time_step;
    time_param_2 -= time_step;

    // --- Loop the Animation ---
    // If rocket 1 completes its path, reset it.
    if (time_param_1 > 2 * M_PI) {
        time_param_1 = 0.0f;
        rocket1_path.clear(); // Clear the trail for the new lap.
    }
    // If rocket 2 completes its path, reset it.
    if (time_param_2 < 0) {
        time_param_2 = 2 * M_PI;
        rocket2_path.clear();
    }
    
    glutPostRedisplay(); // Request a redraw of the window.
    glutTimerFunc(16, update, 0); // Re-register the timer for the next frame.
}

// Initialization function.
void initialize(void) {
    glClearColor(0.090, 0.116, 0.269, 1.0); // Set the background color to a dark blue.
}

// Callback function for when the window is resized.
void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION); // Switch to the Projection matrix.
    glLoadIdentity();            // Reset the matrix.
    glViewport(0, 0, w, h);      // Set the viewport to be the entire window.

    // Set up a 2D orthographic projection.
    // The world coordinates will range from (0,0) at the bottom-left to (800,400) at the top-right.
    glOrtho(0, 800, 0, 400, -1, 1);
    glMatrixMode(GL_MODELVIEW);  // Switch back to the Modelview matrix.
}

// Callback function for handling standard keyboard presses.
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ESC:               // If the ESC key is pressed...
            exit(EXIT_SUCCESS); // ...exit the application.
            break;
    }
}

// Draws the path (trail) of a rocket.
void drawPath(vector<pair<float, float>> path, GLfloat color[3]) {
    glColor3fv(color);       // Set the color for the trail.
    glPointSize(3.0f);       // Set the size of the points.

    glBegin(GL_POINTS);
    for (int i = 0; i < path.size(); i++) {
        glVertex2f(path[i].first, path[i].second);
    }
    glEnd();
}

// The main display callback function. This is where all the rendering happens.
void display(void) {
    // Clear the color buffer to the background color set in initialize().
    glClear(GL_COLOR_BUFFER_BIT);
    // Reset the modelview matrix for this frame.
    glLoadIdentity();

    // --- Draw Scenery ---
    Star star1(300, 100, 0.15, star_color);
    star1.draw();
    Star star2(480, 280, 0.25, star_color);
    star2.draw();
    Star star3(560, 160, 0.15, star_color);
    star3.draw();
    Star star4(680, 70, 0.25, star_color);
    star4.draw();
    Star star5(90, 340, 0.25, star_color);
    star5.draw();
    Moon moon(700, 300, 70, moon_colors);
    moon.draw();

    // --- Draw Rocket Trails ---
    drawPath(rocket1_path, path1_color);
    drawPath(rocket2_path, path2_color);

    // --- Draw Rockets ---
    Rocket rocket1(rocket1_posX, rocket1_posY, rocket1_angle, 0.8, rocket1_colors);
    rocket1.draw();
    Rocket rocket2(rocket2_posX, rocket2_posY, rocket2_angle, rocket2_scale, rocket2_colors);
    rocket2.draw();

    // Execute all the buffered OpenGL commands.
    glFlush();
}