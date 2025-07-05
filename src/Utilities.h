#pragma once
#include <iostream>
#include <random>
using namespace std;

struct Position2f {
	float x;
	float y;
};

int getRandomNumber(int min, int max) {
	static random_device rd;
	static mt19937 gen(rd());
	uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

#pragma once
#include "Block.h"

class Camera {
private:
    Position2f position; // Top-left of camera
    float viewportWidth, viewportHeight;

    // Dead zone box inside screen (relative to screen center)
    float deadZoneWidth, deadZoneHeight;

public:
    Camera(float width, float height, float dzWidth, float dzHeight);

    void update(const Position2f& playerPos);
    Position2f worldToScreen(const Position2f& worldPos) const;
    bool isVisible(float worldX, float worldY, float width, float height) const;

    // Accessors
    Position2f getPosition() const;
};

Camera::Camera(float width, float height, float dzWidth, float dzHeight)
    : viewportWidth(width), viewportHeight(height),
      deadZoneWidth(dzWidth), deadZoneHeight(dzHeight) {
    position = {0, 3200};
}

void Camera::update(const Position2f& playerPos) {
    float camCenterX = position.x + viewportWidth / 2.0f;
    float camCenterY = position.y + viewportHeight / 2.0f;

    float leftBound   = camCenterX - deadZoneWidth / 2.0f;
    float rightBound  = camCenterX + deadZoneWidth / 2.0f;
    float topBound    = camCenterY - deadZoneHeight / 2.0f;
    float bottomBound = camCenterY + deadZoneHeight / 2.0f;

    // Move camera only if player is outside dead zone
    if (playerPos.x < leftBound) {
        position.x -= (leftBound - playerPos.x);
    } else if (playerPos.x > rightBound) {
        position.x += (playerPos.x - rightBound);
    }

    if (playerPos.y < topBound) {
        position.y -= (topBound - playerPos.y);
    } else if (playerPos.y > bottomBound) {
        position.y += (playerPos.y - bottomBound);
    }
}

Position2f Camera::getPosition() const {
    return position;
}

Position2f Camera::worldToScreen(const Position2f& worldPos) const {
    return {
        worldPos.x - position.x,
        worldPos.y - position.y
    };
}

bool Camera::isVisible(float x, float y, float width, float height) const {
    return (
        x + width  >= position.x &&
        x          <= position.x + viewportWidth &&
        y + height >= position.y &&
        y          <= position.y + viewportHeight
    );
}