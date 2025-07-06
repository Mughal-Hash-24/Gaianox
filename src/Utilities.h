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
    void draw(RenderWindow& window);

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

    float dx = 0, dy = 0;

    if (playerPos.x < leftBound)
        dx = playerPos.x - leftBound;
    else if (playerPos.x > rightBound)
        dx = playerPos.x - rightBound;

    if (playerPos.y < topBound)
        dy = playerPos.y - topBound;
    else if (playerPos.y > bottomBound)
        dy = playerPos.y - bottomBound;

    if (dx != 0 || dy != 0) {
        // Move instantly when outside dead zone
        position.x += dx;
        position.y += dy;
    } else {
        // Player is inside dead zone — recentre slowly
        float targetX = playerPos.x - viewportWidth / 2.0f;
        float targetY = playerPos.y - viewportHeight / 2.0f;

        // Smooth dampening — tweak factor (0.05f to 0.1f for smooth follow)
        position.x += (targetX - position.x) * 0.02f;
        position.y += (targetY - position.y) * 0.02f;
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

void Camera::draw(RenderWindow& window) {
    sf::RectangleShape rect(sf::Vector2f(deadZoneWidth, deadZoneHeight));

    // Calculate dead zone top-left (centered in the viewport)
    float screenX = (viewportWidth - deadZoneWidth) / 2.0f;
    float screenY = (viewportHeight - deadZoneHeight) / 2.0f;

    rect.setPosition(screenX, screenY); // Relative to screen
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(3.0f);

    window.draw(rect);
}
