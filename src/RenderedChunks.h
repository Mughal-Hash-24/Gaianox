// --- RenderedChunks.h ---
#pragma once
#include "Chunk.h"
#include <fstream>
#include <string>
#include <iostream>

struct ChunkSlot {
    Chunk* chunk;
    int x;
};

const int RENDERED_CHUNK_COUNT = 9;

class RenderedChunks {
    ChunkSlot chunkSlots[9];
    int currCenterChunkX;
    TextureManager* textureManager;
    std::string savePath;

public:
    RenderedChunks(TextureManager* tex, const std::string& worldDir)
        : currCenterChunkX(1), textureManager(tex), savePath(worldDir + "/Chunks") {
        for (int i = 0; i < 9; i++) {
            int x = i - 4;
            chunkSlots[i].x = x;
            chunkSlots[i].chunk = loadOrGenerateChunk(x);
        }
    }

    void update(float playerX) {
        int newCenterChunkX = playerX / 512;
        if (newCenterChunkX == currCenterChunkX)
            return;

        int shift = newCenterChunkX - currCenterChunkX;

        while (shift > 0) {
            unloadLeftmost();
            shiftLeft();
            int newX = chunkSlots[RENDERED_CHUNK_COUNT - 2].x + 1;
            chunkSlots[RENDERED_CHUNK_COUNT - 1].x = newX;
            chunkSlots[RENDERED_CHUNK_COUNT - 1].chunk = loadOrGenerateChunk(newX);
            currCenterChunkX++;
            shift--;
        }

        while (shift < 0) {
            unloadRightmost();
            shiftRight();
            int newX = chunkSlots[1].x - 1;
            chunkSlots[0].x = newX;
            chunkSlots[0].chunk = loadOrGenerateChunk(newX);
            currCenterChunkX--;
            shift++;
        }
    }

    void draw(RenderWindow& window, Camera* cam) {
        for (int i = 0; i < RENDERED_CHUNK_COUNT; ++i) {
            if (chunkSlots[i].chunk)
                chunkSlots[i].chunk->draw(window, cam);
        }
    }

private:
    void unloadLeftmost() {
        Chunk* c = chunkSlots[0].chunk;
        int x = chunkSlots[0].x;
        if (c) {
            saveChunk(c, x);
            delete c;
        }
    }

    void unloadRightmost() {
        Chunk* c = chunkSlots[RENDERED_CHUNK_COUNT - 1].chunk;
        int x = chunkSlots[RENDERED_CHUNK_COUNT - 1].x;
        if (c) {
            saveChunk(c, x);
            delete c;
        }
    }

    void shiftLeft() {
        for (int i = 0; i < RENDERED_CHUNK_COUNT - 1; ++i)
            chunkSlots[i] = chunkSlots[i + 1];
    }

    void shiftRight() {
        for (int i = RENDERED_CHUNK_COUNT - 1; i > 0; --i)
            chunkSlots[i] = chunkSlots[i - 1];
    }

    std::string getFileName(int x) {
        return savePath + "/chunk_" + std::to_string(x) + "_0.bin";
    }

    Chunk* loadOrGenerateChunk(int x) {
        std::string filename = getFileName(x);
        std::ifstream in(filename, std::ios::binary);

        if (in.is_open()) {
            Chunk* c = new Chunk(x, textureManager, in);
            in.close();
            cout<<"[LOADED CHUNK]: "<<filename<<endl;
            return c;
        }

        Chunk* c = new Chunk({(float)x, 0}, textureManager);
        std::ofstream out(filename, std::ios::binary);
        cout<<"[GENERATED CHUNK]: "<<filename<<endl;
        if (out.is_open()) {
            c->saveToFile(out);
            out.close();
        }
        return c;
    }

    void saveChunk(Chunk* c, int x) {
        std::ofstream out(getFileName(x), std::ios::binary);
        if (out.is_open()) {
            c->saveToFile(out);
            out.close();
        }
    }
};
