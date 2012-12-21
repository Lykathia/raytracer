#ifndef __GROUP_H_INCLUDED__
#define __GROUP_H_INCLUDED__

class Object3D;

#include <vector>
#include "object3d.h"

class Group : public Object3D
{
    public:
        Group(int n);
        ~Group();
        virtual bool intersect(const Ray &r, Hit &h, float tmin);
        void addObject(int i, Object3D *obj);
    private:
        int numObjects;
        vector<Object3D*> index;
};

#endif // __GROUP_H_INCLUDED__
