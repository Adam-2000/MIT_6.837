#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "vectors.h"

// ==================================================================
// A helper class for super-sampling.  The position specifies the 2D
// offset within the pixel from (0,0) -> (1,1).

class Sample {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Sample () { 
    position = Vec2f(0.5, 0.5);
    color = Vec3f(0,0,0); }
  ~Sample () {}

  // ACCESSORS
  Vec2f getPosition() { return position; }
  Vec3f getColor() { return color; }

  // MODIFIERS
  void set(Vec2f p, Vec3f c) { 
    assert (p.x() >= 0 && p.x() <= 1);
    assert (p.y() >= 0 && p.y() <= 1);
    position = p;
    color = c; }

private:

  // REPRESENTATION
  Vec2f position;
  Vec3f color;

};


class Sampler{

public:
  Sampler(){}
  Sampler(int n_samples):n_samples(n_samples){}
  ~Sampler(){}

  virtual Vec2f getSamplePosition(int n) = 0;


protected:
  int n_samples;

};

class RandomSampler: public Sampler{

public:
  RandomSampler(int n_samples){
    this->n_samples = n_samples;
  }
  ~RandomSampler(){}

  Vec2f getSamplePosition(int n){
    return Vec2f(drand48(), drand48());
  }

};

class UniformSampler: public Sampler{

public:
  UniformSampler(int n_samples){
    this->n_samples = n_samples;
    n_sqrt = (int)sqrt(n_samples);
    d = 1.0 / n_sqrt;
    start_p = d / 2.0;
  }
  ~UniformSampler(){}

  Vec2f getSamplePosition(int n){
    return Vec2f(start_p + d * (n % n_sqrt), start_p + d * (n / n_sqrt));
  }

private:
  float start_p;
  float d;
  int n_sqrt;
};

class JitteredSampler: public Sampler{

public:
  JitteredSampler(int n_samples){
    this->n_samples = n_samples;
    n_sqrt = (int)sqrt(n_samples);
    d = 1.0 / n_sqrt;
  }
  ~JitteredSampler(){}

  Vec2f getSamplePosition(int n){
    return Vec2f(d * (n % n_sqrt + drand48()), d * (n / n_sqrt + drand48()));
  }

private:
  float d;
  int n_sqrt;
};



// ==================================================================

#endif
