#include "camera.h"

/* Orthographic Camera */
OrthographicCamera::OrthographicCamera(Vec3f c, Vec3f direction, Vec3f u, float s)
{
    center = c;
    dir = direction; dir.Normalize(); // This is using sqrt, try quake appox?
    // Make orthonormal
    if(u.Dot3(dir) != 0)
    {
        Vec3f t;
        Vec3f::Cross3(t,u,dir);
        Vec3f::Cross3(up,dir,t);
    }
    else
        up = u;

    up.Normalize();
    
    size = s;
    Vec3f::Cross3(horz, dir, up);
    horz.Normalize();
}

OrthographicCamera::~OrthographicCamera() {}

Ray OrthographicCamera::generateRay(Vec2f point)
{
    //0,0 -> 1,1 :: -.5,-.5 -> .5,.5
    //center + y*size*up + x*size*horz;
    Vec3f p(center + (point.y()-.5f)*(size*up) + (point.x()-.5f)*(size*horz));
    return (Ray(p,dir));
}

float OrthographicCamera::getTMin() const
{
    return -FLT_MAX;
    //return log(0); // -&infin
}

/* Perspective Camera */

PerspectiveCamera::PerspectiveCamera(Vec3f c, Vec3f direction, Vec3f u, float a)
{
    center = c;
    dir = direction; dir.Normalize();
    up = u; up.Normalize();
    Vec3f::Cross3(horz, dir, u); horz.Normalize();
    //Vec3f::Cross3(up, dir, horz);
    angle = a;
}

PerspectiveCamera::~PerspectiveCamera() {}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    float size = 2.0f*tan(angle/2.0f);
    Vec3f c = center + dir;
    Vec3f p(c + (point.y()-.5f)*(size*up) + (point.x()-.5f)*(size*horz));
    Vec3f d = p - center;

    return Ray(center,d);
}

float PerspectiveCamera::getTMin() const
{
    return 1e-6; // min representable by float close to 0
}
