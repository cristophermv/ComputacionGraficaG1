#ifndef BARRIL_H
#define BARRIL_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

// Función para dibujar un círculo en el plano XZ
void drawCircleBarril(float radius, float y, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, y, 0.0f); // Centro del círculo

    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, y, z);
    }
    glEnd();
}

// Función para dibujar el cuerpo del barril con forma curvada
void drawBarrilBody() {
    int segments = 32;
    int rings = 20;
    float height = 2.0f;
    float baseRadius = 0.6f;
    float midRadius = 0.8f;

    glColor3f(0.6f, 0.4f, 0.2f); // Color marrón para la madera

    // Dibujar el cuerpo del barril con curva
    for (int i = 0; i < rings; i++) {
        float y1 = -height/2 + (height * i) / rings;
        float y2 = -height/2 + (height * (i + 1)) / rings;

        // Crear la curva del barril (más ancho en el medio)
        float t1 = (float)i / rings;
        float t2 = (float)(i + 1) / rings;
// Función de curva para dar forma de barril
        float curve1 = 1.0f - 4.0f * (t1 - 0.5f) * (t1 - 0.5f);
        float curve2 = 1.0f - 4.0f * (t2 - 0.5f) * (t2 - 0.5f);

        float r1 = baseRadius + (midRadius - baseRadius) * curve1;
        float r2 = baseRadius + (midRadius - baseRadius) * curve2;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= segments; j++) {
            float angle = 2.0f * M_PI * j / segments;
            float x = cos(angle);
            float z = sin(angle);

            // Normal para iluminación
            glNormal3f(x, 0.0f, z);

            glVertex3f(r1 * x, y1, r1 * z);
            glVertex3f(r2 * x, y2, r2 * z);
        }
        glEnd();
    }
}

// Función para dibujar las bandas metálicas del barril
void drawBarrilBands() {
    glColor3f(0.4f, 0.4f, 0.4f); // Color gris metálico

    float height = 2.0f;
    float bandWidth = 0.1f;
    float baseRadius = 0.6f;
    float midRadius = 0.8f;
    int segments = 32;

    // Dibujar 3 bandas metálicas
    float bandPositions[] = {-0.7f, 0.0f, 0.7f};

    for (int band = 0; band < 3; band++) {
        float y = bandPositions[band];
        float t = (y + height/2) / height;
        float curve = 1.0f - 4.0f * (t - 0.5f) * (t - 0.5f);
        float radius = baseRadius + (midRadius - baseRadius) * curve;

        // Banda superior
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * M_PI * i / segments;
            float x = (radius + 0.02f) * cos(angle);
            float z = (radius + 0.02f) * sin(angle);

            glVertex3f(x, y + bandWidth/2, z);
            glVertex3f(x, y - bandWidth/2, z);
        }
        glEnd();
    }
}
// Función para dibujar las tapas del barril
void drawBarrilCaps() {
    glColor3f(0.5f, 0.3f, 0.1f); // Color más oscuro para las tapas

    float baseRadius = 0.6f;

    // Tapa superior
    drawCircleBarril(baseRadius, 1.0f, 32);

    // Tapa inferior
    drawCircleBarril(baseRadius, -1.0f, 32);
}

// Función principal para dibujar el barril completo
void DrawBarril() {
    glPushMatrix();

    // Dibujar componentes del barril
    drawBarrilBody();
    drawBarrilBands();
    drawBarrilCaps();

    glPopMatrix();
}

#endif // BARRIL_H
