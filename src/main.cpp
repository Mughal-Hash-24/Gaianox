#include <SFML/Graphics.hpp>
#include "RenderedChunks.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "Gaianox");

    TextureManager textureManager;

    Position2f playerPos{800, 450};
    Camera camera(1600, 900, 100, 75);
    RenderedChunks renderedChunks(&textureManager);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            playerPos.x += 1.f;
            camera.update(playerPos);
            renderedChunks.update(playerPos.x);
        }
        else if (Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            playerPos.x -= 1.f;
            camera.update(playerPos);
            renderedChunks.update(playerPos.x);
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            playerPos.y += 1.f;
            camera.update(playerPos);
        }
        else if (Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            playerPos.y -= 1.f;
            camera.update(playerPos);
        }

        window.clear();
        renderedChunks.draw(window, &camera);
        window.display();
    }

    return 0;
}