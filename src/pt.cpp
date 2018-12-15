#include "pt.h"

#include <iostream>
using namespace std;

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

pt pt::operator / (ptT k) const
{
    return {x / k, y / k};
}

pt pt::operator * (ptT k) const
{
    return {x * k, y * k};
}

std::ostream& operator << (std::ostream &os, const pt &p)
{
    return os << '(' << p.x << ", " << p.y << ')';
}

ptT pt::cross(const pt &a, const pt &b)
{
    return a.x * b.y - a.y * b.x;
}

//Returns > 0 if (o, a, b) clockwise, < 0 if counter-clockwise, 0 if collinear
ptT pt::orientation(const pt &o, const pt &a, const pt &b)
{
    return cross(b - o, a - o);
}

// returns true if [l1, r1] intersects [l2, r2] on number line
bool pt::intersect1d(ptT l1, ptT r1, ptT l2, ptT r2)
{
    if (l1 > r1)
	std::swap(l1, r1);
    if (l2 > r2)
	std::swap(l2, r2);
    return std::max(l1, l2) <= std::min(r1, r2) + EPS;
}

// returns true if line segment p1_q1 intersects line segment p2_q2%
bool pt::intersects(const pt &p1, const pt &q1, const pt &p2, const pt &q2)
{
    return intersect1d(p1.x, q1.x, p2.x, q2.x) and
           intersect1d(p1.y, q1.y, p2.y, q2.y) and
           orientation(p1, q1, p2) * orientation(p1, q1, q2) <= 0 and
           orientation(p2, q2, p1) * orientation(p2, q2, q1) <= 0;
}

bool pt::contains(const std::vector <pt> &polygon, const pt &point)
{   
    bool leftTurn = false;
    bool rightTurn = false;
    int sz = polygon.size();
    for (int i = 0; i < sz; i++)
    {
	int j = (i + 1) % sz;
	auto orient = orientation(polygon[i], polygon[j], point);
	if (orient > 0)
	    rightTurn = true;
	if (orient < 0)
	    leftTurn = true;
    }
    return !(leftTurn and rightTurn);

}
