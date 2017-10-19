#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>

#include <core/vector.h>
#include <core/orientation.h>
#include <model/rawpointdata.h>

namespace Ui {
class TestWindow;
}

class QtGraphicsView;

class TestWindow : public QWidget
{
    Q_OBJECT    
public:
    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();

    void toggelUi(bool flag);
    void loadPly(QString filename);

signals:
    void sgnDone(int param);

protected:
    void keyPressEvent(QKeyEvent * e);

private slots:
    void on_loadDataBtn_clicked();

private:
    Ui::TestWindow *ui;

    QtGraphicsView* m_window;
    QVector<float> m_points;
    QVector<float> m_colors;

    sis::Vector m_translate;
    sis::Orientation m_rotate;
};

#endif // TESTWINDOW_H
