#ifndef __CAMERA_H_INCLUDED__
#define __CAMERA_H_INCLUDED__

#include <cmath>
#include <cfloat> // for FLT_MAX
#include "ray.h"
#include "vectors.h"

class Camera
{
    public:
        virtual Ray generateRay(Vec2f point) = 0;
        virtual float getTMin() const = 0;
};

class OrthographicCamera : public Camera
{
    public:
        OrthographicCamera(Vec3f c, Vec3f direction, Vec3f u, float s);
        ~OrthographicCamera();
        Ray generateRay(Vec2f point);
        float getTMin() const;

    private:
        Vec3f center;
        Vec3f dir;
        Vec3f up;
        Vec3f horz;
        float size;

};

class PerspectiveCamera : public Camera
{
    public:
        PerspectiveCamera(Vec3f c, Vec3f direction, Vec3f u, float a);
        ~PerspectiveCamera();
        Ray generateRay(Vec2f point);
        float getTMin() const;
    private:
        Vec3f center, dir, up, horz;
        float angle;
};

#endif //__CAMERA_H_INCLUDED__
