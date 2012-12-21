#ifndef __PLANE_H_INCLUDED__
#define __PLANE_H_INCLUDED__

class Object3D;

#include "object3d.h"

class Plane : public Object3D
{
    public:
        Plane(Vec3f &n, float &d, Material *t);
        bool intersect(const Ray &r, Hit &h, float tmin);
    private:
        Vec3f normal;
        float offset;
};

#endif // __PLANE_H_INCLUDED__
