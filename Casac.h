#ifndef CASAC_H
#define CASAC_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

void setColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

void drawCube(float width, float height, float depth) {
    glBegin(GL_QUADS);
    
    // Front face
    glVertex3f(-width/2, -height/2, depth/2);
    glVertex3f(width/2, -height/2, depth/2);
    glVertex3f(width/2, height/2, depth/2);
    glVertex3f(-width/2, height/2, depth/2);
    
    // Back face
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f(-width/2, height/2, -depth/2);
    glVertex3f(width/2, height/2, -depth/2);
    glVertex3f(width/2, -height/2, -depth/2);
    
    // Top face
    glVertex3f(-width/2, height/2, -depth/2);
    glVertex3f(-width/2, height/2, depth/2);
    glVertex3f(width/2, height/2, depth/2);
    glVertex3f(width/2, height/2, -depth/2);
    
    // Bottom face
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f(width/2, -height/2, -depth/2);
    glVertex3f(width/2, -height/2, depth/2);
    glVertex3f(-width/2, -height/2, depth/2);
    
    // Right face
    glVertex3f(width/2, -height/2, -depth/2);
    glVertex3f(width/2, height/2, -depth/2);
    glVertex3f(width/2, height/2, depth/2);
    glVertex3f(width/2, -height/2, depth/2);
    
    // Left face
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f(-width/2, -height/2, depth/2);
    glVertex3f(-width/2, height/2, depth/2);
    glVertex3f(-width/2, height/2, -depth/2);
    
    glEnd();
}

void drawWall(float width, float height) {
    glBegin(GL_QUADS);
    glVertex3f(-width/2, -height/2, 0);
    glVertex3f(width/2, -height/2, 0);
    glVertex3f(width/2, height/2, 0);
    glVertex3f(-width/2, height/2, 0);
    glEnd();
}

void drawDoor(float width, float height) {
    // Marco de la puerta (marrón oscuro)
    setColor(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0, 0, 0.02f);
    drawWall(width + 0.2f, height + 0.2f);
    glPopMatrix();
    
    // Puerta (marrón)
    setColor(0.6f, 0.4f, 0.2f);
    glPushMatrix();
    glTranslatef(0, 0, 0.04f);
    drawWall(width, height);
    glPopMatrix();
    
    // Manija de la puerta
    setColor(0.8f, 0.8f, 0.2f);
    glPushMatrix();
    glTranslatef(width/3, 0, 0.06f);
    glutSolidSphere(0.05f, 8, 8);
    glPopMatrix();
    
    // Líneas de la puerta
    setColor(0.3f, 0.2f, 0.1f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    // Línea vertical central
    glVertex3f(0, -height/2 + 0.1f, 0.05f);
    glVertex3f(0, height/2 - 0.1f, 0.05f);
    // Línea horizontal central
    glVertex3f(-width/2 + 0.1f, 0, 0.05f);
    glVertex3f(width/2 - 0.1f, 0, 0.05f);
    glEnd();
}

void drawWindow(float width, float height) {
    // Marco de la ventana (marrón)
    setColor(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0, 0, 0.02f);
    drawWall(width + 0.1f, height + 0.1f);
    glPopMatrix();
    
    // Cristal de la ventana (azul claro)
    setColor(0.7f, 0.9f, 1.0f);
    glPushMatrix();
    glTranslatef(0, 0, 0.04f);
    drawWall(width, height);
    glPopMatrix();
    
    // Divisiones de la ventana
    setColor(0.4f, 0.2f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // Líneas verticales
    for(int i = 1; i < 3; i++) {
        float x = -width/2 + (width/3) * i;
        glVertex3f(x, -height/2, 0.05f);
        glVertex3f(x, height/2, 0.05f);
    }
    // Líneas horizontales
    for(int i = 1; i < 3; i++) {
        float y = -height/2 + (height/3) * i;
        glVertex3f(-width/2, y, 0.05f);
        glVertex3f(width/2, y, 0.05f);
    }
    glEnd();
}

void drawRoof(float width, float depth, float height) {
    setColor(0.8f, 0.2f, 0.2f); // Rojo para el techo
    
    // Techo plano - solo una superficie superior
    glBegin(GL_QUADS);
    glVertex3f(-width/2, height, -depth/2);
    glVertex3f(-width/2, height, depth/2);
    glVertex3f(width/2, height, depth/2);
    glVertex3f(width/2, height, -depth/2);
    glEnd();
}

void DrawCasac() {
    glPushMatrix();
    
    // Pared principal (amarilla)
    setColor(0.9f, 0.8f, 0.3f);
    glPushMatrix();
    glTranslatef(0, 1.5f, 0);
    drawWall(4.0f, 3.0f);
    glPopMatrix();
    
    // Pared izquierda (verde)
    setColor(0.4f, 0.7f, 0.3f);
    glPushMatrix();
    glTranslatef(-2.0f, 1.5f, -1.5f);
    glRotatef(90, 0, 1, 0);
    drawWall(3.0f, 3.0f);
    glPopMatrix();
    
    // Pared derecha (rosa)
    setColor(0.9f, 0.5f, 0.7f);
    glPushMatrix();
    glTranslatef(2.0f, 1.5f, -1.5f);
    glRotatef(-90, 0, 1, 0);
    drawWall(3.0f, 3.0f);
    glPopMatrix();
    
    // Pared trasera
    setColor(0.8f, 0.6f, 0.4f);
    glPushMatrix();
    glTranslatef(0, 1.5f, -3.0f);
    glRotatef(180, 0, 1, 0);
    drawWall(4.0f, 3.0f);
    glPopMatrix();
    
    // Puerta principal (centrada)
    glPushMatrix();
    glTranslatef(0, 1.0f, 0.01f);
    drawDoor(0.8f, 2.0f);
    glPopMatrix();
    
    // Piso/base
    setColor(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0, -0.1f, -1.5f);
    drawCube(4.2f, 0.2f, 3.2f);
    glPopMatrix();
    
    // Techo plano
    glPushMatrix();
    glTranslatef(0, 3.0f, -1.5f);
    drawRoof(4.0f, 3.0f, 0.2f);
    glPopMatrix();
    
    // Pequeños detalles decorativos
    // Maceta junto a la puerta
    setColor(0.6f, 0.3f, 0.1f);
    glPushMatrix();
    glTranslatef(-1.8f, 0.3f, 0.3f);
    glutSolidCube(0.3f);
    glPopMatrix();
    
    // Planta en la maceta
    setColor(0.2f, 0.8f, 0.2f);
    glPushMatrix();
    glTranslatef(-1.8f, 0.6f, 0.3f);
    glutSolidSphere(0.15f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
}

#endif
