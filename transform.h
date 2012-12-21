#ifndef __TRANSFORM_H_INCLUDED__
#define __TRANSFORM_H_INCLUDED__

class Matrix;
class Object3D;

#include "object3d.h"
#include "matrix.h"

class Transform : public Object3D
{
    public:
        Transform(Matrix &mat, Object3D *&obj) { m = mat; o = obj;}
        bool intersect(const Ray &r, Hit &h, float tmin);
    private:
        Matrix m;
        Object3D *o;
};

#endif //__TRANSFORM_H_INCLUDED__
