
class Triangulate
{
public:
    float trapezoid(int i)
    {
        auto &current = polygon[i];
        auto &next = polygon[(i + 1) % polygon.size()];
        return (next.x - current.x) * (next.y + current.y);
    }

    bool getWindingOrder()
    {
        float total = 0;
        for (size_t i = 0; i < polygon.size(); i++)
        {
            total += trapezoid(i);
        }
        return total > 0;
    }

    float crossProduct(sf::Vector2f &center, sf::Vector2f &before, sf::Vector2f &after)
    {
        sf::Vector2f first_arm = center - before;
        sf::Vector2f second_arm = center - after;
        return first_arm.x * second_arm.y - first_arm.y * second_arm.x;
    }

private:
    float crossProductFromIPoly(size_t center, size_t b, size_t c)
    {
        return crossProduct(polygon[poly_i[center]], polygon[poly_i[b]], polygon[poly_i[c]]);
    }

    bool checkIfCrosses(size_t point_index)
    {

        size_t before_index = move_i(poly_i.size(), point_index, -1);
        size_t after_index = move_i(poly_i.size(), point_index, 1);

        for (size_t j = 0; j < poly_i.size(); j++)
        {

            auto check_point = poly_i[j];
            if (check_point != poly_i[before_index] &&
                check_point != poly_i[point_index] &&
                check_point != poly_i[after_index] &&
                (crossProductFromIPoly(j, move_i(poly_i.size(), j, -1), move_i(poly_i.size(), j, 1)) > 0) != winding_order)
            {
                bool cross_product1 = crossProductFromIPoly(j, before_index, point_index) > 0;
                bool cross_product2 = crossProductFromIPoly(j, point_index, after_index) > 0;
                bool cross_product3 = crossProductFromIPoly(j, after_index, before_index) > 0;

                if (cross_product1 == cross_product2 && cross_product2 == cross_product3)
                {
                    return true;
                }
            }
        }
        return false;
    }

public:
    VectorArray &polygon;
    std::vector<size_t> poly_i;
    bool winding_order;

    Triangulate(VectorArray &polygon) : polygon(polygon) {}

    void set()
    {
        while (!poly_i.empty())
            poly_i.pop_back();

        for (size_t i = 0; i < polygon.size(); i++)
            poly_i.push_back(i);

        winding_order = getWindingOrder();
    }

    std::vector<size_t> triangulate()
    {
        set();

        std::vector<size_t> indices;

        size_t i = 0;
        int count = 0;
        while (poly_i.size() > 2 && count < 100)
        {

            count++;
            size_t before = move_i(poly_i.size(), i, -1);
            size_t after = move_i(poly_i.size(), i, 1);

            float cross_product = crossProductFromIPoly(i, before, after);

            if ((cross_product > 0) == winding_order && !checkIfCrosses(i))
            {
                indices.push_back(poly_i[i]);
                indices.push_back(poly_i[before]);
                indices.push_back(poly_i[after]);
                poly_i.erase(poly_i.begin() + i);
            }
            i = (i + 1) % poly_i.size();
        }

        return indices;
    }
};