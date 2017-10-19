#ifndef GLMESH_H
#define GLMESH_H

// Credit to: mkeeter/fstl @ 2015

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <vector>

class Mesh;

class GLMesh : protected QOpenGLFunctions
{
public:
    GLMesh();
    void setVertexAttributes(int v1, int v2)
    {
        vp = v1;
        vc = v2;
    }

    void setPointCloud(const QVector<float> &points, const QVector<float> &cloudColors);
    void draw();
    bool hasPoints() const
    {
        return m_hasPoints;
    }

private:
    bool m_hasPoints;
    GLfloat* vertices;
    GLfloat* indices;
    GLuint numsize = 0;
    GLuint vp = 0 , vc = 0;
};

#endif // GLMESH_H
