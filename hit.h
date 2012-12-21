#ifndef _HIT_H
#define _HIT_H

#include "vectors.h"
#include "ray.h"

class Material;

// ====================================================================
// ====================================================================

class Hit {
  
public:

  // CONSTRUCTOR & DESTRUCTOR
  Hit() { material = NULL; }
  Hit(float _t, Material *m, Vec3f n) { 
    t = _t; material = m; normal = n; }
  Hit(const Hit &h) { 
    t = h.t; 
    material = h.material; 
    normal = h.normal;
    intersectionPoint = h.intersectionPoint; }
  ~Hit() {}

  // ACCESSORS
  float getT() const { return t; }
  Material* getMaterial() const { return material; }
  Vec3f getIntersectionPoint() const { return intersectionPoint; }
  Vec3f getNormal() const { return normal; }
  
  // MODIFIER
  void set(float _t, Material *m, Vec3f n, const Ray &ray) {
    t = _t; material = m; normal = n;
    intersectionPoint = ray.pointAtParameter(t); }

private: 

  // REPRESENTATION
  float t;
  Material *material;
  Vec3f intersectionPoint;
  Vec3f normal;
};

inline ostream &operator<<(ostream &os, const Hit &h) {
  os << "Hit <t:" << h.getT() <<">";
  return os;
}
// ====================================================================
// ====================================================================

#endif
