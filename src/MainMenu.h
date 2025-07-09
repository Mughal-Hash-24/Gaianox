#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include "TextureManager.h"
#include "Utilities.h"

class MainMenu {
public:
    enum Option { None, NewGame, LoadGame, Quit, Back };

    MainMenu(float width, float height, TextureManager* tex) : textureManager(tex) {
        background.setTexture(textureManager->getTexture("UI/Background/bg_1.png"));
        background.setPosition(0, 0);
        background.setScale(
            width / background.getLocalBounds().width,
            height / background.getLocalBounds().height
        );

        logo.setTexture(textureManager->getTexture("UI/Text.png"));
        logo.setPosition(width / 2 - logo.getGlobalBounds().width / 2, 100);

        // New Game Button
        Button newGameBtn;
        newGameBtn.setTextures(
            &textureManager->getTexture("UI/Buttons/NewGame/NewGame_Default.png"),
            &textureManager->getTexture("UI/Buttons/NewGame/NewGame_Hover.png")
        );
        newGameBtn.setPosition(width / 2 - 300, 350);
        newGameBtn.setSize(600, 100);
        buttons.push_back(newGameBtn);

        // Load Game Button
        Button loadGameBtn;
        loadGameBtn.setTextures(
            &textureManager->getTexture("UI/Buttons/LoadGame/LoadGame_Default.png"),
            &textureManager->getTexture("UI/Buttons/LoadGame/LoadGame_Hover.png")
        );
        loadGameBtn.setPosition(width / 2 - 300, 470);
        loadGameBtn.setSize(600, 100);
        buttons.push_back(loadGameBtn);

        // Quit Button
        Button quitBtn;
        quitBtn.setTextures(
            &textureManager->getTexture("UI/Buttons/Quit/Quit_Default.png"),
            &textureManager->getTexture("UI/Buttons/Quit/Quit_Hover.png")
        );
        quitBtn.setPosition(width / 2 - 300, 590);
        quitBtn.setSize(600, 100);
        buttons.push_back(quitBtn);
    }

    Option run(sf::RenderWindow& window) {
        sf::Event event;

        while (window.isOpen()) {
            sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    return Quit;

                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].isClicked(event, mousePos)) {
                        return static_cast<Option>(i + 1);
                    }
                }
            }

            for (auto& button : buttons)
                button.update(mousePos);

            window.clear();
            window.draw(background);
            window.draw(logo);
            for (auto& button : buttons)
                button.draw(window);
            window.display();
        }

        return None;
    }

    std::string promptWorldName(sf::RenderWindow& window) {
        std::string worldName;
        sf::Font font;
        font.loadFromFile("../Data/Fonts/RasterForgeRegular-JpBgm.ttf");

        sf::Text prompt("Enter World Name: ", font, 30);
        prompt.setPosition(100, 100);

        sf::Text inputText("", font, 30);
        inputText.setPosition(100, 150);
        inputText.setFillColor(sf::Color::Cyan);

        sf::Text backHint("Press ESC to go back", font, 20);
        backHint.setFillColor(sf::Color::Red);
        backHint.setPosition(100, 200);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) return "";

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    return "";

                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8 && !worldName.empty()) {
                        worldName.pop_back();
                    } else if ((event.text.unicode == 13 || event.text.unicode == '\n') &&
                               !std::all_of(worldName.begin(), worldName.end(), [](unsigned char c) { return std::isspace(c); })) {
                        return worldName;
                    } else if (event.text.unicode < 128 && std::isalnum(event.text.unicode)) {
                        worldName += static_cast<char>(event.text.unicode);
                    }
                    inputText.setString(worldName);
                }
            }

            window.clear();
            window.draw(prompt);
            window.draw(inputText);
            window.draw(backHint);
            window.display();
        }

        return "";
    }

    std::string chooseWorldToLoad(sf::RenderWindow& window) {
        sf::Font font;
        font.loadFromFile("../Data/Fonts/RasterForgeRegular-JpBgm.ttf");

        std::vector<std::string> saves;
        std::string basePath = "../Data/Saves";

        for (const auto& entry : std::filesystem::directory_iterator(basePath)) {
            if (entry.is_directory())
                saves.push_back(entry.path().filename().string());
        }

        if (saves.empty()) return "";

        size_t selected = 0;
        sf::Text header("Select Save:", font, 30);
        header.setPosition(100, 50);

        sf::Text backHint("Press ESC to go back", font, 20);
        backHint.setFillColor(sf::Color::Red);
        backHint.setPosition(100, 200 + saves.size() * 40);

        std::vector<sf::Text> saveTexts;
        for (size_t i = 0; i < saves.size(); ++i) {
            sf::Text t(saves[i], font, 28);
            t.setPosition(100, 100 + i * 40);
            saveTexts.push_back(t);
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) return "";

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) return "";
                    if (event.key.code == sf::Keyboard::Up && selected > 0) --selected;
                    else if (event.key.code == sf::Keyboard::Down && selected < saves.size() - 1) ++selected;
                    else if (event.key.code == sf::Keyboard::Enter) return saves[selected];
                }
            }

            for (size_t i = 0; i < saveTexts.size(); ++i)
                saveTexts[i].setFillColor(i == selected ? sf::Color::Yellow : sf::Color::White);

            window.clear();
            window.draw(header);
            for (auto& t : saveTexts) window.draw(t);
            window.draw(backHint);
            window.display();
        }

        return "";
    }

private:
    TextureManager* textureManager;
    sf::Sprite background;
    sf::Sprite logo;
    std::vector<Button> buttons;
};
