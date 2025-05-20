#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
using namespace std;
using namespace sf;

class Animation {
    Texture* texture;
    int width;
    int height;
    int frameCount;
    int currentFrame;
    int frameDelay;
    bool loop;
    Clock clock;

public:
    Animation(int width = 40, int height = 40, int fd = 100) :
        texture(nullptr), width(width), height(height), frameCount(1), currentFrame(0), frameDelay(fd), loop(true) {
    }

    //Setters
    void setTexture(Texture& tex) { texture = &tex; }
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    void setFrameCount(int count) { frameCount = count; }
    void setFrameDelay(int delay) { frameDelay = delay; }
    void setCurrentFrame(int currentF) { currentFrame = currentF; }
    //////////////////////////////////////////////////

    //Getters
    int getWidth() const { return width; }
    int getCurrentFrame() const { return currentFrame; }
    int getFrameCount() const { return frameCount; }
    //////////////////////////////////////////////////

    void update() {
        if (clock.getElapsedTime().asMilliseconds() >= frameDelay) {
            currentFrame++;
            if (currentFrame >= frameCount) {
                if(loop)
                    currentFrame = 0;
				else
					currentFrame = frameCount - 1; // Stop at the last frame if not looping
			}
            clock.restart(); //Restart clock to keep track of time for next frame
        }
    }

    void applyToSprite(Sprite& sprite) const {
        if (texture) {
            sprite.setTexture(*texture);
            sprite.setTextureRect(IntRect(currentFrame * width, 0, width, height));
        }
    }

    void reset() {
        currentFrame = 0;
        clock.restart();
    }

	void setLoop(bool l) {
		loop = l;
	}
};

