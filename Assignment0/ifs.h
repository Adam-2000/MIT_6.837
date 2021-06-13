#ifndef _IFS_H_
#define _IFS_H_
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "vectors.h"
#include "matrix.h"
#include "image.h"
class IFS {

public:
    // ========================
    // CONSTRUCTOR & DESTRUCTOR
    IFS(){};
    IFS(int num): n(num) {
        matrices_ptr = new Matrix[n];
        probs = new float[10];
    }
    ~IFS() {
        if (matrices_ptr != NULL){
            delete [] matrices_ptr;
            matrices_ptr = NULL;
        }
             
        if (probs != NULL){
            delete [] probs; 
            probs = NULL;
        }
    }

    void read_from_file(const char* filename);
    void render(Image& img, int n_pts, int n_iters);
  
private:

    // ==============
    // REPRESENTATION
    int n;
    Matrix* matrices_ptr;
    float* probs;

};

#endif /*_IFS_H_*/