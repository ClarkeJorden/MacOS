#include "Tessellator.h"

// FIXME: OSX doesn't like void (*)().
Tesselator::Tesselator() 
: _tessobj(gluNewTess())
{
    gluTessCallback(_tessobj, GLU_TESS_ERROR,
                    (GLvoid ( *) ( ))(Tesselator::error));
    gluTessCallback(_tessobj, GLU_TESS_COMBINE_DATA,
                    (GLvoid ( *) ( ))(Tesselator::combine));
    
    gluTessCallback(_tessobj, GLU_TESS_BEGIN_DATA,
                    (GLvoid ( *) ( ))(Tesselator::tessBegin));
    gluTessCallback(_tessobj, GLU_TESS_END_DATA,
                    (GLvoid ( *) ( ))(Tesselator::tessEnd));
    
    gluTessCallback(_tessobj, GLU_TESS_VERTEX_DATA,
                    (GLvoid ( *) ( ))(Tesselator::tessVertex));
    
//    gluTessProperty( _tessobj, GLU_TESS_TOLERANCE, 0.5f );
    
    // all coordinates lie in the x-y plane
    // this speeds up tesselation
    gluTessNormal(_tessobj, 0.0, 0.0, 1.0);
}

Tesselator::~Tesselator()
{
    gluDeleteTess(_tessobj);
}

void
Tesselator::beginPolygon()
{
    gluTessBeginPolygon(_tessobj, this);
}

void Tesselator::beginContour()
{
    gluTessBeginContour(_tessobj);
}

void
Tesselator::feedvert( double* vert )
{
	gluTessVertex(_tessobj, vert, vert);
}

void
Tesselator::endContour()
{
    gluTessEndContour(_tessobj);
}

void
Tesselator::tesselate(std::vector< GLfloat >& triangles)
{
    gluTessEndPolygon(_tessobj);
    
    for (size_t i = 0 ; i < _vertices.size(); i++) {
        triangles.push_back(_vertices[i]);
    }
    
    _tessVertices.clear();
    _vertices.clear();
}

void
Tesselator::addVertex(GLdouble* v)
{
    _vertices.push_back( (GLfloat)v[0] );
    _vertices.push_back( (GLfloat)v[1] );
}

// static
void
Tesselator::error(GLenum error)
{
    //                log_error(_("GLU: %s"), gluErrorString(error));
}

static GLdouble startVertex_[2];
static GLdouble lastVertex_[2];
static int vertexCount_ = 0;

//static
void Tesselator::tessBegin( GLenum type, GLvoid* user )
{
    Tesselator* me = (Tesselator*)user;
    me->setPrimType( type );
    vertexCount_ = 0;
}

//static
void Tesselator::tessEnd( GLvoid* user )
{
    
}

//static
void Tesselator::tessVertex( GLvoid* vertex, GLvoid* user )
{
    Tesselator* me = (Tesselator*)user;
    GLdouble* v = (GLdouble*)vertex;
    
    if ( me->primType() == GL_TRIANGLE_FAN ) {
        // break up fans and strips into triangles
        switch ( vertexCount_ ) {
            case 0:
                startVertex_[0] = v[0];
                startVertex_[1] = v[1];
                break;
            case 1:
                lastVertex_[0] = v[0];
                lastVertex_[1] = v[1];
                break;
                
            default:
                me->addVertex( startVertex_ );
                me->addVertex( lastVertex_ );
                me->addVertex( v );
                lastVertex_[0] = v[0];
                lastVertex_[1] = v[1];
                break;
        }
    } else if ( me->primType() == GL_TRIANGLES ) {
        me->addVertex( v );
    } else if ( me->primType() == GL_TRIANGLE_STRIP ) {
        switch ( vertexCount_ ) {
            case 0:
                me->addVertex( v );
                break;
            case 1:
                startVertex_[0] = v[0];
                startVertex_[1] = v[1];
                me->addVertex( v );
                break;
            case 2:
                lastVertex_[0] = v[0];
                lastVertex_[1] = v[1];
                me->addVertex( v );
                break;
                
            default:
                me->addVertex( startVertex_ );
                me->addVertex( lastVertex_ );
                me->addVertex( v );
                startVertex_[0] = lastVertex_[0];
                startVertex_[1] = lastVertex_[1];
                lastVertex_[0] = v[0];
                lastVertex_[1] = v[1];
                break;
        }
    }
    vertexCount_++;
}

// static
void
Tesselator::combine(GLdouble coords [3], void ** /*vertex_data[4] */,
                    GLfloat * /* weight[4] */, void **outData, void* userdata)
{
    Tesselator* me = (Tesselator*)userdata;
    v3_t v;
    v.x = coords[0];
    v.y = coords[1];
    v.z = coords[2];
    me->addTessVertex( v );
    *outData = me->tessVerticesBackPtr();
}


//Tesselator _tesselator;
//
//_tesselator.beginPolygon();
//
//for (std::list<PathPtrVec>::const_iterator iter = contours.begin(),
//	 final = contours.end(); iter != final; ++iter) 
//{      
//	const PathPtrVec& refs = *iter;
//
//	_tesselator.beginContour();
//
//	for (PathPtrVec::const_iterator it = refs.begin(), end = refs.end();
//		it != end; ++it) {
//			const Path& cur_path = *(*it);          
//
//			assert(pathpoints.find(&cur_path) != pathpoints.end());
//
//			_tesselator.feed(pathpoints[&cur_path]);                    
//	}
//
//	_tesselator.endContour();
//}
//
//_tesselator.tesselate( pMeshSet->_triangles.back()._vertices );  