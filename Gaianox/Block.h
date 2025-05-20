#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "TextureManager.h"
#include "Utilities.h"

class Block {
	int type;
	Animation animation;
	Sprite sprite;
	TextureManager* textureManager;

public:
	Block() : animation(64, 64, 100) {
		type = 0;
	}

	void initialize(int type, TextureManager* tex) {
		this->type = type;
		textureManager = tex;
		if (type == 1) {
			animation.setTexture(textureManager->getTexture("Tiles/test.png"));
			animation.setFrameCount(1);
		}
	}

	int getType() {
		return type;
	}

	void setType(int t) {
		type = t;
	}

	void draw(RenderWindow& window, int x, int y) {
		animation.applyToSprite(sprite);
		sprite.setPosition(x, y);
		window.draw(sprite);
	}

	void update() {
		animation.update();
	}
};