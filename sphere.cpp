#include "sphere.h"

Sphere::Sphere(Vec3f c, float r, Material* t)
{
    texture = t;
    center = c;
    radius = r;
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    // t = -(v.d) +- sqrt(v.d^2 - (v^2 -r^2))
    // -b +- sqrt(b^2 - 4ac) / 2a
    // ax^2 + bx + c = 0
    float a = r.getDirection().Dot3(r.getDirection());
    float b = 2.0f * r.getDirection().Dot3((r.getOrigin()-center));
    float c = (r.getOrigin()-center).Dot3((r.getOrigin()-center)) - (radius*radius);

    // calc discriminant
    float disc = b*b-4.0f*a*c;

    // No root
    if(disc < EPS)
        return false;

    float q = (-b - sqrt(disc));
    float u = (-b + sqrt(disc));

    float s = max(q,u)/(2.0f*a);
    float t = min(q,u)/(2.0f*a);

    if(s < tmin) return false;

    q = (t < tmin ? s : t);

    if(q >= h.getT())
        return false;

    Vec3f n = (r.pointAtParameter(q)-center); n.Normalize();
    h.set(q, texture, n, r);
    return true;
}
