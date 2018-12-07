#include "pt.h"

const float EPS = 1e-5;

/*
 * Compares floats with epsilon margin checking
 * if a > b, returns 1
 * if a < b, returns -1
 * if a = b, returns 0
 */
int cmp(ptT a, ptT b)
{
    return (a > b + EPS ? 1 : (a < b - EPS ? -1 : 0));
}

bool pt::operator < (const pt &o) const
{
    if (cmp(o.x, x) != 0) return cmp(x, o.x) == -1;
    else return cmp(y, o.y) == -1;
}

bool pt::operator == (const pt &o) const
{
    return cmp(x, o.x) == 0 and cmp(y, o.y) == 0;
}

bool pt::operator != (const pt &o) const
{
    return !(*this == o);
}

pt pt::operator + (const pt &o) const
{
    return {x + o.x, y + o.y};
}

pt pt::operator - (const pt &o) const
{
    return {x - o.x, y - o.y};
}

std::ostream& operator << (std::ostream &os, const pt &p)
{
    return os << '(' << p.x << ", " << p.y << ')';
}

ptT cross(const pt &a, const pt &b)
{
    return a.x * b.y - a.y * b.x;
}

//Returns > 0 if (o, a, b) clockwise, < 0 if counter-clockwise, 0 if collinear
ptT orientation(const pt &o, const pt &a, const pt &b)
{
    return cross(b - o, a - o);
}
