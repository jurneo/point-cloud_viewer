#include "qtgraphicsview.h"

#include <QtGui>
#include <QtOpenGL\QtOpenGL>
#include <GL/glut.h>

#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>

#include <QSurfaceFormat>
#include "glmesh.h"

namespace   // anonymous
{

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif
#define FRONT_PLANE 1
#define PIXELS_PER_INCH 100.0
#define EYE_BACK 36.0

float speed = 0.1;
float speedr = 1;
double zScale = 1.0;

}

QtGraphicsView::QtGraphicsView(QWidget *screen)
    : QOpenGLWidget(screen)
{
    setFocusPolicy(Qt::StrongFocus);

    setMinimumSize(512, 512);
    std::fill (posTrans_, posTrans_ + 3, 0);
    std::fill (lastPos_, lastPos_ + 2, 0);

    distFactor_ = 0.5;

    m_background = Qt::blue;//parent->palette().background().color();

    timer.start(5, this);
    m_font.setPointSizeF(11);
    m_font.setBold(true);
}

QtGraphicsView::~QtGraphicsView()
{
    timer.stop();
    killTimer(timer.timerId());
}

void QtGraphicsView::initializeGL()
{
    QOpenGLWidget::initializeGL();

    initializeOpenGLFunctions();
    m_background.setRgbF(0.4, 0.4, 0.5);
    glClearColor(m_background.redF(),m_background.greenF(),m_background.blueF(),1);
    initShaders();

    m_mesh = new GLMesh();

    GLuint vp = mesh_shader.attributeLocation("vertex_position");
    GLuint vc = mesh_shader.attributeLocation("vertex_color");
    m_mesh->setVertexAttributes(vp, vc);

    glEnable(GL_DEPTH_TEST);

    frameTime.start();
}

void QtGraphicsView::initShaders()
{
    glEnable( GL_PROGRAM_POINT_SIZE );
    if (!mesh_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/mesh.vert"))
    {
        QString msg = tr("Unable to load the mesh vertex shader");
        //QMessageBox::warning(0, "Warning", msg);
        qDebug() << msg;
        return;
    }
    if (!mesh_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/mesh.frag"))
    {
        QString msg = tr("Unable to load the mesh fragment shader");
        //QMessageBox::warning(0, "Warning", msg);
        qDebug() << msg;
        return;
    }
    if (!mesh_shader.link())
    {
        QString msg = tr("Unable to compile the shaders");
        //QMessageBox::warning(0, "Warning", msg);
        qDebug() << msg;
        return;
    }
}

void QtGraphicsView::initTextures()
{
}

void QtGraphicsView::computeViewpoint()
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float w = viewport[2];
    float h = viewport[3];

    // Compute a correct projection matrix
    glMatrixMode(GL_PROJECTION);

    const qreal zNear = 1e-3, zFar = 1e8, fov = 45.0;

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Reset projection
    m_projection.setToIdentity();

    // Set perspective projection
    m_projection.perspective(fov, aspect, zNear, zFar);
}

void QtGraphicsView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh_shader.bind();

    computeViewpoint();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    QMatrix4x4 matrix;
    matrix.translate(posTrans_[0], posTrans_[1], posTrans_[2] - m_campos);
    matrix.rotate(m_rotation);

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    draw(matrix);

    glPopAttrib();

    mesh_shader.release();

    QPainter painter;
    painter.begin(this);
    painter.setFont(m_font);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawText(10, height() - 10, status);
    painter.end();

    frameCount++;
    int elapsed = frameTime.elapsed();
    if (elapsed >= 1000) {
        elapsed = frameTime.restart();
        double fps = double(0.1 * int(10000.0 * (qreal(frameCount) / qreal(elapsed))));
        status = numsize + "  FPS: " + QString::number(fps,'f', 1);
        frameCount = 0;
    }

}

void QtGraphicsView::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId())
    {
        if (!m_ispause)
            m_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 0.4 * speedr) * m_rotation;
        update();
    }
    else
    {
        QWidget::timerEvent(event);
    }
}

void QtGraphicsView::resizeGL(int width, int height)
{
    int side = qMax(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void QtGraphicsView::setBoundingBox(const sis::BoundingBox& box)
{
    m_box = box;

    auto sx = m_box.getSizeX();
    auto sy = m_box.getSizeY();
    auto sz = m_box.getSizeZ();

    auto ss = qMax(sx,qMax(sy,sz)) / 2.0;
    m_campos = 3 * ss;
    m_zoomspeed = ss / 5;
}

void QtGraphicsView::setDataCloud(QVector<float> source, QVector<float> color)
{
    numsize = "Size: " + QString::number(source.size()/3);
    if (m_mesh != nullptr)
        m_mesh->setPointCloud(source, color);
    update();
}

void QtGraphicsView::draw(const QMatrix4x4 &matrix)
{
    if (m_mesh->hasPoints())
    {
        // Set modelview-projection matrix
        mesh_shader.setUniformValue("mvp_matrix", m_projection * matrix);
        m_mesh->draw();
    }
}

void QtGraphicsView::wheelEvent ( QWheelEvent* event )
{
    int delta = event->delta();
    double val = (delta > 0)  ? m_zoomspeed : -m_zoomspeed;

    auto kstep = 100;
    if (val < -kstep)
    {
        val = -kstep;
    }
    else if (val > kstep)
    {
        val = kstep;
    }

    posTrans_[2] = posTrans_[2] + val;

    update();
}

void QtGraphicsView::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    default:
        m_ispause = !m_ispause;
        QWidget::keyPressEvent(event);
        break;
    }
}

void QtGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QVector2D diff = QVector2D(event->pos()) - m_mousePressPosition;

    if ((event->buttons() & Qt::LeftButton) &&
             (event->modifiers() & Qt::AltModifier))
    {
        posTrans_[0] = posTrans_[0] + (float) diff.x() * speed;
        posTrans_[1] = posTrans_[1] - (float) diff.y() * speed;
        update();

        m_mousePressPosition = QVector2D(event->pos());
    }
    else  if (event->buttons() & Qt::LeftButton)
    {
        // Update rotation
        m_rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), diff.y() * speedr) * m_rotation;
        m_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), diff.x() * speedr) * m_rotation;

        update();

        m_mousePressPosition = QVector2D(event->pos());
    }
}

void QtGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        // Save mouse press position
        m_mousePressPosition = QVector2D(event->pos());
    }
}

