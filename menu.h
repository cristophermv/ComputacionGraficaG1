// Enhanced version of your menu system with C++98 compatibility

#ifndef MENU_H
#define MENU_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

class GameMenu {
private:
    struct MenuItem {
        std::string text;
        float x, y;
        bool enabled;
        float alpha; // For fade effects
        
        MenuItem(const std::string& t, float posX, float posY, bool en = true) 
            : text(t), x(posX), y(posY), enabled(en), alpha(1.0f) {}
    };
    
    std::vector<MenuItem> menuItems;
    int selectedItem;
    int windowWidth, windowHeight;
    bool mouseMode;
    int mouseX, mouseY;
    GLuint backgroundTexture;
    bool textureLoaded;
    
    // Animation variables
    float selectionGlowPhase;
    float menuTransitionAlpha;
    
    // Auto-centering variables
    float menuCenterX;
    float menuCenterY;
    float itemSpacing;
    float titleOffsetY;
    float footerOffsetY;
    
    // Enhanced color system
    struct Color {
        float r, g, b, a;
        Color(float red, float green, float blue, float alpha = 1.0f) 
            : r(red), g(green), b(blue), a(alpha) {}
            
        // Interpolation for smooth transitions
        Color lerp(const Color& other, float t) const {
            return Color(
                r + (other.r - r) * t,
                g + (other.g - g) * t,
                b + (other.b - b) * t,
                a + (other.a - a) * t
            );
        }
    };
    
    Color normalColor;
    Color selectedColor;
    Color disabledColor;
    Color highlightColor;
    
    // Enhanced position calculation with aspect ratio handling
    void calculateMenuPositions() {
        menuCenterX = 0.0f;
        menuCenterY = 0.0f;
        itemSpacing = 0.12f;
        
        float aspectRatio = (float)windowWidth / (float)windowHeight;
        
        // Dynamic spacing based on aspect ratio and window size
        if (aspectRatio > 2.0f) {
            itemSpacing *= 0.7f; // Tighter spacing for ultra-wide screens
        } else if (aspectRatio < 0.8f) {
            itemSpacing *= 1.3f; // Looser spacing for portrait screens
        }
        
        // Scale spacing based on window height for better readability
        if (windowHeight < 600) {
            itemSpacing *= 0.8f;
        } else if (windowHeight > 1200) {
            itemSpacing *= 1.1f;
        }
        
        float totalMenuHeight = (menuItems.size() - 1) * itemSpacing;
        float menuStartY = menuCenterY + (totalMenuHeight / 2.0f);
        
        // Assign positions to each item
        for (size_t i = 0; i < menuItems.size(); i++) {
            menuItems[i].x = menuCenterX;
            menuItems[i].y = menuStartY - (i * itemSpacing);
        }
        
        // Calculate offsets with better bounds checking
        titleOffsetY = std::min(0.85f, menuStartY + 0.3f);
        footerOffsetY = std::max(-0.9f, menuStartY - totalMenuHeight - 0.25f);
    }
    
public:
    GameMenu() : selectedItem(0), windowWidth(800), windowHeight(600), 
                 mouseMode(false), mouseX(0), mouseY(0), backgroundTexture(0),
                 textureLoaded(false), selectionGlowPhase(0.0f), menuTransitionAlpha(1.0f),
                 menuCenterX(0.0f), menuCenterY(0.0f), itemSpacing(0.12f),
                 titleOffsetY(0.6f), footerOffsetY(-0.8f),
                 normalColor(0.8f, 0.8f, 0.8f, 0.9f),
                 selectedColor(1.0f, 0.9f, 0.7f, 1.0f),
                 disabledColor(0.4f, 0.4f, 0.4f, 0.6f),
                 highlightColor(1.0f, 0.8f, 0.4f, 0.8f) {
        
        initializeMenuItems();
        calculateMenuPositions();
    }
    
    ~GameMenu() {
        cleanup();
    }
    
    void initializeMenuItems() {
        menuItems.clear();
        menuItems.push_back(MenuItem("Nueva partida", 0.0f, 0.1f));
        menuItems.push_back(MenuItem("Continuar", 0.0f, -0.1f));
        menuItems.push_back(MenuItem("Opciones", 0.0f, -0.2f));
        menuItems.push_back(MenuItem("Extras", 0.0f, -0.3f));
        menuItems.push_back(MenuItem("Salir", 0.0f, -0.4f));
    }
    
    void cleanup() {
        if (textureLoaded) {
            glDeleteTextures(1, &backgroundTexture);
            textureLoaded = false;
        }
        stopMusic();
    }
    
    void init() {
        // Enhanced OpenGL setup
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        
        playBackgroundMusic();
        loadBackgroundTexture("background.bmp");
        calculateMenuPositions();
        
        std::cout << "Menu initialized successfully" << std::endl;
    }
    
    void update(float deltaTime = 0.016f) {
        // Update animations
        selectionGlowPhase += deltaTime * 3.0f; // 3 rad/sec
        if (selectionGlowPhase > 2.0f * M_PI) {
            selectionGlowPhase -= 2.0f * M_PI;
        }
        
        // Update item alphas for fade effects
        for (size_t i = 0; i < menuItems.size(); i++) {
            float targetAlpha = menuItems[i].enabled ? 1.0f : 0.6f;
            menuItems[i].alpha += (targetAlpha - menuItems[i].alpha) * deltaTime * 5.0f;
        }
    }
    
    // Enhanced texture loading with better error handling
    bool loadBMPTexture(const char* filename) {
        FILE* file = fopen(filename, "rb");
        if (!file) {
            return false;
        }
        
        unsigned char header[54];
        if (fread(header, 1, 54, file) != 54) {
            fclose(file);
            return false;
        }
        
        if (header[0] != 'B' || header[1] != 'M') {
            fclose(file);
            return false;
        }
        
        int dataOffset = *(int*)&header[10];
        int width = *(int*)&header[18];
        int height = *(int*)&header[22];
        int bitsPerPixel = *(short*)&header[28];
        
        if (bitsPerPixel != 24 || width <= 0 || height <= 0) {
            fclose(file);
            return false;
        }
        
        fseek(file, dataOffset, SEEK_SET);
        
        int padding = (4 - (width * 3) % 4) % 4;
        int rowSize = width * 3 + padding;
        
        unsigned char* data = new unsigned char[height * width * 3];
        unsigned char* rowBuffer = new unsigned char[rowSize];
        
        bool success = true;
        for (int y = height - 1; y >= 0 && success; y--) {
            if (fread(rowBuffer, 1, rowSize, file) != rowSize) {
                success = false;
                break;
            }
            
            for (int x = 0; x < width; x++) {
                int dataIndex = (y * width + x) * 3;
                int bufferIndex = x * 3;
                
                data[dataIndex] = rowBuffer[bufferIndex + 2];     // R
                data[dataIndex + 1] = rowBuffer[bufferIndex + 1]; // G
                data[dataIndex + 2] = rowBuffer[bufferIndex];     // B
            }
        }
        
        fclose(file);
        delete[] rowBuffer;
        
        if (success) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Cambio: usar GL_CLAMP en lugar de GL_CLAMP_TO_EDGE para compatibilidad
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        }
        
        delete[] data;
        return success;
    }
    
    void loadBackgroundTexture(const char* filename) {
        if (textureLoaded) {
            glDeleteTextures(1, &backgroundTexture);
            textureLoaded = false;
        }
        
        glGenTextures(1, &backgroundTexture);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        
        const char* imageFiles[] = {
            "background.bmp", "menu_bg.bmp", "fondo.bmp", 
            "background.BMP", "village_bg.bmp"
        };
        
        bool imageLoaded = false;
        for (int i = 0; i < 5; i++) {
            if (loadBMPTexture(imageFiles[i])) {
                std::cout << "Background loaded: " << imageFiles[i] << std::endl;
                imageLoaded = true;
                textureLoaded = true;
                break;
            }
        }
        
        if (!imageLoaded) {
            std::cout << "Creating default background texture..." << std::endl;
            createDefaultTexture();
            textureLoaded = true;
        }
    }
    
    void createDefaultTexture() {
        const int texWidth = 512;
        const int texHeight = 512;
        unsigned char* textureData = new unsigned char[texWidth * texHeight * 3];
        
        // Create a more sophisticated default texture
        for (int y = 0; y < texHeight; y++) {
            for (int x = 0; x < texWidth; x++) {
                int index = (y * texWidth + x) * 3;
                
                // Create a subtle gradient with some noise
                float gradientX = (float)x / texWidth;
                float gradientY = (float)y / texHeight;
                float noise = sin(x * 0.1f) * cos(y * 0.1f) * 0.1f;
                
                unsigned char baseValue = (unsigned char)(20 + gradientY * 40 + noise * 10);
                textureData[index] = baseValue;         // R
                textureData[index + 1] = baseValue;     // G
                textureData[index + 2] = baseValue + 15; // B (slightly blue tint)
            }
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Cambio: usar GL_CLAMP en lugar de GL_CLAMP_TO_EDGE para compatibilidad
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        
        delete[] textureData;
    }
    
    void playBackgroundMusic() {
        const char* musicFiles[] = {
            "background.wav", "background.mp3", "music.wav", 
            "menu_music.wav", "background.WAV"
        };
        
        bool musicLoaded = false;
        for (int i = 0; i < 5; i++) {
            BOOL result = PlaySound(TEXT(musicFiles[i]), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
            if (result) {
                std::cout << "Music loaded: " << musicFiles[i] << std::endl;
                setMusicVolume(0.25f);
                musicLoaded = true;
                break;
            }
        }
        
        if (!musicLoaded) {
            std::cout << "No music files found. Game will run silently." << std::endl;
        }
    }
    
    void stopMusic() {
        PlaySound(NULL, NULL, 0);
    }
    
    void setMusicVolume(float volume) {
        volume = std::max(0.0f, std::min(1.0f, volume));
        DWORD vol = (DWORD)(volume * 0xFFFF);
        DWORD stereoVol = (vol << 16) | vol;
        waveOutSetVolume(0, stereoVol);
    }
    
    void setWindowSize(int width, int height) {
        if (width > 0 && height > 0) {
            windowWidth = width;
            windowHeight = height;
            calculateMenuPositions();
        }
    }
    
    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Setup viewport and projection
        glViewport(0, 0, windowWidth, windowHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        renderBackground();
        renderTitle();
        renderMenuItems();
        renderFooterText();
        
        glutSwapBuffers();
    }
    
    void renderBackground() {
        if (textureLoaded) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, backgroundTexture);
            glColor4f(0.8f, 0.8f, 0.9f, 1.0f);
            
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
            glEnd();
            
            glDisable(GL_TEXTURE_2D);
        } else {
            // Fallback gradient background
            glDisable(GL_TEXTURE_2D);
            glBegin(GL_QUADS);
                glColor4f(0.05f, 0.05f, 0.1f, 1.0f);  // Top
                glVertex2f(-1.0f, 1.0f);
                glVertex2f(1.0f, 1.0f);
                glColor4f(0.15f, 0.15f, 0.2f, 1.0f);  // Bottom
                glVertex2f(1.0f, -1.0f);
                glVertex2f(-1.0f, -1.0f);
            glEnd();
        }
    }
    
    void renderTitle() {
        // Main title
        glColor4f(0.9f, 0.8f, 0.6f, menuTransitionAlpha);
        std::string title = "Chaves New Life";
        renderCenteredText(title, menuCenterX, titleOffsetY, GLUT_BITMAP_TIMES_ROMAN_24);
        

    }
    
    void renderMenuItems() {
        for (size_t i = 0; i < menuItems.size(); i++) {
            Color color = normalColor;
            
            if (!menuItems[i].enabled) {
                color = disabledColor;
            } else if ((int)i == selectedItem) {
                // Animated selection color
                float glowIntensity = 0.5f + 0.5f * sin(selectionGlowPhase);
                color = normalColor.lerp(selectedColor, glowIntensity);
                renderSelectionGlow(menuItems[i].x, menuItems[i].y);
            }
            
            color.a *= menuItems[i].alpha * menuTransitionAlpha;
            glColor4f(color.r, color.g, color.b, color.a);
            
            renderCenteredText(menuItems[i].text, menuItems[i].x, menuItems[i].y, GLUT_BITMAP_HELVETICA_18);
        }
    }
    
    void renderSelectionGlow(float x, float y) {
        float glowIntensity = 0.3f + 0.2f * sin(selectionGlowPhase * 2.0f);
        glColor4f(highlightColor.r, highlightColor.g, highlightColor.b, glowIntensity * menuTransitionAlpha);
        
        float glowWidth = 0.5f;
        float glowHeight = 0.06f;
        
        glBegin(GL_QUADS);
            glVertex2f(x - glowWidth/2, y - glowHeight/2);
            glVertex2f(x + glowWidth/2, y - glowHeight/2);
            glVertex2f(x + glowWidth/2, y + glowHeight/2);
            glVertex2f(x - glowWidth/2, y + glowHeight/2);
        glEnd();
    }
    
    void renderFooterText() {
        glColor4f(0.5f, 0.5f, 0.5f, 0.8f * menuTransitionAlpha);
        
        std::string footerText = getFooterText();
        renderCenteredText(footerText, menuCenterX, footerOffsetY, GLUT_BITMAP_HELVETICA_12);
    }
    
    void renderCenteredText(const std::string& text, float x, float y, void* font) {
        // Calculate text width for centering
        int textWidth = 0;
        // Cambio: usar for tradicional en lugar de range-based for
        for (size_t i = 0; i < text.length(); i++) {
            textWidth += glutBitmapWidth(font, text[i]);
        }
        
        float normalizedWidth = (float)textWidth / windowWidth * 2.0f; // Convert to normalized coordinates
        float startX = x - normalizedWidth / 2.0f;
        
        glRasterPos2f(startX, y);
        // Cambio: usar for tradicional en lugar de range-based for
        for (size_t i = 0; i < text.length(); i++) {
            glutBitmapCharacter(font, text[i]);
        }
    }
    
    std::string getFooterText() {
        const char* descriptions[] = {
            "Empieza una partida nueva.",
            "Continua la partida actual.",
            "Configura las opciones del juego.",
            "Contenido adicional y extras.",
            "Salir del juego."
        };
        
        if (selectedItem >= 0 && selectedItem < 6) {
            return descriptions[selectedItem];
        }
        return "Selecciona una opcion.";
    }
    
    // Input handling methods remain the same but with improved bounds checking
    void handleKeyboard(unsigned char key) {
        switch (key) {
            case 27: // ESC
                exitGame();
                break;
            case 13: // ENTER
                selectCurrentItem();
                break;
            case 'w': case 'W':
                navigateUp();
                break;
            case 's': case 'S':
                navigateDown();
                break;
        }
        mouseMode = false;
    }
    
    void handleSpecialKeys(int key) {
        switch (key) {
            case GLUT_KEY_UP:
                navigateUp();
                break;
            case GLUT_KEY_DOWN:
                navigateDown();
                break;
        }
        mouseMode = false;
    }
    
    void handleMouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            mouseMode = true;
            updateMouseSelection(x, y);
            selectCurrentItem();
        }
    }
    
    void handleMouseMotion(int x, int y) {
        mouseMode = true;
        mouseX = x;
        mouseY = y;
        updateMouseSelection(x, y);
    }
    
    void updateMouseSelection(int x, int y) {
        if (x < 0 || y < 0 || x >= windowWidth || y >= windowHeight) return;
        
        float glX = (2.0f * x / windowWidth) - 1.0f;
        float glY = 1.0f - (2.0f * y / windowHeight);
        
        for (size_t i = 0; i < menuItems.size(); i++) {
            if (menuItems[i].enabled) {
                float itemY = menuItems[i].y;
                float itemX = menuItems[i].x;
                
                float detectionWidth = 0.6f;
                float detectionHeight = 0.06f;
                
                if (glY >= itemY - detectionHeight && glY <= itemY + detectionHeight &&
                    glX >= itemX - detectionWidth && glX <= itemX + detectionWidth) {
                    selectedItem = (int)i;
                    break;
                }
            }
        }
    }
    
    void navigateUp() {
        int attempts = 0;
        do {
            selectedItem--;
            if (selectedItem < 0) {
                selectedItem = (int)menuItems.size() - 1;
            }
            attempts++;
        } while (!menuItems[selectedItem].enabled && attempts < (int)menuItems.size());
    }
    
    void navigateDown() {
        int attempts = 0;
        do {
            selectedItem++;
            if (selectedItem >= (int)menuItems.size()) {
                selectedItem = 0;
            }
            attempts++;
        } while (!menuItems[selectedItem].enabled && attempts < (int)menuItems.size());
    }
    
    void selectCurrentItem() {
        if (selectedItem >= 0 && selectedItem < (int)menuItems.size() && 
            menuItems[selectedItem].enabled) {
            std::cout << "Selected: " << menuItems[selectedItem].text << std::endl;
        }
    }
    
    void exitGame() {
        std::cout << "Exiting game..." << std::endl;
        cleanup();
        exit(0);
    }
    
    void setItemEnabled(int index, bool enabled) {
        if (index >= 0 && index < (int)menuItems.size()) {
            menuItems[index].enabled = enabled;
            
            if (!enabled && selectedItem == index) {
                navigateDown();
            }
        }
    }
    
    // Getters
    int getSelectedItem() const { return selectedItem; }
    
    std::string getSelectedItemText() const {
        if (selectedItem >= 0 && selectedItem < (int)menuItems.size()) {
            return menuItems[selectedItem].text;
        }
        return "";
    }
    
    void recenterMenu() {
        calculateMenuPositions();
        std::cout << "Menu recentered" << std::endl;
    }
};

#endif // MENU_H
