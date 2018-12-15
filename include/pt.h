#ifndef PT_DEFINED
#define PT_DEFINED

#include <iostream>
#include <vector>

typedef float ptT;
struct pt
{
    ptT x, y;

    pt(){}
    pt(ptT _x, ptT _y) : x(_x), y(_y) {}
    
    bool operator < (const pt &o) const;
    bool operator == (const pt &o) const;
    bool operator != (const pt &o) const;
    pt operator + (const pt &o) const;
    pt operator - (const pt &o) const;
    pt operator / (ptT k) const;
    pt operator * (ptT k) const;

    static ptT dot(const pt &a, const pt &b);
    static ptT cross(const pt &a, const pt &b);

    static ptT distance(const pt &a, const pt &b);
    static ptT orientation(const pt &o, const pt &a, const pt &b);
    static bool intersect1d(ptT i1, ptT j1, ptT i2, ptT j2);
    static bool intersects(const pt &p1, const pt &q1, const pt &p2, const pt &q2);
    static bool contains(const std::vector <pt> &polygon, const pt &point);
    friend std::ostream& operator << (std::ostream &os, const pt &p);
};

#endif
