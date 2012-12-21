#ifndef __RAYTRACER_H_INCLUDED__
#define __RAYTRACER_H_INCLUDED__

class SceneParser;
class Material;
class Ray;
class Hit;

#include <cfloat>
#include <cmath>
#include <algorithm>
#include "scene_parser.h"
#include "material.h"
#include "ray.h"
#include "hit.h"
#include "light.h"
#include "group.h"
#include "camera.h"

class RayTracer
{
    public:
        RayTracer(SceneParser *sc, int b, bool shadows, bool bs);
        const Vec3f castRay(Ray &r, Hit &h, float tmin, int n, float refracIndex);
        const Vec3f antiAlias(Vec2f pixel, float width, float height);
        float specularExponent(Ray r, Hit h, Vec3f dir);
        float clamp(float a);
    private:
        SceneParser *s;
        bool back_shading, noshadows;
        int max_bounce;
};

#endif //__RAYTRACER_H_INCLUDED__
