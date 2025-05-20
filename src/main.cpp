#include <SFML/Graphics.hpp>
#include "Chunk.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Gaianox");

    TextureManager textureManager;

    Chunk* chunks[3];
    chunks[0] = new Chunk(Position2f{ 0, 0 }, &textureManager);
    chunks[1] = new Chunk(Position2f{ 1, 0 }, &textureManager);
    chunks[2] = new Chunk(Position2f{ 2, 0 }, &textureManager);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (int i = 0; i < 3; i++) {
            chunks[i]->draw(window);
        }
        window.display();
    }

    return 0;
}