#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"
#include "vectors.h"
class Group: public Object3D{

public:
    Group(int n): n_obj(n){
        obj = new Object3D*[n];
        this->bbox = NULL;
    }
    ~Group(){
        for(int i = 0; i < n_obj; i++){
            delete obj[i];
        }
        delete [] obj;
        if (this->bbox != NULL){
            delete this->bbox;
            this->bbox = NULL;
        }
    }

    bool intersect(const Ray &r, Hit &h, float tmin){
        bool flag = false;
        int i;
        for (i = 0; i < n_obj; i++){
            flag |= obj[i]->intersect(r, h, tmin);
        }
        return flag;
    }

    void addObject(int index, Object3D *obj){
        if (index >= 0 && index < n_obj){
            this->obj[index] = obj;
            if (obj->getBoundingBox() != NULL){
                if (this->bbox == NULL){
                    this->bbox = new BoundingBox(obj->getBoundingBox()->getMin(), obj->getBoundingBox()->getMax());
                } else {
                    this->bbox->Extend(obj->getBoundingBox());
                }
            }
        }
    }

    void insertIntoGrid(Grid *g, Matrix *m){
        for (int i = 0; i < n_obj; i++){
            obj[i]->insertIntoGrid(g, m);
        }
    }

    virtual void paint(){
        int i;
        for (i = 0; i < n_obj; i++){
            obj[i]->paint();
        }
    }

private:

    Object3D** obj;
    int n_obj;

};

#endif /*_GROUP_H_*/