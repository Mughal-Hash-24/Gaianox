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

		for (int j = 0; j < 8; j++) {  // columns
			int groundHeight = getRandomNumber(4, 13);

			for (int i = groundHeight; i < 14; i++) {
				blocks[i][j].initialize(1, tex);
			}
		}

	}

	Chunk(int x, TextureManager* tex, std::ifstream& in) {
		pos = {(float)x, 0};
		blocks = new Block * [14];
		for (int i = 0; i < 14; ++i)
			blocks[i] = new Block[8];

		for (int i = 0; i < 14; ++i) {
			for (int j = 0; j < 8; ++j) {
				int type = 1;
				in.read((char*)&type, sizeof(int));
				blocks[i][j].initialize(type, tex);
			}
		}
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

	void draw(RenderWindow& window, Camera* cam) {
		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 8; j++) {
				if (blocks[i][j].getType() != 0) {
					int x = (pos.x * 512) + (j * 64);
					int y = i * 64;
					blocks[i][j].draw(window, x, y, cam);
				}
			}
		}
	}

	void saveToFile(std::ofstream& out) {
		for (int i = 0; i < 14; ++i) {
			for (int j = 0; j < 8; ++j) {
				int type = blocks[i][j].getType();
				out.write(reinterpret_cast<const char*>(&type), sizeof(int));
			}
		}
	}

	void loadFromFile(std::ifstream& in) {
		for (int i = 0; i < 14; ++i) {
			for (int j = 0; j < 8; ++j) {
				int type;
				in.read(reinterpret_cast<char*>(&type), sizeof(int));
				blocks[i][j].setType(type);
			}
		}
	}
};