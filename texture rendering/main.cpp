//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include<iostream>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>

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
SDL_Texture* loadTexture( std::string path );


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Texture* gCurrentSurface = NULL; 
SDL_Texture* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Window* window = NULL;  //The window we'll be rendering to
SDL_Surface* screenSurface = NULL;    //The surface contained by the window
SDL_Renderer* gRenderer = NULL;

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

                    SDL_RenderClear(gRenderer);
                    SDL_RenderCopy(gRenderer, gCurrentSurface, NULL, NULL);
                    SDL_RenderPresent(gRenderer);
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
        else{
            gRenderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_ACCELERATED);
            if( gRenderer == NULL ){
                printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            else{
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)){
                    printf("Couldn't initialize SDL_image! SDL_image Error: %s\n", IMG_GetError());
                }
                else screenSurface = SDL_GetWindowSurface(window);
            }
        }
    }
    return success;
}

bool loadMedia(){
    bool success = true;

    //Load default surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadTexture( "../resources/default.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL ){
        printf( "Failed to load default image!\n" );
        success = false;
    }

     gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadTexture( "../resources/up.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL ){
        printf( "Failed to load up image!\n" );
        success = false;
    }

     gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadTexture( "../resources/down.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL ){
        printf( "Failed to load down image!\n" );
        success = false;
    }

     gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadTexture( "../resources/left.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL ){
        printf( "Failed to load left image!\n" );
        success = false;
    }

     gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadTexture( "../resources/right.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL ){
        printf( "Failed to load right image!\n" );
        success = false;
    }

    return success;
}

SDL_Surface* loadSurface(string path){
    SDL_Surface* optimizedSurface = NULL;
    // SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == NULL ) printf( "Unable to load image %s! IMG Error: %s\n", path, IMG_GetError() );
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

SDL_Texture* loadTexture(string path){
    SDL_Texture* texture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface==NULL){
        printf( "Unable to load image %s! IMG Error: %s\n", path,IMG_GetError());
    }
    else{
        texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(texture == NULL) printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        SDL_FreeSurface(loadedSurface);
    }
    return texture;
}

void close(){
    for(int i =0;i<KEY_PRESS_SURFACE_TOTAL;i++){
        SDL_DestroyTexture( gKeyPressSurfaces[i] );
        gKeyPressSurfaces[i] = NULL;
    }

    SDL_DestroyTexture(gCurrentSurface);
    gCurrentSurface = NULL;
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( window );
    gRenderer = NULL;
    window = NULL;
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}