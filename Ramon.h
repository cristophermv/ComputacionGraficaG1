#ifndef RAMON_H
#define RAMON_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

class Ramon {
private:
    // Variables de estado
    float rotacionX;
    float rotacionY;
    float animacionBrazos;
    float animacionPiernas;
    float movimientoPersonaje;
    float direccionMovimiento;
    
    // Variables para comportamiento autónomo
    float tiempoTotal;
    float velocidadMovimiento;
    float rangoMovimiento;
    int estadoComportamiento; // 0=caminar, 1=pausa, 2=giro aleatorio
    float tiempoEstado;
    float duracionEstado;
    bool inicializado;
    
    // Variables de animación mejoradas
    float velocidadAnimacionBrazos;
    float velocidadAnimacionPiernas;
    float intensidadAnimacion;
    
    // Funciones privadas de dibujo
    void dibujarEsfera(float radio, float r, float g, float b);
    void dibujarCilindro(float radio, float altura, float r, float g, float b);
    void dibujarCuboRedondeado(float ancho, float alto, float profundo, float r, float g, float b);
    void dibujarCabeza();
    void dibujarCuello();
    void dibujarTorso();
    void dibujarBrazo(bool esIzquierdo);
    void dibujarPierna(bool esIzquierda);
    
    // Funciones de comportamiento autónomo
    void actualizarComportamiento();
    void cambiarEstado();
    float numeroAleatorio(float min, float max);

public:
    // Constructor
    Ramon();
    
    // Métodos públicos
    void dibujar();
    void actualizar();
    void rotar(float deltaX, float deltaY);
    void reiniciar();
    
    // Getters
    float getPosicionX() const { return movimientoPersonaje; }
    float getRotacionX() const { return rotacionX; }
    float getRotacionY() const { return rotacionY; }
    int getEstado() const { return estadoComportamiento; }
    
    // Setters
    void setPosicion(float x);
    void setRotacion(float x, float y);
    void setRangoMovimiento(float rango);
    void setVelocidad(float velocidad);
};

// Implementación de la clase Ramon

Ramon::Ramon() {
    rotacionX = 0.0f;
    rotacionY = 0.0f;
    animacionBrazos = 0.0f;
    animacionPiernas = 0.0f;
    movimientoPersonaje = 0.0f;
    direccionMovimiento = 1.0f;
    
    // Inicialización del comportamiento autónomo
    tiempoTotal = 0.0f;
    velocidadMovimiento = 0.03f + (rand() % 20) * 0.001f; // Velocidad aleatoria
    rangoMovimiento = 8.0f + (rand() % 10); // Rango aleatorio
    estadoComportamiento = 0;
    tiempoEstado = 0.0f;
    duracionEstado = 2.0f + (rand() % 30) * 0.1f; // Duración aleatoria
    inicializado = false;
    
    // Animaciones con variación
    velocidadAnimacionBrazos = 0.06f + (rand() % 10) * 0.002f;
    velocidadAnimacionPiernas = 0.08f + (rand() % 10) * 0.002f;
    intensidadAnimacion = 0.8f + (rand() % 40) * 0.01f;
    
    // Inicializar semilla aleatoria si no está inicializada
    if (!inicializado) {
        srand(time(NULL));
        inicializado = true;
    }
}

float Ramon::numeroAleatorio(float min, float max) {
    return min + (max - min) * ((float)rand() / RAND_MAX);
}

void Ramon::cambiarEstado() {
    // Cambiar aleatoriamente entre diferentes comportamientos
    int nuevoEstado = rand() % 4;
    
    switch(nuevoEstado) {
        case 0: // Caminar normal
            estadoComportamiento = 0;
            duracionEstado = numeroAleatorio(3.0f, 8.0f);
            velocidadMovimiento = numeroAleatorio(0.02f, 0.06f);
            break;
            
        case 1: // Pausa/contemplar
            estadoComportamiento = 1;
            duracionEstado = numeroAleatorio(1.0f, 4.0f);
            velocidadMovimiento = 0.0f;
            break;
            
        case 2: // Giro aleatorio
            estadoComportamiento = 2;
            duracionEstado = numeroAleatorio(0.5f, 2.0f);
            velocidadMovimiento = numeroAleatorio(0.01f, 0.03f);
            break;
            
        case 3: // Caminar rápido
            estadoComportamiento = 0;
            duracionEstado = numeroAleatorio(2.0f, 5.0f);
            velocidadMovimiento = numeroAleatorio(0.06f, 0.12f);
            break;
    }
    
    tiempoEstado = 0.0f;
}

void Ramon::actualizarComportamiento() {
    tiempoEstado += 0.016f; // ~60 FPS
    
    // Cambiar de estado cuando se cumple la duración
    if (tiempoEstado >= duracionEstado) {
        cambiarEstado();
    }
    
    // Ejecutar comportamiento según el estado actual
    switch(estadoComportamiento) {
        case 0: // Caminar
            movimientoPersonaje += direccionMovimiento * velocidadMovimiento;
            
            // Cambiar dirección si llega al límite
            if (movimientoPersonaje > rangoMovimiento || movimientoPersonaje < -rangoMovimiento) {
                direccionMovimiento *= -1.0f;
                rotacionY += 180.0f;
                
                // Posibilidad de cambiar de estado al llegar al límite
                if (rand() % 3 == 0) {
                    cambiarEstado();
                }
            }
            break;
            
        case 1: // Pausa
            // No mover, solo animar ligeramente
            intensidadAnimacion = 0.3f;
            
            // Posible giro lento mientras está parado
            if (rand() % 100 == 0) {
                rotacionY += numeroAleatorio(-5.0f, 5.0f);
            }
            break;
            
        case 2: // Giro aleatorio
            rotacionY += numeroAleatorio(-2.0f, 2.0f);
            movimientoPersonaje += direccionMovimiento * velocidadMovimiento * 0.5f;
            
            // Cambiar dirección aleatoriamente
            if (rand() % 30 == 0) {
                direccionMovimiento *= -1.0f;
            }
            break;
    }
    
    // Restaurar intensidad de animación si no está en pausa
    if (estadoComportamiento != 1) {
        intensidadAnimacion = 0.8f + sin(tiempoTotal * 0.1f) * 0.2f;
    }
}

void Ramon::dibujarEsfera(float radio, float r, float g, float b) {
    glColor3f(r, g, b);
    glutSolidSphere(radio, 20, 20);
}

void Ramon::dibujarCilindro(float radio, float altura, float r, float g, float b) {
    glColor3f(r, g, b);
    GLUquadricObj* cilindro = gluNewQuadric();
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    gluCylinder(cilindro, radio, radio, altura, 20, 20);
    glPopMatrix();
    gluDeleteQuadric(cilindro);
}

void Ramon::dibujarCuboRedondeado(float ancho, float alto, float profundo, float r, float g, float b) {
    glColor3f(r, g, b);
    glPushMatrix();
    glScalef(ancho, alto, profundo);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void Ramon::dibujarCabeza() {
    glPushMatrix();
    glTranslatef(0.0f, 1.8f, 0.0f);
    
    // Pequeño movimiento de cabeza autónomo
    float movimientoCabeza = sin(tiempoTotal * 0.3f) * 3.0f;
    glRotatef(movimientoCabeza, 0, 1, 0);
    
    // Cabeza principal (más pálida y siniestra)
    dibujarEsfera(0.5f, 0.8f, 0.8f, 0.75f);
    
    // Cabello negro y despeinado
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, 0.0f);
    glScalef(1.2f, 0.9f, 1.2f);
    dibujarEsfera(0.45f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();
    
    // Mechones despeinados con movimiento
    for(int i = 0; i < 6; i++) {
        glPushMatrix();
        float angulo = i * 60.0f + sin(tiempoTotal * 0.5f + i) * 5.0f;
        glRotatef(angulo, 0, 1, 0);
        glTranslatef(0.0f, 0.4f, 0.4f);
        glScalef(0.3f, 0.6f, 0.3f);
        dibujarEsfera(0.15f, 0.1f, 0.1f, 0.1f);
        glPopMatrix();
    }
    
    // Ojos rojos brillantes y grandes con parpadeo
    float parpadeo = sin(tiempoTotal * 3.0f) > 0.8f ? 0.3f : 1.0f;
    
    glPushMatrix();
    glTranslatef(-0.2f, 0.15f, 0.4f);
    dibujarEsfera(0.12f, 0.1f, 0.1f, 0.1f);
    glTranslatef(0.0f, 0.0f, 0.05f);
    dibujarEsfera(0.09f, parpadeo, 0.1f, 0.1f);
    glTranslatef(0.0f, 0.0f, 0.03f);
    dibujarEsfera(0.04f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, 0.15f, 0.4f);
    dibujarEsfera(0.12f, 0.1f, 0.1f, 0.1f);
    glTranslatef(0.0f, 0.0f, 0.05f);
    dibujarEsfera(0.09f, parpadeo, 0.1f, 0.1f);
    glTranslatef(0.0f, 0.0f, 0.03f);
    dibujarEsfera(0.04f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();
    
    // Cejas diabólicas con expresión variable
    float expresion = sin(tiempoTotal * 0.2f) * 10.0f;
    
    glPushMatrix();
    glTranslatef(-0.2f, 0.3f, 0.42f);
    glRotatef(30 + expresion, 0, 0, 1);
    glScalef(1.2f, 0.2f, 0.2f);
    dibujarEsfera(0.08f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.2f, 0.3f, 0.42f);
    glRotatef(-30 - expresion, 0, 0, 1);
    glScalef(1.2f, 0.2f, 0.2f);
    dibujarEsfera(0.08f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();
    
    // Nariz puntiaguda
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.48f);
    glScalef(0.5f, 0.5f, 1.5f);
    dibujarEsfera(0.05f, 0.7f, 0.7f, 0.65f);
    glPopMatrix();
    
    // Boca siniestra - sonrisa diabólica
    glPushMatrix();
    glTranslatef(0.0f, -0.15f, 0.45f);
    glScalef(2.0f, 0.3f, 0.3f);
    dibujarEsfera(0.08f, 0.2f, 0.05f, 0.05f);
    glPopMatrix();
    
    // Dientes puntiagudos
    for(int i = -2; i <= 2; i++) {
        glPushMatrix();
        glTranslatef(i * 0.06f, -0.15f, 0.47f);
        glScalef(0.3f, 0.5f, 0.3f);
        dibujarEsfera(0.03f, 0.9f, 0.9f, 0.85f);
        glPopMatrix();
    }
    
    // Cicatrices en la cara
    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.47f);
    glRotatef(45, 0, 0, 1);
    glScalef(0.8f, 0.1f, 0.1f);
    dibujarEsfera(0.05f, 0.3f, 0.1f, 0.1f);
    glPopMatrix();
    
    glPopMatrix();
}

void Ramon::dibujarCuello() {
    glPushMatrix();
    glTranslatef(0.0f, 1.15f, 0.0f);
    dibujarCilindro(0.1f, 0.25f, 0.8f, 0.8f, 0.75f);
    glPopMatrix();
}

void Ramon::dibujarTorso() {
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 0.0f);
    
    // Ligero movimiento de respiración
    float respiracion = 1.0f + sin(tiempoTotal * 0.8f) * 0.05f;
    glScalef(respiracion, 1.0f, respiracion);
    
    // Torso principal - más oscuro y siniestro
    dibujarCuboRedondeado(0.8f, 1.4f, 0.5f, 0.2f, 0.2f, 0.25f);
    
    // Detalles de camisa desgarrada
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.26f);
    dibujarCuboRedondeado(0.6f, 1.2f, 0.02f, 0.15f, 0.15f, 0.2f);
    glPopMatrix();
    
    // Agujeros en la camisa
    glPushMatrix();
    glTranslatef(-0.2f, 0.2f, 0.27f);
    dibujarEsfera(0.08f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.15f, -0.3f, 0.27f);
    dibujarEsfera(0.06f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();
    
    glPopMatrix();
}

void Ramon::dibujarBrazo(bool esIzquierdo) {
    float multiplicador = esIzquierdo ? -1.0f : 1.0f;
    float animacion = sin(animacionBrazos + (esIzquierdo ? 0 : 3.14159f)) * 25.0f * intensidadAnimacion;
    
    // Movimiento adicional aleatorio ocasional
    if (rand() % 200 == 0) {
        animacion += numeroAleatorio(-15.0f, 15.0f);
    }
    
    glPushMatrix();
    glTranslatef(multiplicador * 0.4f, 1.1f, 0.0f);
    
    // Hombro
    dibujarEsfera(0.12f, 0.8f, 0.8f, 0.75f);
    
    // Rotación del brazo completo desde el hombro
    glRotatef(animacion, 1.0f, 0.0f, 0.0f);
    glRotatef(esIzquierdo ? 10.0f : -10.0f, 0.0f, 0.0f, 1.0f);
    
    // Brazo superior
    glPushMatrix();
    glTranslatef(0.0f, -0.25f, 0.0f);
    dibujarCilindro(0.08f, 0.5f, 0.2f, 0.2f, 0.25f);
    glPopMatrix();
    
    // Codo
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    dibujarEsfera(0.09f, 0.8f, 0.8f, 0.75f);
    glPopMatrix();
    
    // Antebrazo
    glPushMatrix();
    glTranslatef(0.0f, -0.75f, 0.0f);
    dibujarCilindro(0.07f, 0.5f, 0.2f, 0.2f, 0.25f);
    glPopMatrix();
    
    // Muñeca
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    dibujarEsfera(0.07f, 0.8f, 0.8f, 0.75f);
    glPopMatrix();
    
    // Mano con garras
    glPushMatrix();
    glTranslatef(0.0f, -1.15f, 0.0f);
    dibujarEsfera(0.09f, 0.7f, 0.7f, 0.65f);
    
    // Garras/uñas largas con movimento
    for(int i = 0; i < 4; i++) {
        glPushMatrix();
        float angulo = i * 20.0f - 30.0f + sin(tiempoTotal * 2.0f + i) * 5.0f;
        glRotatef(angulo, 0, 0, 1);
        glTranslatef(0.0f, -0.12f, 0.0f);
        glScalef(0.2f, 0.8f, 0.2f);
        dibujarEsfera(0.03f, 0.1f, 0.1f, 0.1f);
        glPopMatrix();
    }
    glPopMatrix();
    
    glPopMatrix();
}

void Ramon::dibujarPierna(bool esIzquierda) {
    float multiplicador = esIzquierda ? -0.15f : 0.15f;
    float animacion = sin(animacionPiernas + (esIzquierda ? 0 : 3.14159f)) * 20.0f * intensidadAnimacion;
    
    glPushMatrix();
    glTranslatef(multiplicador, -0.3f, 0.0f);
    glRotatef(animacion, 1.0f, 0.0f, 0.0f);
    
    // Cadera
    dibujarEsfera(0.1f, 0.15f, 0.15f, 0.2f);
    
    // Muslo
    glPushMatrix();
    glTranslatef(0.0f, -0.4f, 0.0f);
    dibujarCilindro(0.12f, 0.8f, 0.1f, 0.1f, 0.15f);
    glPopMatrix();
    
    // Rodilla
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.0f);
    dibujarEsfera(0.1f, 0.1f, 0.1f, 0.15f);
    glPopMatrix();
    
    // Pantorrilla
    glPushMatrix();
    glTranslatef(0.0f, -1.15f, 0.0f);
    dibujarCilindro(0.1f, 0.7f, 0.1f, 0.1f, 0.15f);
    glPopMatrix();
    
    // Tobillo
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, 0.0f);
    dibujarEsfera(0.08f, 0.8f, 0.8f, 0.75f);
    glPopMatrix();
    
    // Zapatos/botas negras
    glPushMatrix();
    glTranslatef(0.0f, -1.65f, 0.15f);
    dibujarCuboRedondeado(0.22f, 0.15f, 0.5f, 0.05f, 0.05f, 0.05f);
    glPopMatrix();
    
    glPopMatrix();
}

void Ramon::dibujar() {
    glPushMatrix();
    
    // Movimiento automático
    glTranslatef(movimientoPersonaje, 0.0f, 0.0f);
    
    // Aplicar rotaciones
    glRotatef(rotacionX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotacionY, 0.0f, 1.0f, 0.0f);
    
    // Pequeño balanceo del cuerpo completo
    float balanceo = sin(tiempoTotal * 0.4f) * 2.0f;
    glRotatef(balanceo, 0.0f, 0.0f, 1.0f);
    
    // Dibujar el cuerpo
    dibujarPierna(true);   
    dibujarPierna(false);  
    dibujarTorso();        
    dibujarCuello();       
    dibujarBrazo(true);    
    dibujarBrazo(false);   
    dibujarCabeza();       
    
    glPopMatrix();
}

void Ramon::actualizar() {
    // Incrementar tiempo total
    tiempoTotal += 0.016f; // ~60 FPS
    
    // Actualizar comportamiento autónomo
    actualizarComportamiento();
    
    // Actualizar animaciones con velocidades variables
    animacionBrazos += velocidadAnimacionBrazos;
    animacionPiernas += velocidadAnimacionPiernas;
    
    // Ocasionalmente cambiar velocidades de animación
    if (rand() % 300 == 0) {
        velocidadAnimacionBrazos = numeroAleatorio(0.04f, 0.12f);
        velocidadAnimacionPiernas = numeroAleatorio(0.06f, 0.15f);
    }
}

void Ramon::rotar(float deltaX, float deltaY) {
    rotacionX += deltaX;
    rotacionY += deltaY;
}

void Ramon::reiniciar() {
    rotacionX = rotacionY = 0.0f;
    movimientoPersonaje = 0.0f;
    direccionMovimiento = 1.0f;
    animacionBrazos = 0.0f;
    animacionPiernas = 0.0f;
    tiempoTotal = 0.0f;
    estadoComportamiento = 0;
    tiempoEstado = 0.0f;
}

void Ramon::setPosicion(float x) {
    movimientoPersonaje = x;
}

void Ramon::setRotacion(float x, float y) {
    rotacionX = x;
    rotacionY = y;
}

void Ramon::setRangoMovimiento(float rango) {
    rangoMovimiento = rango;
}

void Ramon::setVelocidad(float velocidad) {
    velocidadMovimiento = velocidad;
}

#endif // RAMON_H
