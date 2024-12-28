#ifndef DOT_H
#define DOT_H

#include<SDL2/SDL.h>
#include "LTexture.h"
class Dot{
    public:
        Dot();

        void handleEvent(SDL_Event& e);

        void move();

        void render(LTexture& gDotTexture);
        void render(LTexture& gDotTexture, SDL_Rect *clip);

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

#endif