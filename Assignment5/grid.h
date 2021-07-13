#ifndef _GRID_H_
#define _GRID_H_

#include "object3d.h"
#include "vectors.h"

class Grid: public Object3D{

public:
    Grid(BoundingBox *bb, int nx, int ny, int nz): nx(nx), ny(ny), nz(nz) {
        this->bbox = bb;
        occ_array = new bool[nx*ny*nz];
        for (int i = 0; i < nx*ny*nz; i++){
            occ_array[i] = false;
        }
    }
    ~Grid(){delete [] occ_array;}

    void set_array(bool val, int x, int y, int z){
        occ_array[x * ny * nz + y * nz + nz] = val;
    }
    bool get_array(bool val, int x, int y, int z){
        return occ_array[x * ny * nz + y * nz + nz];
    }
    void get_n(int &x, int &y, int &z){
        x = nx;
        y = ny;
        z = nz;
    }
    bool intersect(const Ray &r, Hit &h, float tmin){}
    void paint(){}
private:

    int nx; 
    int ny; 
    int nz;
    bool* occ_array;

};

#endif
