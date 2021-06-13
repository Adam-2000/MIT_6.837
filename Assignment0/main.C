#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ifs.h"
#include "image.h"

int main(int argc, char** argv){
    char *input_file = NULL;
    int num_points = 10000;
    int num_iters = 10;
    int size = 100;
    char *output_file = NULL;
    // sample command line:
    // ifs -input fern.txt -points 10000 -iters 10 -size 100 -output fern.tga
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-points")) {
            i++; assert (i < argc); 
            num_points = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-iters")) {
            i++; assert (i < argc); 
            num_iters = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            size = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
    IFS ifs = IFS();
    ifs.read_from_file(input_file);
    // for (int k = 0; k < ifs.n; k++)z
    // std::cout  << " " << ifs.matrices_ptr[k].Get(0, 2)<< " " << ifs.matrices_ptr[k].Get(1, 2)<< std::endl;
    Image img = Image(size, size);
    ifs.render(img, num_points, num_iters);
    img.SaveTGA(output_file);
    
    return 0;
}
