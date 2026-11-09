#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <list>
#include <vector>
#include <ranges>
#include <format>

typedef std::vector<sf::Vector2f> VectorArray;
typedef std::vector<size_t> IndexArray;

#include "calculate.hpp"
#include "triangulate.hpp"
#include "app.hpp"

int main(int len, char const *argv[])
{
    std::srand(std::time(nullptr));

    float width = 800.f, hight = 900.f;

    sf::View view(sf::FloatRect(0, 0, width, hight));

    sf::RenderWindow window(sf::VideoMode(width, hight), "triangulate");
    window.setView(view);

    sf::Shader fragmentShader;
    fragmentShader.loadFromFile("shader.frag", sf::Shader::Fragment);
    fragmentShader.setUniform("texture", sf::Shader::CurrentTexture);

    sf::RenderTexture renderTarget;
    renderTarget.create(window.getSize().x, window.getSize().y);

    sf::RenderTexture textRenderTarget;
    textRenderTarget.create(window.getSize().x, window.getSize().y);

    window.setPosition(sf::Vector2i(500, 100));
    window.setVerticalSyncEnabled(true);

    int amount = 10;
    if (len > 1)
    {
        amount = std::stoi(argv[1]);
    }
    App app(window, amount);

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Resized)
            {

                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                renderTarget.create(event.size.width, event.size.height);
                textRenderTarget.create(event.size.width, event.size.height);
            }
            app.handleEvents(event);
        }
        app.update(clock);

        window.clear();

        renderTarget.clear();
        app.draw(renderTarget);
        renderTarget.display();

        textRenderTarget.clear();
        app.drawData(textRenderTarget);
        textRenderTarget.display();

        sf::Sprite textSprite(textRenderTarget.getTexture());
        fragmentShader.setUniform("background", renderTarget.getTexture());
        window.draw(textSprite, &fragmentShader);

        window.display();
    }
}
