#include "../include/LTexture.h"

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

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