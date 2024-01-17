float trapezoid(std::vector<sf::Vector2f> &polygon, int i)
{
    auto &current = polygon[i];
    auto &next = polygon[(i + 1) % polygon.size()];
    return (next.x - current.x) * (next.y + current.y);
}

bool getWindingOrder(std::vector<sf::Vector2f> &polygon)
{
    float total = 0;
    for (size_t i = 0; i < polygon.size(); i++)
    {
        total += trapezoid(polygon, i);
    }
    return total > 0;
}

float crossProduct(sf::Vector2f &center, sf::Vector2f &b, sf::Vector2f &c)
{
    sf::Vector2f first_arm = center - b;
    sf::Vector2f second_arm = center - c;
    return first_arm.x * second_arm.y - first_arm.y * second_arm.x;
}

bool checkIfCrosses(size_t point_index, std::vector<size_t> &poly_i, std::vector<sf::Vector2f> &polygon)
{

    size_t before_index = (point_index == 0) ? poly_i.size() - 1 : point_index - 1;
    size_t after_index = (point_index + 1) % poly_i.size();

    sf::Vector2f &before_point = polygon[poly_i[before_index]];
    sf::Vector2f &point = polygon[poly_i[point_index]];
    sf::Vector2f &after_point = polygon[poly_i[after_index]];

    for (size_t j = 0; j < poly_i.size(); j++)
    {
        auto &check_point = polygon[poly_i[j]];
        if (&check_point != &before_point &&
            &check_point != &point &&
            &check_point != &after_point)
        {

            bool cross_product1 = crossProduct(check_point, before_point, point) > 0;
            bool cross_product2 = crossProduct(check_point, point, after_point) > 0;
            bool cross_product3 = crossProduct(check_point, after_point, before_point) > 0;

            if (cross_product1 == cross_product2 && cross_product2 == cross_product3)
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<size_t> triangulate(std::vector<sf::Vector2f> &polygon)
{
    std::vector<size_t> poly_i(polygon.size());
    std::iota(poly_i.begin(), poly_i.end(), 0);
    std::vector<size_t> indices;

    bool winding_order = getWindingOrder(polygon);

    size_t i = 0;
    while (poly_i.size() > 2)
    {

        size_t index0 = (i == 0) ? poly_i.size() - 1 : i - 1;
        size_t index2 = (i + 1) % poly_i.size();

        auto &before_point = polygon[poly_i[index0]];
        auto &point = polygon[poly_i[i]];
        auto &after_point = polygon[poly_i[index2]];

        float cross_product = crossProduct(point, before_point, after_point);

        if ((cross_product > 0) == winding_order &&
            !checkIfCrosses(i, poly_i, polygon))
        {
            indices.push_back(poly_i[index0]);
            indices.push_back(poly_i[i]);
            indices.push_back(poly_i[index2]);
            poly_i.erase(poly_i.begin() + i);
        }
        i = (i + 1) % poly_i.size();
    }

    return indices;
}