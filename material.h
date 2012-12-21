#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color, const Vec3f &s_color, int e, const Vec3f &r_color, const Vec3f &t_color, int ir)
    { diffuseColor = d_color; specularColor = s_color; exponent = e; reflectiveColor = r_color; transparentColor = t_color; indexOfRefraction = ir; }
  virtual ~Material() {}

  // ACCESSORS
  Vec3f getDiffuseColor() const { return diffuseColor; }
  Vec3f getSpecularColor() const { return specularColor; }
  int getExponent() const { return exponent; }
  float getRefractionIndex() const { return indexOfRefraction; }
  Vec3f getTransparentColor() const { return transparentColor; }
  Vec3f getReflectiveColor() const { return reflectiveColor; }

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  Vec3f specularColor;
  int exponent;
  Vec3f transparentColor;
  Vec3f reflectiveColor;
  float indexOfRefraction;
};

// ====================================================================
// ====================================================================

#endif
