#ifndef __SPHERE_H_INCLUDED__
#define __SPHERE_H_INCLUDED__

class Object3D;

#include "object3d.h"

class Sphere : public Object3D
{
    public:
        Sphere(Vec3f c, float r, Material* t);
        bool intersect(const Ray &r, Hit &h, float tmin);
    private:
        Vec3f center;
        float radius;
};

#endif // __SPHERE_H_INCLUDED__
