// --- Game.h ---
#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include "MainMenu.h"
#include "TextureManager.h"
#include "RenderedChunks.h"
#include "Utilities.h"

class Game {
public:
    Game()
        : window(sf::VideoMode(1600, 900), "Gaianox"),
          playerPos{800, 3650},
          camera(1600, 900, 100, 75)
    {}

    void run() {
        MainMenu menu(1600, 900);

        while (window.isOpen()) {
            MainMenu::Option choice = menu.run(window);

            if (choice == MainMenu::NewGame) {
                std::string worldName = menu.promptWorldName(window);
                if (worldName.empty()) continue; // ESC pressed or closed

                worldPath = "../Data/Saves/" + worldName;
                std::filesystem::create_directories(worldPath + "/Chunks");

                // Generate and save random seed (0 to 18 quintillion)
                std::random_device rd;
                std::mt19937_64 gen(rd());
                std::uniform_int_distribution<uint64_t> dist(0, 18'000'000'000'000'000'000ULL);
                uint64_t seed = dist(gen);

                // Save to seed.txt
                std::ofstream out(worldPath + "/seed.txt");
                out << seed;
                out.close();

                // Optionally: pass seed to noise generator if needed
                renderedChunks = std::make_unique<RenderedChunks>(&textureManager, worldPath, seed);
                gameLoop();
            }

            else if (choice == MainMenu::LoadGame) {
                std::string selectedWorld = menu.chooseWorldToLoad(window);
                if (selectedWorld.empty()) continue; // ESC pressed or closed

                worldPath = "../Data/Saves/" + selectedWorld;

                std::ifstream in(worldPath + "/seed.txt");
                uint64_t seed;
                in >> seed;
                in.close();

                renderedChunks = std::make_unique<RenderedChunks>(&textureManager, worldPath, seed);
                gameLoop();
            }

            else if (choice == MainMenu::Quit || choice == MainMenu::None) {
                window.close();
            }
        }
    }

private:
    void gameLoop() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void update() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            playerPos.x += 3.f;
            camera.update(playerPos);
            renderedChunks->update(playerPos.x);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            playerPos.x -= 3.f;
            camera.update(playerPos);
            renderedChunks->update(playerPos.x);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            playerPos.y += 3.f;
            camera.update(playerPos);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            playerPos.y -= 3.f;
            camera.update(playerPos);
        }
    }

    void render() {
        window.clear(sf::Color(135, 206, 235));
        renderedChunks->draw(window, &camera);
        window.display();
    }

    sf::RenderWindow window;
    TextureManager textureManager;
    std::unique_ptr<RenderedChunks> renderedChunks;
    Camera camera;
    Position2f playerPos;
    std::string worldName;
    std::string worldPath;
};