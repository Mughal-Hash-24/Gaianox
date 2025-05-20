#pragma once
#include "Block.h"
#include <fstream>

class Chunk {
	Block** blocks;
	Position2f pos;

public:
	Chunk(Position2f p, TextureManager* tex) : pos{p.x, p.y} {
		blocks = new Block * [14];
		for (int i = 0; i < 14; i++) {
			blocks[i] = new Block[8];
		}

		ifstream in("Data/chunk.txt");

		int t = 0;
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 8; j++) {
				in >> t;
				blocks[i][j].initialize(t, tex);
			}
		}

		in.close();
	}

	~Chunk() {
		for (int i = 0; i < 14; i++) {
			delete[] blocks[i];
		}
		delete[] blocks;
	}

	void update() {
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 8; j++) {
				blocks[i][j].update();
			}
		}
	}

	void draw(RenderWindow& window) {
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 8; j++) {
				if (blocks[i][j].getType() != 0) {
					int x = (pos.x * 512) + (j * 64);
					int y = i * 64;
					blocks[i][j].draw(window, x, y);
				}
			}
		}
	}
};