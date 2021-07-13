#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "material.h"
#include "ray.h"
#include "hit.h"

class Object3D{

public:

    Object3D(){m = NULL;}
    virtual ~Object3D(){}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;

protected:

    Material* m;

};


#endif /*_OBJECT3D_H_*/

