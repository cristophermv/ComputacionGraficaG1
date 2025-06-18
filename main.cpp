#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "SlideStarter.h"
#include "menu.h"  // Incluir nuestro men� de Village
#include "config.h" // Incluir el sistema de configuraci�n
#include "Game.h"   // A�ADIR ESTA L�NEA - Incluir el juego FPS

// Variables globales
SlideStarter* slideStarter = NULL;
GameMenu* gameMenu = NULL;
GameConfig* gameConfig = NULL;  // Nueva variable para la configuraci�n
OptionsMenu* optionsMenu = NULL; // Nueva variable para el men� de opciones
bool gameStarted = false;
bool showMenu = false;
bool showOptions = false;  // Nueva variable para mostrar opciones
bool configApplied = false;  // Flag para saber si ya se aplic� la config
bool inFPSGame = false;     // A�ADIR ESTA L�NEA - Flag para saber si estamos en el juego FPS
Game* fpsGame = NULL;  // Instancia del juego FPS

// Funci�n para inicializar el starter
void Starter() {
    slideStarter = new SlideStarter();
    SlideStarter::setInstance(slideStarter);
}

// Funci�n para inicializar el men�
void initGameMenu() {
    gameMenu = new GameMenu();
    gameMenu->init();
    gameMenu->setWindowSize(1920, 1080);
}

// Funci�n para inicializar el sistema de configuraci�n
void initGameConfig() {
    if (!gameConfig) {
        gameConfig = new GameConfig();
        optionsMenu = new OptionsMenu(gameConfig);
        optionsMenu->setWindowSize(1600, 900);
    }
}

// Nueva funci�n para aplicar configuraci�n solo cuando sea seguro
void applyGameConfig() {
    if (gameConfig && !configApplied) {
        std::cout << "Aplicando configuracion del juego..." << std::endl;
        // Aplicar la configuraci�n que se carg� del archivo
        gameConfig->initializeSettings();
        configApplied = true;
    }
}

// A�ADIR ESTA FUNCI�N - Inicializar el juego FPS
void initFPSGame() {
    if (!inFPSGame) {
        std::cout << "Inicializando juego FPS..." << std::endl;
        inFPSGame = true;
        
        // Crear instancia del juego si no existe
        if (!fpsGame) {
            fpsGame = new Game();
            fpsGame->initGL();  // Inicializar OpenGL para el juego
        }
        
        // Configurar OpenGL para el juego FPS
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        
        // Configurar luz
        GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 1.0f};
        GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat lightDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Cielo azul
        
        // Configurar perspectiva para 3D
        int width = glutGet(GLUT_WINDOW_WIDTH);
        int height = glutGet(GLUT_WINDOW_HEIGHT);
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
        
        // Capturar mouse para el juego FPS
        glutSetCursor(GLUT_CURSOR_NONE);
        
        // Centrar mouse manualmente
        int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
        int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
        glutWarpPointer(centerX, centerY);
    }
}

// A�ADIR ESTA FUNCI�N - Salir del juego FPS
void exitFPSGame() {
    if (inFPSGame) {
        std::cout << "Saliendo del juego FPS..." << std::endl;
        inFPSGame = false;
        
        // Restaurar configuraci�n para men�s 2D
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        
        glViewport(0, 0, 1600, 900);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Restaurar cursor normal
        glutSetCursor(GLUT_CURSOR_INHERIT);
        
        showMenu = true;
    }
}

// Funciones de callback para GLUT
void display() {
    if (slideStarter && slideStarter->isRunning()) {
        slideStarter->render();
    } else if (showOptions && optionsMenu) {
        // Mostrar el men� de opciones/configuraci�n
        optionsMenu->render();
    } else if (showMenu && gameMenu) {
        // Mostrar el men� de Village
        gameMenu->render();
    } else {
        // Aqu� va tu juego principal despu�s de seleccionar una opci�n
        if (!gameStarted) {
            gameStarted = true;
            showMenu = true;
            if (!gameMenu) {
                initGameMenu();
            }
        }
        
        if (showMenu && gameMenu) {
            gameMenu->render();
        } else {
            // Aplicar configuraci�n solo cuando entremos al juego
            applyGameConfig();
            
            // MODIFICAR ESTA SECCI�N - Ejecutar el juego FPS aqu�
            if (!inFPSGame) {
                initFPSGame();
            }
            
            // Renderizar el juego FPS
            if (fpsGame) {
                fpsGame->display();
            }
        }
    }
}

void update() {
    if (slideStarter && slideStarter->isRunning()) {
        slideStarter->update();
    } else if (inFPSGame) {
        // A�ADIR ESTA L�NEA - Actualizar f�sica del juego FPS
        if (fpsGame) {
            fpsGame->updateMovement();
            fpsGame->updatePhysics();
        }
    }
    glutPostRedisplay();
}

void timer(int value) {
    update();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    // Debug: Mostrar estado actual
    std::cout << "Debug - Key: " << (int)key << " | showMenu: " << showMenu 
              << " | showOptions: " << showOptions << " | slideStarter running: " 
              << (slideStarter && slideStarter->isRunning()) << " | inFPSGame: " << inFPSGame << std::endl;
    
    if (slideStarter && slideStarter->isRunning()) {
        slideStarter->handleKeyboard(key);
        return;
    } 
    
    // ESTADO: Mostrando opciones
    if (showOptions) {
        std::cout << "Debug - En opciones, tecla presionada: " << (int)key << std::endl;
        
        if (key == 27) { // ESC - volver al men� principal
            std::cout << "Debug - ESC presionado en opciones, volviendo al menu" << std::endl;
            showOptions = false;
            showMenu = true;
            if (optionsMenu) {
                optionsMenu->hide();
            }
            
            // Restaurar configuraci�n del men�
            glViewport(0, 0, 1600, 900);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            return;
        }
        
        // Pasar otras teclas al men� de opciones
        if (optionsMenu) {
            optionsMenu->handleKeyboard(key);
            
            // Verificar si se seleccion� "Volver al men�" (opci�n 6)
            if (key == 13 && optionsMenu->getSelectedOption() == 6) {
                std::cout << "Debug - Volver al menu seleccionado via ENTER" << std::endl;
                showOptions = false;
                showMenu = true;
                optionsMenu->hide();
                
                // Restaurar configuraci�n del men�
                glViewport(0, 0, 1600, 900);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                
                return;
            }
        }
        return;
    }
    
    // ESTADO: Mostrando men� principal
    if (showMenu) {
        std::cout << "Debug - En menu principal, tecla presionada: " << (int)key << std::endl;
        
        if (gameMenu) {
            gameMenu->handleKeyboard(key);
            
            // Verificar si se seleccion� una opci�n que inicia el juego
            if (key == 13) { // ENTER
                int selectedItem = gameMenu->getSelectedItem();
                std::cout << "Debug - ENTER presionado, item seleccionado: " << selectedItem << std::endl;
                
                switch (selectedItem) {
                    case 0: // Nueva partida
                        showMenu = false;
                        std::cout << "Iniciando nueva partida..." << std::endl;
                        break;

                    case 1: // Continuar
                        showMenu = false;
                        std::cout << "Continuando partida..." << std::endl;
                        break;
                    case 2: // Opciones
                        std::cout << "Debug - Cambiando a opciones..." << std::endl;
                        showMenu = false;
                        showOptions = true;
                        
                        // Inicializar configuraci�n si no existe
                        if (!gameConfig) {
                            initGameConfig();
                        }
                        
                        // Mostrar men� de opciones
                        if (optionsMenu) {
                            optionsMenu->show();
                        }
                        std::cout << "Abriendo menu de opciones..." << std::endl;
                        break;
                    case 3: // Extras
                        std::cout << "Abriendo extras..." << std::endl;
                        break;
                    default:
                        break;
                }
            }
        }
        return;
    }
    
    // ESTADO: En el juego FPS - MODIFICAR ESTA SECCI�N
    if (inFPSGame) {
        std::cout << "Debug - En juego FPS, tecla presionada: " << (int)key << std::endl;
        
        // Manejar ESC para salir del juego FPS
        if (key == 27) { // ESC - volver al men�
            std::cout << "Debug - ESC presionado en juego FPS, volviendo al menu" << std::endl;
            exitFPSGame();
            return;
        }
        
        // Pasar todas las dem�s teclas al juego FPS
        if (fpsGame) {
            fpsGame->keyboard(key, x, y);
        }
        return;
    }
    
    // ESTADO: En el juego principal (fallback)
    std::cout << "Debug - En juego principal, tecla presionada: " << (int)key << std::endl;
    
    switch (key) {
        case 27: // ESC - volver al men�
            std::cout << "Debug - ESC presionado en juego, volviendo al menu" << std::endl;
            showMenu = true;
            showOptions = false; // Asegurar que opciones est� false
            
            // Restaurar configuraci�n del men� al volver
            glViewport(0, 0, 1600, 900);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            break;
        case 'o': // Tecla 'O' para abrir opciones directamente desde el juego
        case 'O':
            if (!gameConfig) {
                initGameConfig();
            }
            showOptions = true;
            showMenu = false; // Asegurar que men� est� false
            if (optionsMenu) {
                optionsMenu->show();
            }
            break;
        default:
            // Otros controles del juego
            break;
    }
}

// A�ADIR ESTA FUNCI�N - Manejar keyboardUp para el juego FPS
void keyboardUp(unsigned char key, int x, int y) {
    if (inFPSGame) {
        if (fpsGame) {
            fpsGame->keyboardUp(key, x, y);
        }
    }
}

void specialKeys(int key, int x, int y) {
    if (showOptions && optionsMenu) {
        // Manejar teclas especiales en el men� de opciones
        optionsMenu->handleSpecialKeys(key);
    } else if (showMenu && gameMenu) {
        gameMenu->handleSpecialKeys(key);
    } else if (inFPSGame) {
        // A�ADIR ESTA L�NEA - Pasar teclas especiales al juego FPS
        if (fpsGame) {
            fpsGame->specialKeysFunc(key, x, y);  // Usar el nombre correcto del m�todo
        }
    } else {
        // Teclas especiales del juego principal
        switch (key) {
            case GLUT_KEY_F1:
                showMenu = true; // Mostrar men� con F1
                // Restaurar configuraci�n del men�
                glViewport(0, 0, 1600, 900);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                break;
            case GLUT_KEY_F2:
                // Abrir opciones directamente con F2
                if (!gameConfig) {
                    initGameConfig();
                }
                showOptions = true;
                optionsMenu->show();
                break;
            default:
                break;
        }
    }
}

// A�ADIR ESTA FUNCI�N - Manejar specialKeyUp para el juego FPS
void specialKeyUp(int key, int x, int y) {
    if (inFPSGame) {
        if (fpsGame) {
            fpsGame->specialKeysUpFunc(key, x, y);  // Usar el nombre correcto del m�todo
        }
    }
}

void mouse(int button, int state, int x, int y) {
    if (showOptions && optionsMenu) {
        // El men� de opciones se maneja principalmente con teclado
        // pero puedes a�adir soporte de mouse si lo deseas
    } else if (showMenu && gameMenu) {
        gameMenu->handleMouse(button, state, x, y);
        
        // Verificar si se hizo click en una opci�n que inicia el juego
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            int selectedItem = gameMenu->getSelectedItem();
            if (selectedItem == 0 || selectedItem == 1 || selectedItem == 2) {
                showMenu = false;
            } else if (selectedItem == 3) { // Opciones
                showMenu = false;
                showOptions = true;
                if (!gameConfig) {
                    initGameConfig();
                }
                optionsMenu->show();
            }
        }
    }
    // Removido el manejo de mouse para el juego FPS ya que el m�todo no existe
}

void mouseMotion(int x, int y) {
    if (showMenu && gameMenu && !showOptions) {
        gameMenu->handleMouseMotion(x, y);
    } else if (inFPSGame) {
        // A�ADIR ESTA L�NEA - Pasar movimiento del mouse al juego FPS
        if (fpsGame) {
            fpsGame->mouseMotion(x, y);
        }
    }
}

void passiveMotion(int x, int y) {
    if (showMenu && gameMenu && !showOptions) {
        gameMenu->handleMouseMotion(x, y);
    }
    // Removido el manejo de passiveMouseMotion para el juego FPS ya que el m�todo no existe
}

void reshape(int width, int height) {
    if (inFPSGame) {
        // A�ADIR ESTA L�NEA - Usar reshape del juego FPS
        if (fpsGame) {
            fpsGame->reshape(width, height);
        }
    } else {
        // Configuraci�n para men�s 2D
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    
    // Actualizar tama�o del men� si existe
    if (gameMenu) {
        gameMenu->setWindowSize(width, height);
    }
    
    // Actualizar tama�o del men� de opciones si existe
    if (optionsMenu) {
        optionsMenu->setWindowSize(width, height);
    }
}

// Y modifica la secci�n del main() para aplicar la config al inicio:
int main(int argc, char** argv) {
    // Inicializar GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // A�ADIR GLUT_DEPTH para 3D
    glutInitWindowSize(1600, 900);  // Tama�o inicial temporal
    glutInitWindowPosition(170, 60);
    glutCreateWindow("Chaves New Life");
    
    // Inicializar OpenGL
    SlideStarter::initializeOpenGL();
    
    // ===== CONFIGURACI�N MEJORADA =====
    // Inicializar el sistema de configuraci�n
    std::cout << "Inicializando sistema de configuracion..." << std::endl;
    initGameConfig();
    
    // APLICAR LA CONFIGURACI�N CARGADA DEL ARCHIVO AL INICIO
    if (gameConfig) {
        std::cout << "Aplicando configuracion inicial desde config.ini..." << std::endl;
        gameConfig->initializeSettings();  // Esto aplicar� la resoluci�n y otras configuraciones
        configApplied = true;  // Marcar como aplicada
    }
    // ===== FIN DE LA CONFIGURACI�N =====
    
    // Inicializar el starter
    Starter();
    
    // Configurar callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);     // A�ADIR ESTA L�NEA
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeyUp);    // A�ADIR ESTA L�NEA
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(passiveMotion);
    glutTimerFunc(16, timer, 0);

    // Iniciar loop principal
    glutMainLoop();
    
    // Cleanup
    if (slideStarter) {
        delete slideStarter;
    }
    if (gameMenu) {
        delete gameMenu;
    }
    if (gameConfig) {
        delete gameConfig;
    }
    if (optionsMenu) {
        delete optionsMenu;
    }
    if (fpsGame) { // A�ADIR ESTA L�NEA - Limpiar la instancia del juego FPS
        delete fpsGame;
    }
    
    return 0;
}
