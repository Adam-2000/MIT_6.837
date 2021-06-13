#include "ifs.h"


void IFS::read_from_file(const char* filename){
    FILE *input = fopen(filename,"r");
    assert(input != NULL);

    // read the number of transforms
    int num_transforms; 
    fscanf(input,"%d",&num_transforms);
    
    // < DO SOMETHING WITH num_transforms >
    n = num_transforms;
    matrices_ptr = new Matrix[n];
    probs = new float[10];
    // read in the transforms
    for (int i = 0; i < num_transforms; i++) {
        float probability; 
        fscanf (input,"%f",&probability);
        Matrix m; 
        m.Read3x3(input);
        // < DO SOMETHING WITH probability and m >
        matrices_ptr[i] = m;
        probs[i] = probability;
    }

    // close the file
    fclose(input);
}

void IFS::render(Image& img, int n_pts, int n_iters){
    Vec2f point;
    float p;
    int idx;
    int i, j, k;
    int u, v;
    // std::cout << img.Width() << " " <<  img.Height() << std::endl;
    for (i = 0; i < n_pts; i++){
        point = Vec2f(drand48(), drand48());
        for (j = 0; j < n_iters; j++){
            p = (float) drand48();
            for (k = 0; k < n; k++){
                p -= probs[k];
                if (p < 0){
                    break;
                }
            }
            matrices_ptr[k].Transform(point);
        }
        u = point.x() * img.Width();
        v = point.y() * img.Height();
        // // std::cout << u << " " <<  v << std::endl;
        u = (u >= 0) ? u % img.Width() : u % img.Width() + img.Width();
        v = (v >= 0) ? v % img.Height() : v % img.Height() + img.Height();
        // std::cout << u << " " <<  v << std::endl;
        img.SetPixel(u, v, Vec3f(255, 255, 255));
    }
}
