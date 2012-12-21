#include "group.h"

Group::Group(int n) { numObjects = n; index.resize(n); }

Group::~Group() { index.clear(); }

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool __r = false; // Almost forgout about return -.-
    for(int i=0;i<numObjects;i++)
    {
        if(index[i]->intersect(r,h,tmin))
            __r = true;
    }
    return __r;
}

void Group::addObject(int i, Object3D *obj)
{
    assert(i < numObjects);
    index[i] = obj;
}

