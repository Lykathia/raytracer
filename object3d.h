#ifndef __OBJECT3D_H_INCLUDED__
#define __OBJECT3D_H_INCLUDED__

#define EPS 1e-4

class Ray;
class Hit;

#include "ray.h"
#include "hit.h"

class Object3D
{
    public:
        virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0; 

    protected:
        Material *texture;
};

#endif // __OBJECT3D_H_INCLUDED__
