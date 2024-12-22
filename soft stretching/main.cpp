//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include<iostream>

using namespace std;

enum KeyPressSurface {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

bool init();
bool loadMedia();
void close();
SDL_Surface* loadSurface(string path);


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Surface* gCurrentSurface = NULL; 
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Window* window = NULL;  //The window we'll be rendering to
SDL_Surface* screenSurface = NULL;    //The surface contained by the window

int main( int argc, char* args[] ){
        if(!init()){
            printf("Failed initialize");
        }
        else{
            if(!loadMedia()){
                printf("Failed media load");
            }
            else{
                bool quit = false;
                SDL_Event event;
                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

                while(!quit){
                    while(SDL_PollEvent(&event)!=0){
                        if(event.type == SDL_QUIT) quit = true; 
                        else if (event.type == SDL_KEYDOWN) {
                            switch(event.key.keysym.sym){
                                case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;

                                case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;

                                case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;

                                case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                break;

                                default:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;
                            }
                        }
                    }

                    //Apply the image stretched
                    SDL_Rect stretchRect;
                    stretchRect.x = 0;
                    stretchRect.y = 0;
                    stretchRect.w = SCREEN_WIDTH;
                    stretchRect.h = SCREEN_HEIGHT;
                    SDL_BlitScaled( gCurrentSurface, NULL, screenSurface, &stretchRect );
                    // SDL_BlitSurface(gCurrentSurface ,NULL, screenSurface,NULL);
                    SDL_UpdateWindowSurface(window);
                }
            }
        }
    close();
    return 0;
}

bool init(){
    //Initialize SDL
    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else{
        //create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL ){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else screenSurface = SDL_GetWindowSurface(window);
    }
    return success;
}

bool loadMedia(){
    bool success = true;

    //Load default surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "../resources/default.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL ){
        printf( "Failed to load default image!\n" );
        success = false;
    }

     gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "../resources/up.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL ){
        printf( "Failed to load up image!\n" );
        success = false;
    }

     gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "../resources/down.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL ){
        printf( "Failed to load down image!\n" );
        success = false;
    }

     gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "../resources/left.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL ){
        printf( "Failed to load left image!\n" );
        success = false;
    }

     gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "../resources/right.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL ){
        printf( "Failed to load right image!\n" );
        success = false;
    }

    return success;
}

SDL_Surface* loadSurface(string path){
    SDL_Surface* optimizedSurface = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if( loadedSurface == NULL ) printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
    else{
        optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, 0 );
        if( optimizedSurface == NULL ){
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
        return optimizedSurface;
    }

    return loadedSurface;
}

void close(){

    SDL_FreeSurface( gCurrentSurface );
    gCurrentSurface = NULL;
    //Destroy window
    SDL_DestroyWindow( window );
    //Quit SDL subsystems
    SDL_Quit();
}