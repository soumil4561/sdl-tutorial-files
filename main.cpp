#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/Globals.h"
#include "../include/LTexture.h"
#include "../include/Dot.h"

LTexture gDotTexture;
SDL_Renderer *gRenderer;
SDL_Window *gWindow;
TTF_Font *gFont;
LTexture gManTexture;
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];

bool init();
void close();
bool loadMedia();

int main()
{
    if (!init())
    {
        printf("Failed initialize");
    }
    else
    {
        if (!loadMedia())
        {
            printf("Failed media load");
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            Dot dot;
            int frame =0;
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                        quit = true;
                    ++frame;
                    dot.handleEvent(e);
                }
                dot.move();
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);
                
                SDL_Rect* currentClip = &gSpriteClips[ frame / 4 ];
				// gSpriteSheetTexture.render( ( SCREEN_WIDTH - currentClip->w ) / 2, ( SCREEN_HEIGHT - currentClip->h ) / 2, currentClip );
               dot.render(gManTexture, currentClip);
                SDL_RenderPresent(gRenderer);
                //Cycle animation
				if( frame / 4 >= WALKING_ANIMATION_FRAMES ) frame = 0;
            }
        }
    }
    close();
    return 0;
}

bool init()
{
    // Initialize SDL
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // create window
        gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("Couldn't initialize SDL_image! SDL_image Error: %s\n", IMG_GetError());
                }

                if (TTF_Init() == -1)
                {
                    printf("Couldn't initialize SDL_ttf! SDL_ttf Error: %s\n", TTF_GetError());
                }
            }
        }
    }
    return success;
}

bool loadMedia()
{
    bool success = true;

    if(!gManTexture.loadFromFile("../resources/foo.png")){
        printf("Failed to load man");
        success = false;
    }
    else{
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w =  64;
        gSpriteClips[ 0 ].h = 205;

        gSpriteClips[ 1 ].x =  64;
        gSpriteClips[ 1 ].y =   0;
        gSpriteClips[ 1 ].w =  64;
        gSpriteClips[ 1 ].h = 205;
        
        gSpriteClips[ 2 ].x = 128;
        gSpriteClips[ 2 ].y =   0;
        gSpriteClips[ 2 ].w =  64;
        gSpriteClips[ 2 ].h = 205;

        gSpriteClips[ 3 ].x = 192;
        gSpriteClips[ 3 ].y =   0;
        gSpriteClips[ 3 ].w =  64;
        gSpriteClips[ 3 ].h = 205;
    }

    if (!gDotTexture.loadFromFile("../resources/dot.png"))
    {
        printf("Failed to load dot image. SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

void close()
{
    gDotTexture.free();
    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}