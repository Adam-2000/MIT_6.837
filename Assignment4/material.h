#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "glCanvas.h"
#include "hit.h"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(){}
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }

  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
     const Vec3f &lightColor) const = 0;
  virtual void glSetMaterial(void) const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  
};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent, const Vec3f &reflectiveColor, 
                    const Vec3f &transparentColor, float indexOfRefraction):
                specularColor(specularColor), exponent(exponent), reflectiveColor(reflectiveColor), 
                    transparentColor(transparentColor), indexOfRefraction(indexOfRefraction){
    this->diffuseColor = diffuseColor;
  }
  virtual ~PhongMaterial() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f getSpecularColor() const { return specularColor; }
  virtual Vec3f getReflectiveColor() const { return reflectiveColor; }
  virtual Vec3f getTransparentColor() const { return transparentColor; }

  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
          const Vec3f &lightColor) const{
    Vec3f ret_color;
    Vec3f h = (dirToLight - ray.getDirection());
    h.Normalize();
    ret_color = specularColor * std::pow(h.Dot3(hit.getNormal()), exponent) * lightColor;
    return ret_color;
  };

  void glSetMaterial(void) const;
protected:

  // REPRESENTATION
  Vec3f specularColor;
  Vec3f reflectiveColor;
  Vec3f transparentColor;
  float indexOfRefraction;
  float exponent;
};

#endif
