#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "vectors.h"

class Camera{

public:

    Camera(){}
    ~Camera(){}

    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;

};


class OrthographicCamera : public Camera{

public:

    OrthographicCamera(Vec3f center, Vec3f dir, Vec3f up, float size): center(center), dir(dir), size(size){
        this->dir.Normalize();
        this->up = up - up.Dot3(this->dir) * this->dir;
        this->up.Normalize();
        Vec3f::Cross3(horizontal, this->dir, this->up);
        // std::cout<<this->center<<this->dir<<this->up<<horizontal<<std::endl;
    };
    ~OrthographicCamera(){};

    Ray generateRay(Vec2f point){
        Vec3f orig = center;
        orig += up * (point.y() - 0.5) * size + horizontal * (point.x() - 0.5) * size;
        return Ray(orig, dir);
    }
    float getTMin() const{
        return -100;
    }

private:

    Vec3f up;
    Vec3f dir;
    Vec3f center;
    Vec3f horizontal;
    float size;

};

#endif /*_CAMERA_H_*/