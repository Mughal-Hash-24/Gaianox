// --- MainMenu.h ---
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <filesystem>

class MainMenu {
public:
    enum Option { None, NewGame, LoadGame, Quit, Back };

    MainMenu(float width, float height) {
        font.loadFromFile("../Data/Fonts/RasterForgeRegular-JpBgm.ttf");

        optionsText = {
            createText("New Game", width / 2, 300, 64),
            createText("Load Game", width / 2, 400, 64),
            createText("Quit",     width / 2, 500, 64)
        };

        selectedIndex = 0;
        updateHighlight();
    }

    Option run(sf::RenderWindow& window) {
        sf::Event event;
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    return Quit;

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up)
                        moveUp();
                    else if (event.key.code == sf::Keyboard::Down)
                        moveDown();
                    else if (event.key.code == sf::Keyboard::Enter)
                        return getSelectedOption();
                }
            }

            window.clear();
            for (const auto& text : optionsText)
                window.draw(text);
            window.display();
        }
        return None;
    }

    std::string promptWorldName(sf::RenderWindow& window) {
        std::string worldName;
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
                if (event.type == sf::Event::Closed)
                    return "";

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    return ""; // Go back

                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8 && !worldName.empty()) // Backspace
                        worldName.pop_back();
                    else if ((event.text.unicode == 13 || event.text.unicode == '\n') && !std::all_of(worldName.begin(), worldName.end(), isspace)) // Enter
                        return worldName;
                    else if (event.text.unicode < 128 && isalnum(event.text.unicode))
                        worldName += static_cast<char>(event.text.unicode);

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

            for (size_t i = 0; i < saveTexts.size(); ++i) {
                saveTexts[i].setFillColor(i == selected ? sf::Color::Yellow : sf::Color::White);
            }

            window.clear();
            window.draw(header);
            for (auto& t : saveTexts) window.draw(t);
            window.draw(backHint);
            window.display();
        }
        return "";
    }

private:
    sf::Font font;
    std::vector<sf::Text> optionsText;
    int selectedIndex;

    sf::Text createText(const std::string& str, float x, float y, int size = 36) {
        sf::Text text;
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color::White);
        text.setPosition(x - text.getGlobalBounds().width / 2, y);
        return text;
    }

    void moveUp() {
        if (selectedIndex > 0) {
            selectedIndex--;
            updateHighlight();
        }
    }

    void moveDown() {
        if (selectedIndex < static_cast<int>(optionsText.size()) - 1) {
            selectedIndex++;
            updateHighlight();
        }
    }

    void updateHighlight() {
        for (size_t i = 0; i < optionsText.size(); ++i) {
            optionsText[i].setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
        }
    }

    Option getSelectedOption() const {
        switch (selectedIndex) {
            case 0: return NewGame;
            case 1: return LoadGame;
            case 2: return Quit;
            default: return None;
        }
    }
};
