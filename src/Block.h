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
		if (type == 5) {
			animation.setTexture(textureManager->getTexture("Tiles/stone.png"));
			animation.setFrameCount(1);
		}
		else if (type == 2) {
			animation.setTexture(textureManager->getTexture("Tiles/moss_block.png"));
			animation.setFrameCount(1);
		}
		else if (type == 3) {
			animation.setTexture(textureManager->getTexture("Tiles/dirt.png"));
			animation.setFrameCount(1);
		}
		else if (type == 4) {
			animation.setTexture(textureManager->getTexture("Tiles/coarse_dirt.png"));
			animation.setFrameCount(1);
		}
		else if (type == 6) {
			animation.setTexture(textureManager->getTexture("Tiles/cobblestone.png"));
			animation.setFrameCount(1);
		}
		else if (type == 7) {
			animation.setTexture(textureManager->getTexture("Tiles/tuff.png"));
			animation.setFrameCount(1);
		}
		else if (type == 8) {
			animation.setTexture(textureManager->getTexture("Tiles/iron_ore.png"));
			animation.setFrameCount(1);
		}
		else if (type == 9) {
			animation.setTexture(textureManager->getTexture("Tiles/copper_ore.png"));
			animation.setFrameCount(1);
		}
	}

	int getType() {
		return type;
	}

	void setType(int t) {
		type = t;
	}

	void draw(RenderWindow& window, int x, int y, Camera* cam) {
		Position2f pos = cam->worldToScreen({(float)x, (float)y});
		animation.applyToSprite(sprite);
		sprite.setPosition(pos.x, pos.y);
		window.draw(sprite);
	}

	void update() {
		animation.update();
	}
};