#include "triangle.h"

Triangle::Triangle(Vec3f &aa, Vec3f &bb, Vec3f &cc, Material *t)
{
    a = aa; b = bb; c = cc; texture = t;
    Vec3f::Cross3(norm,b-a,c-a); norm.Normalize();
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f r1=a-b;
    Vec3f r2=a-c;
    Vec3f r3=r.getDirection();
    Vec3f r4=a-r.getOrigin();

    float d=det3x3(r1.x(),r2.x(),r3.x(),
                   r1.y(),r2.y(),r3.y(),
                   r1.z(),r2.z(),r3.z());
    float u=det3x3(r4.x(),r2.x(),r3.x(),
                   r4.y(),r2.y(),r3.y(),
                   r4.z(),r2.z(),r3.z())/d;
    float v=det3x3(r1.x(),r4.x(),r3.x(),
                   r1.y(),r4.y(),r3.y(),
                   r1.z(),r4.z(),r3.z())/d;

    float t=det3x3(r1.x(),r2.x(),r4.x(),
                   r1.y(),r2.y(),r4.y(),
                   r1.z(),r2.z(),r4.z())/d;

    if ((u+v)<1.0f && (u > 0.0f) && (v > 0.0f))
        if (t>=tmin)
            if (t<h.getT())
            {
                h.set(t,texture,norm,r);
                return true;
            }
    
    return false;
}
