#ifndef PELOTA_H
#define PELOTA_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// Función para configurar el material de la pelota
void setPelotaMaterial(float r, float g, float b) {
    GLfloat customColor[] = { r, g, b, 1.0f };
    GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, customColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0f);
}

// Función principal para dibujar la pelota (color celeste por defecto)
void DrawPelota() {
    glPushMatrix();

    glEnable(GL_NORMALIZE); // Asegura que las normales se escalen si hay escalado

    setPelotaMaterial(0.3f, 0.5f, 0.8f); // Color celeste

    glutSolidSphere(1.0, 50, 50);

    glPopMatrix();
}

// Pelota con radio personalizado (color celeste)
void DrawPelotaCustom(float radio) {
    glPushMatrix();

    glEnable(GL_NORMALIZE);
    setPelotaMaterial(0.3f, 0.5f, 0.8f);

    glutSolidSphere(radio, 50, 50);

    glPopMatrix();
}

// Pelota con color personalizado
void DrawPelotaColor(float r, float g, float b) {
    glPushMatrix();

    glEnable(GL_NORMALIZE);
    setPelotaMaterial(r, g, b);

    glutSolidSphere(1.0, 50, 50);

    glPopMatrix();
}

// Pelota con color y radio personalizados
void DrawPelotaCompleta(float radio, float r, float g, float b) {
    glPushMatrix();

    glEnable(GL_NORMALIZE);
    setPelotaMaterial(r, g, b);

    glutSolidSphere(radio, 50, 50);

    glPopMatrix();
}

#endif // PELOTA_H

