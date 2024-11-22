#ifndef SDL_FUNCTIONS
#define SDL_FUNCTIONS

#include "Bibliotecas.h"

// Inicialización de SDL (ventana y renderizador)
bool init(SDL_Window** gWindow, SDL_Renderer** gRenderer);

// Cargar texturas desde un archivo
SDL_Texture* loadTexture(char c[], SDL_Renderer* gRenderer);

// Cargar los recursos necesarios (solo texturas en este caso)
bool loadMedia(SDL_Texture** gBall, SDL_Renderer* gRenderer, char c[]);

// Liberar recursos y cerrar SDL
void close(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gBall);

#endif // SDL_H_INCLUDED
