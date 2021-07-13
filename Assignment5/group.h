#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"
#include "vectors.h"
class Group: public Object3D{

public:
    Group(int n): n_obj(n){
        obj = new Object3D*[n];
    }
    ~Group(){
        delete [] obj;
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