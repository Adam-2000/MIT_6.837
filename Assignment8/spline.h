#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "triangle_mesh.h"
#include "vectors.h"

class Spline {

public:
    Spline(){vertices = NULL;}
    ~Spline(){
        if(vertices!=NULL){
            delete [] vertices;
            vertices = NULL;
        }
    }

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) = 0;

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file){
        fprintf (file, "bezier\n");
        fprintf (file, "num_vertices %d\n", n_vertices);
        for (int i = 0; i < n_vertices; i++) {
            fprintf (file, "%lf %lf %lf\n", vertices[i].x(), vertices[i].y(), vertices[i].z());
        }
    }
    virtual void OutputBSpline(FILE *file){
        fprintf (file, "bspline\n");
        fprintf (file, "num_vertices %d\n", n_vertices);
        for (int i = 0; i < n_vertices; i++) {
            fprintf (file, "%lf %lf %lf\n", vertices[i].x(), vertices[i].y(), vertices[i].z());
        }
    }

    // FOR CONTROL POINT PICKING
    int getNumVertices(){return n_vertices;}
    Vec3f getVertex(int i){
        assert(0<=i && i < n_vertices);
        return vertices[i];
    }

    virtual void set(int i, Vec3f v){
        assert(i>=0 && i < n_vertices);
        vertices[i] = v;
    }
    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y) = 0;
    virtual void addControlPoint(int selectedPoint, float x, float y) = 0;
    virtual void deleteControlPoint(int selectedPoint) = 0;

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser *args){};

protected:
    int n_vertices;
    Vec3f* vertices;

};



#endif

