#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

class TextureManager {
private:
    Texture* textures;  // Pointer to dynamic array of textures
    bool* loaded;       // Pointer to dynamic array indicating if a texture is loaded
    string* names; // Pointer to dynamic array of texture filenames
    int textureCount;
    int maxTextures;    // Max number of textures we can handle

public:
    TextureManager(int maxTextures = 100);
    ~TextureManager();

    Texture& getTexture(string filename);
};

TextureManager::TextureManager(int maxTextures) : textureCount(0), maxTextures(maxTextures) {
    textures = new Texture[maxTextures];  // Allocate memory for textures
    loaded = new bool[maxTextures] { false };  // Allocate memory and initialize to false
    names = new string[maxTextures];  // Allocate memory for texture names
    cout<<"Manager Created"<<endl;
}

TextureManager::~TextureManager() {
    delete[] textures;  // Clean up dynamic memory
    delete[] loaded;    // Clean up dynamic memory
    delete[] names;     // Clean up dynamic memory
}

Texture& TextureManager::getTexture(string filename) {
    // Check if the texture is already loaded
    for (int i = 0; i < textureCount; i++) {
        if (names[i] == filename) {
            return textures[i];  // Return the already loaded texture
        }
    }

    // If not loaded yet, load the texture
    if (textureCount < maxTextures) {
        if (textures[textureCount].loadFromFile("../Data/" + filename)) {
            cout<<"Loaded"<<endl;
            names[textureCount] = filename;  // Store the texture name
            loaded[textureCount] = true;     // Mark as loaded
            textureCount++;                  // Increment texture count
            return textures[textureCount - 1];
        }
        else {
            cout<<"Failed to load"<<endl;
        }
    }
    else {
        cout << "FATAL ERROR" << endl;
        exit(0);
    }
}