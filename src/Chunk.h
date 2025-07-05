#pragma once
#include "Block.h"
#include <fstream>

#include "PerlinNoise.h"

uint64_t hashString(const std::string& s) {
	uint64_t hash = 14695981039346656037ULL; // FNV-1a offset basis
	for (char c : s) {
		hash ^= static_cast<uint64_t>(c);
		hash *= 1099511628211ULL; // FNV prime
	}
	return hash;
}

class Chunk {
	Block** blocks;
	Position2f pos;

public:
	Chunk(Position2f p, TextureManager* tex, uint64_t seed) : pos{p.x, p.y} {
		blocks = new Block * [80];
		for (int i = 0; i < 80; i++) {
			blocks[i] = new Block[8];
		}

		PerlinNoise continentalness(seed);
		PerlinNoise erosion(seed ^ hashString("erosion"));
		PerlinNoise pv(seed ^ hashString("pv"));

		for (int j = 0; j < 8; j++) {  // columns

			float cont = continentalness.noise((j + (p.x * 8)) * 0.0025f, 1);      // Very low frequency = big continents
			float eros = erosion.noise((j + (p.x * 8)) * 0.01f, 2);            // Mid frequency = terrain smoothness
			float pv_   = pv.noise((j + (p.x * 8)) * 0.025f, 4);       // High frequency = rough detail

			// Normalize erosion to [0, 1] if needed
			float flatness = (eros + 1.0f) * 0.5f;

			// Flatten areas with high erosion
			float baseHeight = cont * (1.0f - flatness);

			// Add peaks/valleys influence based on erosion level
			float terrain = baseHeight + pv_ * (0.5f - flatness);

			// Map to height range, e.g., 0–100
			int gH = 50 + int(terrain * 25);
			int groundHeight = std::clamp(gH, 10, 70);;


			cout<<groundHeight<<endl;

			for (int i = groundHeight; i < 80; i++) {
				blocks[i][j].initialize(1, tex);
			}
		}
	}

	Chunk(int x, TextureManager* tex, std::ifstream& in) {
		pos = {(float)x, 0};
		blocks = new Block * [80];
		for (int i = 0; i < 80; ++i)
			blocks[i] = new Block[8];

		for (int i = 0; i < 80; ++i) {
			for (int j = 0; j < 8; ++j) {
				int type = 1;
				in.read((char*)&type, sizeof(int));
				blocks[i][j].initialize(type, tex);
			}
		}
	}


	~Chunk() {
		for (int i = 0; i < 80; i++) {
			delete[] blocks[i];
		}
		delete[] blocks;
	}

	void update() {
		for (int i = 0; i < 80; i++) {
			for (int j = 0; j < 8; j++) {
				blocks[i][j].update();
			}
		}
	}

	void draw(RenderWindow& window, Camera* cam) {
		for (int i = 0; i < 80; i++) {
			for (int j = 0; j < 8; j++) {
				if (blocks[i][j].getType() != 0) {
					int x = (pos.x * 512) + (j * 64);
					int y = i * 64;
					blocks[i][j].draw(window, x, y, cam);
				}
			}
		}
		// cout<<"drawn"<<endl;
	}

	void saveToFile(std::ofstream& out) {
		for (int i = 0; i < 80; ++i) {
			for (int j = 0; j < 8; ++j) {
				int type = blocks[i][j].getType();
				out.write(reinterpret_cast<const char*>(&type), sizeof(int));
			}
		}
	}

	void loadFromFile(std::ifstream& in) {
		for (int i = 0; i < 80; ++i) {
			for (int j = 0; j < 8; ++j) {
				int type;
				in.read(reinterpret_cast<char*>(&type), sizeof(int));
				blocks[i][j].setType(type);
			}
		}
	}
};
