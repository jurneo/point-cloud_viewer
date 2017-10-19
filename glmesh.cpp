#include "glmesh.h"
#include "mesh.h"
#include <boost/cstdint.hpp>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

// Credit to: mkeeter/fstl @ 2015

using namespace boost;

namespace {
bool g_isUpdate = false;
Mesh g_mesh;

}

GLMesh::GLMesh() : m_hasPoints(false)
{
    initializeOpenGLFunctions();
    vertices = nullptr;
    indices = nullptr;
}

void GLMesh::setPointCloud(const QVector<float>& pcloud, const QVector<float>& ccloud)
{
    if (pcloud.isEmpty())  return;

    g_isUpdate = true;

    vertices = const_cast<float*>(pcloud.data());
    indices = const_cast<float*>(ccloud.data());
    numsize = pcloud.size() / 3; // x y z

    qDebug() << numsize << ", " << vp << ", " << vc;

    m_hasPoints = true;
    g_isUpdate = false;

}

void GLMesh::draw()
{
    glEnableVertexAttribArray(vp);
    glEnableVertexAttribArray(vc);

    glVertexAttribPointer(vp, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(vc, 3, GL_FLOAT, GL_FALSE, 0, indices);
    glDrawArrays(GL_POINTS, 0, numsize);

    glDisableVertexAttribArray(vc);
    glDisableVertexAttribArray(vp);
}
