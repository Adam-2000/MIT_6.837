#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "vectors.h"

// #define MY_PI 3.141592653589793238462

class Camera{

public:

    Camera(){}
    ~Camera(){}

    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;

    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;



};


class OrthographicCamera : public Camera{

public:

    OrthographicCamera(Vec3f &center, Vec3f &dir, Vec3f &up, float size): center(center), direction(dir), size(size){
        this->direction.Normalize();
        this->up = up - up.Dot3(this->direction) * this->direction;
        this->up.Normalize();
        Vec3f::Cross3(horizontal, this->direction, this->up);
        // std::cout<<this->center<<this->dir<<this->up<<horizontal<<std::endl;
    };
    ~OrthographicCamera(){};

    Ray generateRay(Vec2f point){
        Vec3f orig = center;
        orig += up * (point.y() - 0.5) * size + horizontal * (point.x() - 0.5) * size;
        return Ray(orig, direction);
    }
    float getTMin() const{
        return -10000;
    }

    void glInit(int w, int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);


private:

    Vec3f up;
    Vec3f direction;
    Vec3f center;
    Vec3f horizontal;
    float size;

};

class PerspectiveCamera : public Camera{

public:

    PerspectiveCamera(Vec3f &center, Vec3f &dir, Vec3f &up, float angle): center(center), direction(dir), angle(angle){
        this->direction.Normalize();
        this->up = up - up.Dot3(this->direction) * this->direction;
        this->up.Normalize();
        Vec3f::Cross3(horizontal, this->direction, this->up);
        dist_e2s = 0.5 / std::tan(angle / 2);
        // std::cout<<this->center<<this->dir<<this->up<<horizontal<<std::endl;
    };
    ~PerspectiveCamera(){};

    Ray generateRay(Vec2f point){
        Vec3f ray_dir = direction * dist_e2s;
        ray_dir += up * (point.y() - 0.5) + horizontal * (point.x() - 0.5);
        ray_dir.Normalize();
        return Ray(center, ray_dir);
    }
    float getTMin() const{
        return 0;
    }

    void glInit(int w, int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);

private:

    Vec3f up;
    Vec3f direction;
    Vec3f center;
    Vec3f horizontal;
    float angle;
    float dist_e2s;

};

#endif /*_CAMERA_H_*/