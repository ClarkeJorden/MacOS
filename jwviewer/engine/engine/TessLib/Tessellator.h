#ifndef TESSELLATOR_H
#define TESSELLATOR_H

#include <vector>
#include <list>
#include <stdio.h>
#include "glu.h"

class Tesselator
{
public:
    struct v3_t {
        GLdouble x,y,z;
        v3_t() : x(0), y(0), z(0) {}
        void print() const {
            printf("(%f, %f)\n", x, y);
        }
    };
    
public:
    Tesselator();
    ~Tesselator();
    
    void beginPolygon();
    
    void feedvert( double* vert );
    
    void tesselate(std::vector< GLfloat >& triangles);
    
    void beginContour();
    void endContour();
    
    GLenum primType() {
        return _primType;
    }
    void setPrimType( GLenum t ) {
        _primType = t;
    }
    void addVertex(GLdouble* v);
    void addTessVertex( const v3_t& v ) {
        _tessVertices.push_back( v );
    }
    GLdouble* tessVerticesBackPtr() {
        return &(_tessVertices.back().x);
    }
    
    static void error(GLenum error);
    static void tessBegin( GLenum type, GLvoid* user );
    static void tessEnd( GLvoid* user );
    static void tessVertex( GLvoid* vertex, GLvoid* user );
    static void combine(GLdouble coords [3], void *vertex_data[4],
                        GLfloat weight[4], void **outData, void* userdata);
    
private:
    GLenum				_primType;
    std::vector<GLfloat> _vertices;
    std::list<v3_t>		_tessVertices;
    GLUtesselator* _tessobj;
};

#endif // TESSELLATOR_H
