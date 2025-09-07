/*
 * Computer Graphics
 * Example Code: Library with a function to draw a circle in OpenGL
 * Original Author: Prof. Laurindo de Sousa Britto Neto
 * Refactored and Commented by: Gemini AI
 */

// Header guard to prevent multiple inclusions of this file,
// which would cause redefinition errors.
#ifndef CIRCLE_H
#define CIRCLE_H

#include <cmath>

// Define the mathematical constant PI if it's not already defined by the cmath header.
#ifndef M_PI
    #define M_PI 3.14159265f
#endif

/**
 * @brief Draws a circle centered at the origin (0,0) in the current model-view matrix.
 * * This function uses OpenGL's GL_TRIANGLE_FAN for a filled circle or GL_LINE_LOOP
 * for an outline. The smoothness of the circle is determined by the number of segments.
 * * @param radius The radius of the circle.
 * @param num_segments The number of line segments used to approximate the circle. 
 * More segments result in a smoother circle.
 * @param isFilled If true, the circle is drawn filled; otherwise, only its outline is drawn.
 */
void drawCircle(float radius, int num_segments, bool isFilled)
{
    // The angle increment for each segment in radians.
    // A full circle is 2*PI radians.
    GLfloat angle_increment = (2.0f * M_PI) / num_segments;

    // Choose the appropriate OpenGL primitive based on whether the circle should be filled.
    if (isFilled) {
        // GL_TRIANGLE_FAN creates a series of triangles that share a common central vertex.
        glBegin(GL_TRIANGLE_FAN);
        
        // The first vertex is the center of the fan (and our circle).
        glVertex2f(0.0f, 0.0f);
    }
    else {
        // GL_LINE_LOOP connects all vertices in order and then connects the last vertex back to the first.
        glBegin(GL_LINE_LOOP);
    }

    // Loop through each segment to calculate and draw its vertex on the circumference.
    for (int i = 0; i <= num_segments; i++) 
    {
        float current_angle = i * angle_increment;
        // Calculate the (x, y) coordinates for the vertex using parametric circle equations.
        // x = r * cos(theta)
        // y = r * sin(theta)
        // Note: The original code used a rotated version. This is the more standard orientation.
        glVertex2f(radius * cos(current_angle), radius * sin(current_angle));
    }
    
    // Finalize the drawing primitive.
    glEnd();
}

#endif /* CIRCLE_H */