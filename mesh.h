#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QtOpenGL/QtOpenGL>

#include <vector>

class Mesh
{
public:
    Mesh() {}
    virtual ~Mesh() {}
    Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);

    float min(size_t start) const;
    float max(size_t start) const;

    float xmin() const { return min(0); }
    float ymin() const { return min(1); }
    float zmin() const { return min(2); }
    float xmax() const { return max(0); }
    float ymax() const { return max(1); }
    float zmax() const { return max(2); }

private:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    friend class GLMesh;
};

#endif // MESH_H
