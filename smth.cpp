#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int SCREEN_HEIGHT = 640;
int SCREEN_WIDTH = 480;

LTexture gDotTexture;
SDL_Renderer *gRenderer;
SDL_Window *gWindow;
TTF_Font *gFont;

class LTexture
{
public:
    // Initializes variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Gets image dimensions
    int getWidth();
    int getHeight();

private:
    // The actual hardware texture
    SDL_Texture *mTexture;

    // Image dimensions
    int mWidth;
    int mHeight;
};

LTexture::LTexture(){
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture(){
    free();
}

bool LTexture::loadFromFile(std::string path){
    free();

    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadingSurface = IMG_Load(path.c_str());
    if(loadingSurface == NULL){
        printf("Can't Load image from path: %s, SDL Issue: %s \n", path.c_str(), IMG_GetError());
    }
    else{
        SDL_SetColorKey(loadingSurface, SDL_TRUE, SDL_MapRGB(loadingSurface->format, 0,0,0));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadingSurface);
        if(newTexture == NULL){
            printf("Cant convert to texture, SDL Issue: %s \n", SDL_GetError());
        }
        else{
            mWidth = loadingSurface->w;
            mHeight = loadingSurface->h;
        }
        SDL_FreeSurface(loadingSurface);
    }
    mTexture = newTexture;
    return mTexture!=NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor){
    free();
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if(textSurface == NULL){
        printf("Unable to load texture to surface: %s\n",TTF_GetError());
    }
    else{
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    return mTexture!=NULL;
}

void LTexture::free(){
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL; // Nullify the texture after freeing
        mHeight = 0;
        mWidth = 0;
    }
}


void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point* center , SDL_RendererFlip flip){
    if (mTexture == NULL) {
        printf("Attempted to render a null texture!\n");
        return;
    }
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if( clip != NULL ){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	// SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getHeight(){
    return mHeight;
}

int LTexture::getWidth(){
        return mWidth;
}

class Dot{
    public:
        Dot();

        void handleEvent(SDL_Event& e);

        void move();

        void render(LTexture gDotTexture);

        int getWidth();
        int getHeight();
        int getVelocity();
        void setVelocity(int v);

    private:
        int DOT_WIDTH = 10;
        int DOT_HEIGHT = 10;
        int DOT_VELOCITY = 10;
        //The X and Y offsets of the dot
        int mPosX, mPosY;

        //The velocity of the dot
        int mVelX, mVelY;
};

Dot::Dot(){
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

int Dot::getHeight(){
    return DOT_HEIGHT;
}

int Dot::getWidth(){
    return DOT_WIDTH;
}

int Dot::getVelocity(){
    return DOT_VELOCITY;
}

void Dot::setVelocity(int v){
    DOT_VELOCITY = v;
}

void Dot::handleEvent(SDL_Event& e){
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch (e.key.keysym.sym){
            case SDLK_UP:
                mVelY -= DOT_VELOCITY;
                break;
            case SDLK_DOWN:
                mVelY += DOT_VELOCITY;
                break;
            case SDLK_LEFT:
                mVelX -= DOT_VELOCITY;
                break;
            case SDLK_RIGHT:
                mVelX += DOT_VELOCITY;
                break;
            default:
                break;
        }
    }
    else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        switch (e.key.keysym.sym){
            case SDLK_UP:
                mVelY += DOT_VELOCITY;
                break;
            case SDLK_DOWN:
                mVelY -= DOT_VELOCITY;
                break;
            case SDLK_LEFT:
                mVelX += DOT_VELOCITY;
                break;
            case SDLK_RIGHT:
                mVelX -= DOT_VELOCITY;
                break;
            default:
                break;
        }
    }
}

void Dot::move(){
    mPosX+=mVelX;
    if(mPosX <0 || DOT_WIDTH+mPosX>SCREEN_WIDTH){
        mPosX -= mVelX;
    }

    mPosY+=mVelY;
    if(mPosY <0 || DOT_HEIGHT+mPosY>SCREEN_HEIGHT){
        mPosY -= mVelY;
    }

}

void Dot::render(){
    gDotTexture.render(mPosX, mPosY);
    // SDL_RenderDrawPoint(gRenderer, mPosX, mPosY);
}

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

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                        quit = true;
                    dot.handleEvent(e);
                }
                
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);
                // dot.move();
                // dot.render(gDotTexture);
                gDotTexture.render(0,0);
                SDL_RenderPresent(gRenderer);
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

    if (!gDotTexture.loadFromFile("dot.bmp"))
    {
        printf("Failed to load dot image. SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}

void close()
{
    gDotTexture.free();
    // Destroy renderer and window
    if (gRenderer)
    {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = NULL;
    }

    if (gWindow)
    {
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
    }

    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}