#include "SDL.h"
#include "Bibliotecas.h"

bool init(SDL_Window** gWindow, SDL_Renderer** gRenderer)
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("¡SDL no se puede inicializar! Error de SDL: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        *gWindow = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (*gWindow == NULL)
        {
            printf("¡La ventana no se puede crear! Error de SDL: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (*gRenderer == NULL)
            {
                printf("¡El renderizador no se puede crear! Error de SDL: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(*gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("¡SDL_image no puede inicializarse! Error de SDL_image: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

SDL_Texture* loadTexture(char c[], SDL_Renderer* gRenderer)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(c);
    if (loadedSurface == NULL)
    {
        printf("¡No se puede cargar la imagen %s! Error de SDL_image: %s\n", c, IMG_GetError());
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 10, 94, 25));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("¡No se puede crear la textura a partir de %s! Error de SDL: %s\n", c, SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool loadMedia(SDL_Texture** gBall, SDL_Renderer* gRenderer, char c[])
{
    bool success = true;
    *gBall = loadTexture(c, gRenderer);

    if (*gBall == NULL)
    {
        printf("¡Fallo al cargar la textura de la imagen!\n");
        success = false;
    }

    return success;
}

void close(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gBall)
{
    SDL_DestroyTexture(*gBall);
    *gBall = NULL;

    SDL_DestroyRenderer(*gRenderer);
    SDL_DestroyWindow(*gWindow);
    *gRenderer = NULL;
    *gWindow = NULL;

    SDL_Quit();
    IMG_Quit();
}
