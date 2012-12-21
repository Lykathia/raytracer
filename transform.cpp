#include "transform.h"

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Matrix t = m; t.Inverse(1e-4);
    Vec3f s=r.getOrigin(),d=r.getDirection();
    t.Transform(s); t.TransformDirection(d);

    Ray rr(s,d); // New ray
    if(o->intersect(rr,h,tmin))
    {
        Vec3f n = h.getNormal();
        t.Transpose(); t.Transform(n);
        n.Normalize();

        h.set(h.getT(),h.getMaterial(),n,r);
        return true;
    }
    return false;
}

