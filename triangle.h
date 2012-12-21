#ifndef __TRIANGLE_H_INCLUDED__
#define __TRIANGLE_H_INCLUDED__

class Object3D;

#include "matrix.h"
#include "object3d.h"

class Triangle : public Object3D
{
    public:
        Triangle(Vec3f &aa, Vec3f &bb, Vec3f &cc, Material *t);
        bool intersect(const Ray &r, Hit &h, float tmin);
    private:
        Vec3f a,b,c,norm;
        float offset;
};

#endif // __TRIANGLE_H_INCLUDED__
