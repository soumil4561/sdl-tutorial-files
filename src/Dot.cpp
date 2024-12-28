#include "../include/Dot.h"
#include<SDL2/SDL.h>
#include "../include/Globals.h"

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

void Dot::render(LTexture& gDotTexture){
    gDotTexture.render(mPosX, mPosY);
    // SDL_RenderDrawPoint(gRenderer, mPosX, mPosY);
}

void Dot::render(LTexture& gDotTexture, SDL_Rect *clip){
    gDotTexture.render(mPosX, mPosY, clip);
}