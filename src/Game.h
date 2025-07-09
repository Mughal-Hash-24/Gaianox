#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include <fstream>
#include <random>

#include "MainMenu.h"
#include "TextureManager.h"
#include "RenderedChunks.h"
#include "Utilities.h"
#include "Steve.h"

class Game {
public:
    Game()
        : window(sf::VideoMode(1600, 900), "Gaianox"),
          gameState(0) // 0 = MainMenu, 1 = Playing, 2 = Exit
    {
        window.setFramerateLimit(60);
    }

    void run() {
        while (window.isOpen() && gameState != 2) {
            if (gameState == 0)
                handleMainMenu();
            else if (gameState == 1)
                gameLoop();
        }
    }

private:
    sf::RenderWindow window;
    TextureManager textureManager;
    int gameState;

    std::string worldPath;

    std::unique_ptr<Camera> camera = nullptr;
    std::unique_ptr<MainMenu> mainMenu = nullptr;
    std::unique_ptr<RenderedChunks> renderedChunks = nullptr;
    std::unique_ptr<Steve> steve = nullptr;

    void handleMainMenu() {
        mainMenu = std::make_unique<MainMenu>(1600, 900, &textureManager);
        int choice = mainMenu->run(window);

        if (choice == 0) { // New Game
            std::string worldName = mainMenu->promptWorldName(window);
            if (worldName.empty()) return;

            worldPath = "../Data/Saves/" + worldName;
            std::filesystem::create_directories(worldPath + "/Chunks");

            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<uint64_t> dist(0, 18'000'000'000'000'000'000ULL);
            uint64_t seed = dist(gen);

            std::ofstream out(worldPath + "/seed.txt");
            out << seed;
            out.close();

            initGame(seed);
            gameState = 1;
        }
        else if (choice == 1) { // Load Game
            std::string selectedWorld = mainMenu->chooseWorldToLoad(window);
            if (selectedWorld.empty()) return;

            worldPath = "../Data/Saves/" + selectedWorld;

            std::ifstream in(worldPath + "/seed.txt");
            uint64_t seed;
            in >> seed;
            in.close();

            initGame(seed);
            gameState = 1;
        }
        else {
            gameState = 2; // Exit
        }
    }

    void initGame(uint64_t seed) {
        steve = std::make_unique<Steve>(&textureManager);
        camera = std::make_unique<Camera>(1600, 900, 400, 300);
        camera->update(steve->getPosition());

        renderedChunks = std::make_unique<RenderedChunks>(&textureManager, worldPath, seed);
    }

    void gameLoop() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }

        // After exiting gameplay loop, return to menu
        gameState = 0;
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void update() {
        steve->update();
        Position2f stevePos = steve->getPosition();
        renderedChunks->update(stevePos.x);
        camera->update(stevePos);
    }

    void render() {
        window.clear(sf::Color(135, 206, 235)); // Sky blue
        renderedChunks->draw(window, camera.get());
        camera->draw(window);
        steve->draw(window, camera.get());
        window.display();
    }
};
