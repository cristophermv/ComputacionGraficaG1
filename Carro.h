#ifndef CARRO_H
#define CARRO_H

#include <GL/glut.h>



// Función para dibujar la carrocería del carro
void DrawCarBody() {
    // Color amarillo brillante para la carrocería
    glColor3f(1.0f, 0.9f, 0.1f);
    
    glPushMatrix();
    
    // Carrocería principal (parte baja)
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.0f);
    glScalef(2.0f, 0.6f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Cabina (parte alta)
    glPushMatrix();
    glTranslatef(0.2f, 0.9f, 0.0f);
    glScalef(1.2f, 0.6f, 0.9f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Capó frontal
    glPushMatrix();
    glTranslatef(-0.8f, 0.45f, 0.0f);
    glScalef(0.4f, 0.3f, 0.9f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

// Función para dibujar las ventanas del carro
void DrawCarWindows() {
    // Color azul oscuro para las ventanas
    glColor3f(0.1f, 0.2f, 0.4f);
    
    glPushMatrix();
    
    // Parabrisas frontal
    glPushMatrix();
    glTranslatef(-0.1f, 0.9f, 0.46f);
    glScalef(0.6f, 0.5f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Parabrisas trasero
    glPushMatrix();
    glTranslatef(0.5f, 0.9f, 0.46f);
    glScalef(0.6f, 0.5f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Ventana frontal (otro lado)
    glPushMatrix();
    glTranslatef(-0.1f, 0.9f, -0.46f);
    glScalef(0.6f, 0.5f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Ventana trasera (otro lado)
    glPushMatrix();
    glTranslatef(0.5f, 0.9f, -0.46f);
    glScalef(0.6f, 0.5f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Ventanas laterales
    glPushMatrix();
    glTranslatef(0.2f, 0.9f, 0.46f);
    glScalef(0.02f, 0.5f, 0.6f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, 0.9f, -0.46f);
    glScalef(0.02f, 0.5f, 0.6f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

// Función para dibujar las ruedas del carro
void DrawCarWheels() {
    // Color negro para las ruedas
    glColor3f(0.05f, 0.05f, 0.05f);
    
    glPushMatrix();
    
    // Rueda delantera izquierda
    glPushMatrix();
    glTranslatef(-0.6f, 0.0f, 0.6f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glutSolidTorus(0.1, 0.25, 8, 16);
    glPopMatrix();
    
    // Rueda delantera derecha
    glPushMatrix();
    glTranslatef(-0.6f, 0.0f, -0.6f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glutSolidTorus(0.1, 0.25, 8, 16);
    glPopMatrix();
    
    // Rueda trasera izquierda
    glPushMatrix();
    glTranslatef(0.6f, 0.0f, 0.6f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glutSolidTorus(0.1, 0.25, 8, 16);
    glPopMatrix();
    
    // Rueda trasera derecha
    glPushMatrix();
    glTranslatef(0.6f, 0.0f, -0.6f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glutSolidTorus(0.1, 0.25, 8, 16);
    glPopMatrix();
    
    glPopMatrix();
}
// Función principal para dibujar el carro completo
void DrawCarro() {
    glPushMatrix();
    
    // Escalar todo el carro para hacerlo más pequeño
    glScalef(0.7f, 0.7f, 0.7f);
    
    // Dibujar carrocería
    DrawCarBody();
    
    // Dibujar ventanas
    DrawCarWindows();
    
    // Dibujar ruedas
    DrawCarWheels();
    
    glPopMatrix();
}
#endif // CARRO_H
