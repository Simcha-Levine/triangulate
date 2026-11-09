bool inBox(float width, float hight, float x, float y)
{
    return x > 0 && x < width && y > 0 && y < hight;
}

float distance(float ax, float ay, float bx, float by)
{
    return ((sqrt(pow(abs(ax - bx), 2) + pow(abs(ay - by), 2))));
}

sf::Vector2f finedIntersection(sf::Vector2f &a, sf::Vector2f &b, sf::Vector2f &c, sf::Vector2f &d)
{
    float x = ((((a.y - a.x * (b.y - a.y) / (b.x - a.x)) - (c.y - c.x * (d.y - c.y) / (d.x - c.x))) /
                ((d.y - c.y) / (d.x - c.x) - (b.y - a.y) / (b.x - a.x))));

    float y = ((x * (b.y - a.y) / (b.x - a.x) + (a.y - a.x * (b.y - a.y) / (b.x - a.x))));

    return sf::Vector2f(x, y);
}

bool doesIntersect(sf::Vector2f &a, sf::Vector2f &b, sf::Vector2f &c, sf::Vector2f &d)
{
    sf::Vector2 i = finedIntersection(a, b, c, d);

    return (i.x < std::max(a.x, b.x) &&
            i.x > std::min(a.x, b.x) &&
            i.y < std::max(a.y, b.y) &&
            i.y > std::min(a.y, b.y) &&

            i.x < std::max(c.x, d.x) &&
            i.x > std::min(c.x, d.x) &&
            i.y < std::max(c.y, d.y) &&
            i.y > std::min(c.y, d.y));
}

size_t move_i(size_t size, size_t i, int n)
{
    if (n > 0)
        return (i + n) % size;
    else
        return (i == 0) ? size + n : i + n;
}
