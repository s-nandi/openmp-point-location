#ifndef PT_DEFINED
#define PT_DEFINED

#include <iostream>

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

    friend ptT cross(const pt &a, const pt &b);
    friend ptT orientation(const pt &o, const pt &a, const pt &b);
    friend std::ostream& operator << (std::ostream &os, const pt &p);
};

#endif
