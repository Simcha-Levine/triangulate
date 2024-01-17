#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <list>
#include <vector>
#include <ranges>
#include <format>

#include "calculate.hpp"
#include "triangulate.hpp"

bool isEntangled();

float width = 800.f, hight = 900.f;
sf::RenderWindow window(sf::VideoMode(width, hight), "split");
std::vector<sf::Vector2f> trail;
int selectedPoint = -1;
std::vector<sf::Color> colors;
sf::Shader fragmentShader;

void setup()
{
    for (size_t i = 0; i < 10; i++)
    {
        float x, y;
        bool start = true, toClose = false;
        while (start || toClose)
        {
            start = false;
            toClose = false;
            x = float(std::rand()) / float(RAND_MAX) * width;
            y = float(std::rand()) / float(RAND_MAX) * hight;
            for (size_t j = 0; j < i; j++)
            {
                toClose = distance(x, y, trail[j].x, trail[j].y) < 150;
                if (toClose)
                    break;
            }
        }

        trail.push_back(sf::Vector2f(x, y));
    }

    float div = 10;
    for (size_t i = 0; i < 20; i++)
    {
        sf::Color color = sf::Color(0, 0, 0, 255);
        color.r = (std::rand() / float(RAND_MAX) * div) * (255 / div);
        color.g = (std::rand() / float(RAND_MAX) * div) * (255 / div);
        color.b = (std::rand() / float(RAND_MAX) * div) * (255 / div);
        colors.push_back(color);
        // colors.push_back(sf::Color::Red);
    }

    fragmentShader.loadFromFile("shader.frag", sf::Shader::Fragment);
    fragmentShader.setUniform("texture", sf::Shader::CurrentTexture);
}

void handleEvents(sf::Event &event)
{
    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        window.close();
    if (event.type == sf::Event::MouseButtonPressed)
    {
        for (size_t i = 0; i < trail.size(); i++)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (distance(mousePos.x, mousePos.y, trail[i].x, trail[i].y) < 7)
            {
                selectedPoint = i;
                break;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased)
    {
        selectedPoint = -1;
    }
    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::Space)
        {
        }
    }
}

void drawTriangles(sf::RenderTexture &renderTarget)
{
    auto indices = triangulate(trail);

    int colorCount = 0;
    sf::VertexArray fill(sf::Triangles, indices.size());
    for (size_t i = 0; i < indices.size(); i++)
    {
        fill[i].position = trail[indices[i]];
        fill[i].color = colors[colorCount];
        if ((i + 1) % 3 == 0)
            colorCount++;
    }
    renderTarget.draw(fill);
}

void drawDots(sf::RenderTexture &renderTarget)
{
    sf::CircleShape dot(5);
    dot.setFillColor(sf::Color::Green);
    for (auto &pos : trail)
    {
        dot.setOrigin(5, 5);
        dot.setPosition(pos);
        renderTarget.draw(dot);
    }
}

void drawLineStrip(sf::RenderTexture &renderTarget)
{
    sf::VertexArray strip(sf::LineStrip, trail.size() + 1);
    for (size_t i = 0; i < trail.size(); i++)
    {
        strip[i].position = trail[i];
        strip[i].color = sf::Color::Yellow;
    }
    strip[trail.size()].position = trail[0];
    strip[trail.size()].color = sf::Color::Yellow;
    renderTarget.draw(strip);
}

void drawData(sf::RenderTexture &renderTarget)
{
    sf::Font font;
    font.loadFromFile("font.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(15);

    renderTarget.clear();

    int total = 0;
    for (size_t i = 0; i < trail.size(); i++)
    {
        total += (int)trapezoid(trail, i) / 1000;

        size_t b = (i == 0) ? trail.size() - 1 : i - 1;
        size_t c = (i + 1) % trail.size();
        auto &point = trail[i];

        text.setString(std::format(
            "{}\ncross: {}\nwind: {}",
            i,
            crossProduct(point, trail[b], trail[c]) > 0,
            (int)trapezoid(trail, i) / 1000));

        text.setPosition(point);

        renderTarget.draw(text);
    }

    text.setString(std::format(
        "winding order: {}\ntotal: {}", getWindingOrder(trail), total));
    text.setPosition(0, 0);
    text.setCharacterSize(20);

    // auto box = text.getGlobalBounds();
    // sf::RectangleShape rect(sf::Vector2f(box.width, box.height));
    // rect.setFillColor(sf::Color::Yellow);
    // rect.setPosition(sf::Vector2f(box.left, box.top));
    // renderTarget.draw(rect);

    renderTarget.draw(text);

    renderTarget.display();
}

bool isEntangled()
{
    for (size_t i = 0; i < trail.size(); i++)
    {
        auto &a = trail[i];
        auto &b = trail[(i + 1) % trail.size()];

        for (size_t j = 0; j < trail.size(); j++)
        {
            if (i == j || j == (i + 1) % trail.size() || i == (j + 1) % trail.size())
            {
                continue;
            }
            auto &c = trail[j];
            auto &d = trail[(j + 1) % trail.size()];

            float x = ((a.y - a.x * (b.y - a.y) / (b.x - a.x)) - (c.y - c.x * (d.y - c.y) / (d.x - c.x))) /
                      (((d.y - c.y) / (d.x - c.x) - (b.y - a.y) / (b.x - a.x)));
            float y = x * (b.y - a.y) / (b.x - a.x) + (a.y - a.x * (b.y - a.y) / (b.x - a.x));

            if (x < std::max(a.x, b.x) &&
                x > std::min(a.x, b.x) &&
                y < std::max(a.y, b.y) &&
                y > std::min(a.y, b.y) &&

                x < std::max(c.x, d.x) &&
                x > std::min(c.x, d.x) &&
                y < std::max(c.y, d.y) &&
                y > std::min(c.y, d.y))
            {

                return true;
            }
        }
    }
    return false;
}

void update(sf::Clock &)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (selectedPoint != -1 && inBox(width, hight, mousePos.x, mousePos.y))
    {
        trail[selectedPoint].x = mousePos.x;
        trail[selectedPoint].y = mousePos.y;
    }
}

void draw(sf::RenderTexture &renderTarget)
{
    renderTarget.clear();

    if (!isEntangled())
        drawTriangles(renderTarget);
    else
        drawLineStrip(renderTarget);

    drawDots(renderTarget);

    renderTarget.display();
}

int main()
{
    std::srand(std::time(nullptr));

    sf::RenderTexture renderTarget;
    renderTarget.create(window.getSize().x, window.getSize().y);

    sf::RenderTexture textRenderTarget;
    textRenderTarget.create(window.getSize().x, window.getSize().y);

    window.setPosition(sf::Vector2i(500, 100));
    window.setVerticalSyncEnabled(true);

    setup();
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            handleEvents(event);
        }
        update(clock);
        draw(renderTarget);

        drawData(textRenderTarget);
        sf::Sprite textSprite(textRenderTarget.getTexture());
        fragmentShader.setUniform("background", renderTarget.getTexture());
        window.draw(textSprite, &fragmentShader);

        window.display();
    }
}
