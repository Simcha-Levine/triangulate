class App
{
private:
    VectorArray generateStrip(float width, float hight, int size)
    {
        VectorArray n_strip;

        for (size_t i = 0; i < size; i++)
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
                    toClose = distance(x, y, n_strip[j].x, n_strip[j].y) < 150;
                    if (toClose)
                        break;
                }
            }

            n_strip.push_back(sf::Vector2f(x, y));
        }

        return n_strip;
    }

    std::vector<sf::Color> generateColors()
    {
        std::vector<sf::Color> colors;

        float div = 10;
        for (size_t i = 0; i < 20; i++)
        {
            sf::Color color = sf::Color(0, 0, 0, 255);
            color.r = (std::rand() / float(RAND_MAX) * div) * (255 / div);
            color.g = (std::rand() / float(RAND_MAX) * div) * (255 / div);
            color.b = (std::rand() / float(RAND_MAX) * div) * (255 / div);
            colors.push_back(color);
            // colors.push_back(sf::Color(0, 0, 255, 255 / 2));
        }

        return colors;
    }

    void drawTriangles(sf::RenderTexture &render,
                       VectorArray &vertices,
                       std::vector<size_t> &indices,
                       std::vector<sf::Color> &colors)
    {

        int colorCount = 0;
        sf::VertexArray fill(sf::Triangles, indices.size());
        for (size_t i = 0; i < indices.size(); i++)
        {
            fill[i].position = vertices[indices[i]];
            fill[i].color = colors[colorCount];
            if ((i + 1) % 3 == 0)
                colorCount++;
        }
        render.draw(fill);
    }

    void drawDots(sf::RenderTexture &render, VectorArray &points, sf::Color color)
    {
        sf::CircleShape dot(5);
        dot.setFillColor(color);
        for (auto &pos : points)
        {
            dot.setOrigin(5, 5);
            dot.setPosition(pos);
            render.draw(dot);
        }
    }

    void drawLineStrip(sf::RenderTexture &render, VectorArray &vertices, sf::Color color)
    {
        sf::VertexArray strip(sf::LineStrip, vertices.size() + 1);
        for (size_t i = 0; i < vertices.size(); i++)
        {
            strip[i].position = vertices[i];
            strip[i].color = color;
        }
        strip[vertices.size()].position = vertices[0];
        strip[vertices.size()].color = sf::Color::Yellow;
        render.draw(strip);
    }

    bool isEntangled(VectorArray &vertices)
    {
        for (size_t i = 0; i < vertices.size(); i++)
        {
            auto &a = vertices[i];
            auto &b = vertices[(i + 1) % vertices.size()];

            for (size_t j = 0; j < vertices.size(); j++)
            {
                if (i == j || j == (i + 1) % vertices.size() || i == (j + 1) % vertices.size())
                {
                    continue;
                }
                auto &c = vertices[j];
                auto &d = vertices[(j + 1) % vertices.size()];

                if (doesIntersect(a, b, c, d))
                {
                    return true;
                }
            }
        }
        return false;
    }

    std::vector<sf::Vector2<size_t>> getIntersectionsIndex(VectorArray &vertices)
    {
        std::vector<sf::Vector2<size_t>> paired;

        for (size_t i = 0; i < vertices.size(); i++)
        {
            auto &a = vertices[i];
            auto &b = vertices[(i + 1) % vertices.size()];

            for (size_t j = 0; j < vertices.size(); j++)
            {
                if (i == j || j == (i + 1) % vertices.size() || i == (j + 1) % vertices.size())
                {
                    continue;
                }
                auto &c = vertices[j];
                auto &d = vertices[(j + 1) % vertices.size()];

                if (doesIntersect(a, b, c, d))
                {
                    bool was_paired = false;
                    for (auto &pair : paired)
                        if (pair.x == j && pair.y == i)
                        {
                            was_paired = true;
                            continue;
                        }
                    if (!was_paired)
                    {
                        paired.push_back(sf::Vector2(i, j));
                    }
                }
            }
        }
        return paired;
    }

    VectorArray getIntersectionsPos(VectorArray &vertices, std::vector<sf::Vector2<size_t>> &pairs)
    {
        VectorArray intersections;

        for (auto &pair : pairs)
        {
            auto &a = vertices[pair.x];
            auto &b = vertices[move_i(vertices.size(), pair.x, 1)];
            auto &c = vertices[pair.y];
            auto &d = vertices[move_i(vertices.size(), pair.y, 1)];
            intersections.push_back(finedIntersection(a, b, c, d));
        }
        return intersections;
    }

public:
    sf::RenderWindow &window;

    VectorArray strip;
    Triangulate tri;
    std::vector<sf::Color> colors;

    int selectedPoint = -1;
    sf::Font font;
    bool dataOn = false;
    bool risky = false;

    App(sf::RenderWindow &window, int amount) : window(window), tri(strip)
    {
        font.loadFromFile("font.ttf");

        strip = generateStrip(window.getSize().x, window.getSize().y, amount);
        colors = generateColors();
    }

    void handleEvents(sf::Event &event)
    {
        switch (event.type)
        {
        case sf::Event::MouseButtonPressed:
            for (size_t i = 0; i < strip.size(); i++)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (distance(mousePos.x, mousePos.y, strip[i].x, strip[i].y) < 10)
                {
                    selectedPoint = i;
                    break;
                }
            }
            break;

        case sf::Event::MouseButtonReleased:
            selectedPoint = -1;
            break;

        case sf::Event::KeyReleased:
            if (event.key.code == sf::Keyboard::Space)
            {
                dataOn = !dataOn;
            }
            else if (event.key.code == sf::Keyboard::R)
            {
                risky = !risky;
            }
            break;

        default:
            break;
        }
    }

    void update(sf::Clock &)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (selectedPoint != -1 && inBox(window.getSize().x, window.getSize().y, mousePos.x, mousePos.y))
        {
            strip[selectedPoint].x = mousePos.x;
            strip[selectedPoint].y = mousePos.y;
        }
    }

    void sortIndices(std::vector<size_t> &indices)
    {
        size_t i = 0;
        while (i < indices.size())
        {
            if (i == 0)
            {
                i += 3;
            }
            else if ((indices[i] + indices[i + 1] + indices[i + 2]) < (indices[i - 3] + indices[i - 2] + indices[i - 1]))
            {
                for (size_t j = 0; j < 3; j++)
                {
                    indices[i + j] = indices[i + j] + indices[i + j - 3];
                    indices[i + j - 3] = indices[i + j] - indices[i + j - 3];
                    indices[i + j] = indices[i + j] - indices[i + j - 3];
                }
                i -= 3;
            }
            else
            {
                i += 3;
            }
        }
    }

    void draw(sf::RenderTexture &render)
    {
        if (!isEntangled(strip) || risky)
        {
            auto indices = tri.triangulate();
            sortIndices(indices);

            drawTriangles(render, strip, indices, colors);
        }
        else
        {
            auto intersectionPairs = getIntersectionsIndex(strip);
            VectorArray intersections = getIntersectionsPos(strip, intersectionPairs);
            drawDots(render, intersections, sf::Color::Red);
        }
        drawLineStrip(render, strip, sf::Color::Yellow);
        drawDots(render, strip, sf::Color::Green);
    }

    void drawData(sf::RenderTexture &render)
    {
        if (!dataOn)
            return;

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(15);

        int total = 0;
        for (size_t i = 0; i < strip.size(); i++)
        {
            total += (int)tri.trapezoid(i) / 1000;

            size_t b = (i == 0) ? strip.size() - 1 : i - 1;
            size_t c = (i + 1) % strip.size();
            auto &point = strip[i];

            // text.setString(
            //     std::format(
            //         "{}\ncross: {}\nwind: {}",
            //         i,
            //         (int)(tri.crossProduct(point, strip[b], strip[c]) > 0),
            //         (int)(tri.trapezoid(i) > 0)

            //             )

            // );
            text.setString(std::format("{}", i));

            text.setPosition(point);

            render.draw(text);
        }

        auto intersectionPairs = getIntersectionsIndex(strip);
        int i = 0;
        int count_0 = 0;
        int count_1 = 0;
        for (auto &pair : intersectionPairs)
        {
            //        d
            //       /
            // a----*----b
            //     /
            //    c
            auto &a = strip[pair.x];
            auto &b = strip[move_i(strip.size(), pair.x, 1)];
            auto &c = strip[pair.y];
            auto &d = strip[move_i(strip.size(), pair.y, 1)];

            auto intersection = finedIntersection(a, b, c, d);

            auto direction = tri.crossProduct(intersection, a, d);

            text.setString(std::format("{}\ndir: {}", i, (int)(direction > 0)));
            text.setPosition(intersection);
            text.setCharacterSize(15);
            render.draw(text);

            if (direction > 0)
                count_1++;
            else
                count_0++;

            i++;
        }

        text.setString(std::format(
            "winding order: {}\ntotal: {}\ndirection 0: {}\ndirection 1: {}",
            tri.getWindingOrder(),
            total,
            count_0, count_1));
        text.setPosition(0, 0);
        text.setCharacterSize(20);
        render.draw(text);
    }
};
