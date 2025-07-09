#pragma once

#include "Animation.h"
#include "Utilities.h"
#include "TextureManager.h"

using namespace std;
using namespace sf;

class Steve {
    TextureManager* textureManager;
    Sprite sprite;
    Animation animation;
    Position2f position;
    int state; // 0 -> idle morning, 1 -> idle night, 2 -> attack
    Position2f velocity;
    float gravity;
    int direction;

public:
    Steve(TextureManager* tex) : textureManager(tex) {
        animation.setTexture(textureManager->getTexture("Steve/Idle/Idle.png"));
        animation.setFrameCount(4);
        animation.setFrameDelay(100);
        animation.setCurrentFrame(0);
        animation.setLoop(true);
        animation.setHeight(35);
        animation.setWidth(21);

        position = {790, 3000};
        velocity = {10, 10};
        gravity = 0.2;
        direction = 1;
        state = 0;
    }

    Position2f getPosition() const {
        return position;
    }

    void draw(RenderWindow& window, Camera* cam) {
        Position2f pos = cam->worldToScreen({(float)position.x, (float)position.y});
        animation.applyToSprite(sprite);
        sprite.setScale(direction * 4.f, 4.f);
        if (direction == -1) {
            if (state == 0) {
                sprite.setOrigin(21, 0);
            }
            else if (state == 2) {
                sprite.setOrigin(41, 0);
            }
        }
        else {
            if (state == 0) {
                sprite.setOrigin(0, 0);
            }
            else if (state == 2) {
                sprite.setOrigin(0, 0);
            }
        }
        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);
    }

    void update() {
        int oldState = state;
        velocity.x = 0;
        velocity.y = 0;

        // Directional input and direction tracking
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            velocity.x = 10;
            direction = 1;
        } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
            velocity.x = -10;
            direction = -1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            velocity.y = -10;
        } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
            velocity.y = 10;
        }

        // Apply velocity to position
        position.x += velocity.x;
        position.y += velocity.y;

        // Determine state
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            state = 2; // Attack
        } else if (velocity.x != 0 || velocity.y != 0) {
            state = 1; // Walking
        } else {
            state = 0;; // Idle Night or Morning
        }

        animation.update();
        if (oldState != state) {
            updateTexture();
        }
    }

    void updateTexture() {
        switch (state) {
        case 0:
            animation.setTexture(textureManager->getTexture("Steve/Idle/Idle.png"));
            animation.setFrameCount(4);
            animation.setFrameDelay(100);
            animation.setCurrentFrame(0);
            animation.setLoop(true);
            animation.setHeight(35);
            animation.setWidth(21);
            break;

        case 1:
            animation.setTexture(textureManager->getTexture("Steve/Idle_Night/Idle_Night.png"));
            animation.setFrameCount(4);
            animation.setFrameDelay(100);
            animation.setCurrentFrame(0);
            animation.setLoop(true);
            animation.setHeight(35);
            animation.setWidth(36);
            break;

        case 2:
            animation.setTexture(textureManager->getTexture("Steve/Attack/Attack.png"));
            animation.setFrameCount(3);
            animation.setFrameDelay(100);
            animation.setCurrentFrame(0);
            animation.setLoop(true);
            animation.setHeight(35);
            animation.setWidth(41);
            break;
        }
    }
};