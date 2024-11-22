#include "Bibliotecas.h"
#include "Collision.h"
#include "SDL.h"
#include "GamesRules.h" 

SDL_Rect* createBricks();

int main(int argc, char* args[]) {
    srand(time(NULL));

    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Texture* gBall = NULL;

    int didBallMove = 0, i, colorIndex, remainingBricks = AMOUNT_BRICKS, rectSize = 1, ballMod = 0;

    Color colors[6], currentColor; 
    Ball b = {0, 0, 7.5};

    colors[0] = (Color){0, 0, 255};
    colors[1] = (Color){255, 0, 0};
    colors[2] = (Color){0, 255, 0};
    colors[3] = (Color){255, 255, 0};
    colors[4] = (Color){75, 0, 130};
    colors[5] = (Color){255, 165, 0};

    if (!init(&gWindow, &gRenderer)) {
        printf("¡Error al inicializar!\n");
    } else {
        if (!loadMedia(&gBall, gRenderer, "ball.png")) {
            printf("¡Error al cargar los medios!\n");
        } else {
            bool quit = false;
            SDL_Event e;
            SDL_Rect fillRect;

            createRect(&fillRect, rectSize);
            createBall(&b);

            SDL_Rect* fillBricks;
            fillBricks = createBricks();

            Uint32 startTime, endTime, frameTime;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                startTime = SDL_GetTicks();

                const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
                if (currentKeyStates[SDL_SCANCODE_LEFT]) {
                    fillRect.x -= MOVE_SPEED;
                    if (didBallMove == 0) {
                        b.x = -1;
                        b.y = 1;
                    }
                    didBallMove = 1;
                } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
                    fillRect.x += MOVE_SPEED;
                    if (didBallMove == 0) {
                        b.x = -1;
                        b.y = 1;
                    }
                    didBallMove = 1;
                }

                // Limitar el movimiento
                if (fillRect.x < 0)
                    fillRect.x = 0;
                if (fillRect.x > SCREEN_WIDTH - SCREEN_WIDTH / 4)
                    fillRect.x = SCREEN_WIDTH - SCREEN_WIDTH / 4;

                // Movimiento de la pelota
                if (didBallMove == 1) {
                    if (b.y == 1)
                        b.quad.y -= BALL_SPEED + ballMod;
                    else
                        b.quad.y += BALL_SPEED + ballMod;

                    if (b.x == -1)
                        b.quad.x -= BALL_SPEED + ballMod;
                    else
                        b.quad.x += BALL_SPEED + ballMod;
                }

                // Colisiones
                if (collision(&b.quad, &b.x, &b.y)) {
                    if (didBallMove != 0) {
                        createRect(&fillRect, rectSize);
                        createBall(&b);
                        fillBricks = createBricks();
                        didBallMove = 0;
                        remainingBricks = AMOUNT_BRICKS;
                    }
                }

                if (collisionBrickBall(b.quad, fillRect)) {
                    b.y = 1;
                    b.quad.y = SCREEN_HEIGHT - 40;
                }

                for (i = 0; i < AMOUNT_BRICKS; i++) {
                    if (collisionBrickBall(b.quad, fillBricks[i])) {
                        fillBricks[i].x = SCREEN_WIDTH + 100;
                        fillBricks[i].y = SCREEN_HEIGHT + 100;
                        b.y = -1;
                        remainingBricks--;
                        break;
                    }
                }

                // Renderizado
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
                SDL_RenderClear(gRenderer);

                SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0xFF);
                SDL_RenderFillRect(gRenderer, &fillRect);
                SDL_RenderCopy(gRenderer, gBall, NULL, &b.quad);

                for (i = 0; i < AMOUNT_BRICKS; i++) {
                    colorIndex = i / 5;
                    currentColor = colors[colorIndex];

                    SDL_SetRenderDrawColor(gRenderer, currentColor.R, currentColor.G, currentColor.B, 0xFF);
                    SDL_RenderFillRect(gRenderer, &fillBricks[i]);
                    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
                    SDL_RenderDrawRect(gRenderer, &fillBricks[i]);
                }

                SDL_RenderPresent(gRenderer);

                endTime = SDL_GetTicks();
                frameTime = endTime - startTime;

                if (frameTime < 1000 / FPS) {
                    SDL_Delay((Uint32)(1000 / FPS - frameTime));
                }

                if (remainingBricks == 0) {
                    ballMod += 3;
                    createRect(&fillRect, rectSize);
                    createBall(&b);
                    fillBricks = createBricks();
                    didBallMove = 0;
                    remainingBricks = AMOUNT_BRICKS;
                }
            }
            free(fillBricks);
        }
    }

    close(&gWindow, &gRenderer, &gBall);

    return 0;
}
