#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "material.h"
#include "ray.h"
#include "hit.h"
#include "boundingbox.h"

class Object3D{

public:

    Object3D(){m = NULL; bbox = NULL;}
    virtual ~Object3D(){
        // if (this->bbox != NULL){
        //     delete this->bbox;
        //     this->bbox = NULL;
        // }
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;
    virtual BoundingBox* getBoundingBox(){ return bbox; }
    virtual void insertIntoGrid(Grid *g, Matrix *m){}
protected:

    Material* m;
    BoundingBox* bbox;
};


#endif /*_OBJECT3D_H_*/

