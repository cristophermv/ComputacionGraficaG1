#ifndef SLIDESTARTER_H
#define SLIDESTARTER_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class SlideStarter {
private:
    // Variables del componente
    int currentSlide;
    const int totalSlides;
    float fadeAlpha;
    bool fadingOut;
    int slideTimer;
    const int slideDisplayTime;
    GLuint textures[3];
    bool texturesLoaded;
    bool isActive;
    
    // Estructura para datos de imagen BMP
    struct BMPImage {
        int width;
        int height;
        unsigned char* data;
    };
    
    // M�todos privados
    BMPImage loadBMP(const char* filename);
    void loadTextures();
    void drawCenteredLogo(int slideIndex);
    
public:
    // Constructor y destructor
    SlideStarter();
    ~SlideStarter();
    
    // M�todos p�blicos principales
    void render();
    void update();
    void handleKeyboard(unsigned char key);
    bool isRunning() const;
    void restart();
    
    // M�todos est�ticos para integraci�n con GLUT
    static void initializeOpenGL();
    static SlideStarter* getInstance();
    static void setInstance(SlideStarter* instance);
    
private:
    static SlideStarter* instance;
};

#endif // SLIDESTARTER_H
