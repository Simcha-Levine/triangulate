bool inBox(float width, float hight, float x, float y)
{
    return x > 0 && x < width && y > 0 && y < hight;
}

float distance(float ax, float ay, float bx, float by)
{
    return sqrt(pow(abs(ax - bx), 2) + pow(abs(ay - by), 2));
}