#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "glCanvas.h"
#include "hit.h"
#include "matrix.h"
#include "perlin_noise.h"

#define EPSILON 0.001

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
  virtual void setDiffuseColor(const Vec3f &diffuseColor) {this->diffuseColor = diffuseColor;}
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f getSpecularColor() const = 0;
  virtual Vec3f getReflectiveColor() const = 0;
  virtual Vec3f getTransparentColor() const = 0;
  virtual float getIndexOfRefraction() const = 0;
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
  ~PhongMaterial() {}
  void setDiffuseColor(const Vec3f &diffuseColor) {this->diffuseColor = diffuseColor;}
  // ACCESSORS
  Vec3f getDiffuseColor() const { return diffuseColor; }
  Vec3f getSpecularColor() const { return specularColor; }
  Vec3f getReflectiveColor() const { return reflectiveColor; }
  Vec3f getTransparentColor() const { return transparentColor; }
  float getIndexOfRefraction() const { return indexOfRefraction; }
  Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
          const Vec3f &lightColor) const{
    float cos = dirToLight.Dot3(hit.getNormal());
    Vec3f ret_color(0, 0, 0);
    if(dirToLight.Length() <= EPSILON){
      return lightColor * diffuseColor;
    }
    if(cos <= 0){
      return ret_color;
    }
    Vec3f h = (dirToLight - ray.getDirection());
    h.Normalize();
    ret_color = specularColor * std::pow(h.Dot3(hit.getNormal()), exponent) * lightColor + diffuseColor * lightColor * cos;
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

class Checkerboard : public Material{

public: 
    Checkerboard(Matrix *m, Material *mat1, Material *mat2):
                    m(m), mat1(mat1), mat2(mat2){}
    ~Checkerboard(){if (m != NULL) delete m;}

    void glSetMaterial()const{mat1->glSetMaterial();}
    Vec3f getDiffuseColor() const { return mat1->getDiffuseColor(); }
    Vec3f getSpecularColor() const { return mat1->getSpecularColor(); }
    Vec3f getReflectiveColor() const { return mat1->getReflectiveColor(); }
    Vec3f getTransparentColor() const { return mat1->getTransparentColor(); }
    float getIndexOfRefraction() const { return mat1->getIndexOfRefraction(); }
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
          const Vec3f &lightColor) const{
      Vec3f p_intsct = hit.getIntersectionPoint();
      m->TransformPoint(p_intsct);
      bool p_bool = ((int)floor(p_intsct[0]) & 1) ^ ((int)floor(p_intsct[1]) & 1) ^ ((int)floor(p_intsct[2]) & 1);
      if(!p_bool){
        return mat1->Shade(ray, hit, dirToLight, lightColor);
      } else {
        return mat2->Shade(ray, hit, dirToLight, lightColor);
      }
    }

private:
    Matrix *m;
    Material *mat1; 
    Material *mat2;

};

class Noise : public Material{

public: 
    Noise(Matrix *m, Material *mat1, Material *mat2, int octaves):
              m(m), mat1(mat1), mat2(mat2), octaves(octaves){};
    ~Noise(){if (m != NULL) delete m;}

    void glSetMaterial()const{mat1->glSetMaterial();}
    Vec3f getDiffuseColor() const { return mat1->getDiffuseColor(); }
    Vec3f getSpecularColor() const { return mat1->getSpecularColor(); }
    Vec3f getReflectiveColor() const { return mat1->getReflectiveColor(); }
    Vec3f getTransparentColor() const { return mat1->getTransparentColor(); }
    float getIndexOfRefraction() const { return mat1->getIndexOfRefraction(); }
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
          const Vec3f &lightColor) const{
      Vec3f p_intsct = hit.getIntersectionPoint();
      m->TransformPoint(p_intsct);
      float x = p_intsct.x(), y = p_intsct.y(), z = p_intsct.z();
      double perlin_rand = 0;
      for(int i = 1; i <= octaves; i++){
        perlin_rand += PerlinNoise::noise(i*x, i*y, i*z);
      }
      return perlin_rand * mat1->Shade(ray, hit, dirToLight, lightColor) + 
                (1 - perlin_rand) * mat2->Shade(ray, hit, dirToLight, lightColor);
    }

private:
    Matrix *m;
    Material *mat1; 
    Material *mat2;
    int octaves;

};

class Marble : public Material{

public: 
    Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude):
              m(m), mat1(mat1), mat2(mat2), octaves(octaves), frequency(frequency), amplitude(amplitude){};
    ~Marble(){if (m != NULL) delete m;}

    void glSetMaterial()const{mat1->glSetMaterial();}
    Vec3f getDiffuseColor() const { return mat1->getDiffuseColor(); }
    Vec3f getSpecularColor() const { return mat1->getSpecularColor(); }
    Vec3f getReflectiveColor() const { return mat1->getReflectiveColor(); }
    Vec3f getTransparentColor() const { return mat1->getTransparentColor(); }
    float getIndexOfRefraction() const { return mat1->getIndexOfRefraction(); }
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
          const Vec3f &lightColor) const{
      Vec3f p_intsct = hit.getIntersectionPoint();
      m->TransformPoint(p_intsct);
      float x = p_intsct.x(), y = p_intsct.y(), z = p_intsct.z();
      double perlin_rand = 0;
      for(int i = 1; i <= octaves; i++){
        perlin_rand += PerlinNoise::noise(i*x, i*y, i*z);
      }
      perlin_rand = sin(frequency * x + amplitude* perlin_rand);
      return perlin_rand * mat1->Shade(ray, hit, dirToLight, lightColor) + 
                (1 - perlin_rand) * mat2->Shade(ray, hit, dirToLight, lightColor);
    }

private:
    Matrix *m;
    Material *mat1; 
    Material *mat2;
    int octaves;
    float frequency;
    float amplitude;
};

class Wood : public Material{

public: 
    Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude):
              m(m), mat1(mat1), mat2(mat2), octaves(octaves), frequency(frequency), amplitude(amplitude){};
    ~Wood(){if (m != NULL) delete m;}
    void glSetMaterial()const{mat1->glSetMaterial();}
    Vec3f getDiffuseColor() const { return mat1->getDiffuseColor(); }
    Vec3f getSpecularColor() const { return mat1->getSpecularColor(); }
    Vec3f getReflectiveColor() const { return mat1->getReflectiveColor(); }
    Vec3f getTransparentColor() const { return mat1->getTransparentColor(); }
    float getIndexOfRefraction() const { return mat1->getIndexOfRefraction(); }
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
          const Vec3f &lightColor) const{
      Vec3f p_intsct = hit.getIntersectionPoint();
      m->TransformPoint(p_intsct);
      float x = p_intsct.x(), y = p_intsct.y(), z = p_intsct.z();
      double perlin_rand = 0;
      for(int i = 1; i <= octaves; i++){
        perlin_rand += PerlinNoise::noise(i*x, i*y, i*z);
      }
      perlin_rand = sin(frequency * x + amplitude* perlin_rand);
      return perlin_rand * mat1->Shade(ray, hit, dirToLight, lightColor) + 
                (1 - perlin_rand) * mat2->Shade(ray, hit, dirToLight, lightColor);
    }

private:
    Matrix *m;
    Material *mat1; 
    Material *mat2;
    int octaves;
    float frequency;
    float amplitude;

};


#endif
