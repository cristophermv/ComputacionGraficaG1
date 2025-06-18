#ifndef LAVADERO_H
#define LAVADERO_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Lavadero {
private:
    void drawCube(float x, float y, float z, float width, float height, float depth) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(width, height, depth);
        
        glBegin(GL_QUADS);
        
        // Cara frontal
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        
        // Cara trasera
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        
        // Cara superior
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        
        // Cara inferior
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        
        // Cara derecha
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        
        // Cara izquierda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        
        glEnd();
        glPopMatrix();
    }

public:
    void draw() {
        glPushMatrix();
        
        // Color gris claro para el concreto
        glColor3f(0.75f, 0.75f, 0.75f);
        
        // Base principal del lavadero
        drawCube(0.0f, 0.0f, 0.0f, 4.0f, 0.8f, 2.5f);
        
        // Pared trasera
        drawCube(0.0f, 1.2f, -1.0f, 4.0f, 1.6f, 0.3f);
        
        // Pared lateral izquierda
        drawCube(-1.7f, 1.2f, -0.2f, 0.6f, 1.6f, 1.3f);
        
        // Pared lateral derecha  
        drawCube(1.7f, 1.2f, -0.2f, 0.6f, 1.6f, 1.3f);
        
        // Interior de la pileta (más oscuro)
        glColor3f(0.5f, 0.5f, 0.5f);
        
        // Fondo de la pileta
        drawCube(0.0f, 0.2f, 0.3f, 3.0f, 0.1f, 1.4f);
        
        // Paredes interiores de la pileta
        // Pared interior trasera
        drawCube(0.0f, 0.6f, -0.4f, 3.0f, 0.6f, 0.1f);
        
        // Pared interior izquierda
        drawCube(-1.4f, 0.6f, 0.3f, 0.1f, 0.6f, 1.4f);
        
        // Pared interior derecha
        drawCube(1.4f, 0.6f, 0.3f, 0.1f, 0.6f, 1.4f);
        
        // Grifo simple
        glColor3f(0.6f, 0.6f, 0.7f); // Color metálico
        
        // Tubo vertical del grifo
        drawCube(0.0f, 2.5f, -0.8f, 0.1f, 1.0f, 0.1f);
        
        // Tubo horizontal del grifo
        drawCube(0.3f, 2.8f, -0.8f, 0.6f, 0.1f, 0.1f);
        
        // Pico del grifo
        drawCube(0.6f, 2.6f, -0.5f, 0.1f, 0.3f, 0.6f);
        
        glPopMatrix();
    }
    
    void draw(float x, float y, float z) {
        glPushMatrix();
        glTranslatef(x, y, z);
        draw();
        glPopMatrix();
    }
    
    void draw(float x, float y, float z, float scale) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(scale, scale, scale);
        draw();
        glPopMatrix();
    }
    
    void draw(float x, float y, float z, float scaleX, float scaleY, float scaleZ) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(scaleX, scaleY, scaleZ);
        draw();
        glPopMatrix();
    }
};

#endif
