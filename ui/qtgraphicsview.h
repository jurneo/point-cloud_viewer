#ifndef QTGRAPHICSVIEW_H
#define QTGRAPHICSVIEW_H

#include <QWindow>
#include <QOpenGLWidget>
#include <QElapsedTimer>
#include <QFont>
#include <QBasicTimer>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QOpenGLShaderProgram>
#include <core/boundingbox.h>
#include <QOpenGLFunctions>

class GLMesh;

class QtGraphicsView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    QtGraphicsView(QWidget* screen=0);
    ~QtGraphicsView();

    // model cloud is static target
    void setDataCloud(QVector<float> target, QVector<float> color);
    void setDataColor(QVector<QVector3D> color);
    void setBoundingBox(const sis::BoundingBox& box);

protected:
    void draw(const QMatrix4x4& matrix);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent ( QWheelEvent * event );
    void keyPressEvent(QKeyEvent *event);

    void initShaders();
    void initTextures();
    void computeViewpoint();

    void timerEvent(QTimerEvent *event);

protected:
    float lastPos_[2];
    float posTrans_[3];
    float distFactor_;

    QColor m_background;
    QMatrix4x4 m_projection;
    QVector2D m_mousePressPosition;
    QQuaternion m_rotation;

    QOpenGLShaderProgram mesh_shader;
    QOpenGLShaderProgram quad_shader;
    GLMesh* m_mesh = nullptr;
    GLuint texture;

    QElapsedTimer frameTime;
    long unsigned int frameCount = 0;
    QString status;
    QBasicTimer timer;

    sis::BoundingBox m_box;
    double m_campos = 0;
    double m_zoomspeed = 0;
    QString numsize;
    QFont m_font;
    bool m_ispause = false;

    QVector<QVector3D> m_colors;
    QVector<QVector3D> m_data;
};

#endif // QTGRAPHICSVIEW_H
