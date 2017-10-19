#include "testwindow.h"
#include "ui_testwindow.h"

//#include "graphicsview.h"
#include "qtgraphicsview.h"

#include <QStringListModel>
#include <QFileDialog>
#include <QCoreApplication>
#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>
#include <QElapsedTimer>


#include <iostream>
//#include <core/boundingbox.h>
#include "utility.h"

namespace {
QFutureWatcher<void> g_f2;
QElapsedTimer timer;
}

TestWindow::TestWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);

    m_window = new QtGraphicsView(this);
    ui->mvl->addWidget(m_window, 1);

    QRect r = rect();
    r.setTop(200);
    r.setLeft(400);
    setGeometry(r);

//    QTimer::singleShot(500, this, [this](){
//    QString fileName("c:/users/k/desktop/test.ply");
//    qDebug() << "loading..: " << fileName;
//    loadPly(fileName);
//    });
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::on_loadDataBtn_clicked()
{
    if (g_f2.isRunning())  return;

    // get the file names
    QString path = QCoreApplication::applicationDirPath();
    QDir dp(path);
    dp.cdUp();

    //path = dp.absolutePath();
    path = "c:/users/k/desktop";

    QString fileName = QFileDialog::getOpenFileName(0, tr("Cloud-Data"), path, tr("*.* ;; Text Files (*.txt)"));
    if (fileName.isEmpty())  return;

    if (fileName.endsWith(".png"))
    {
        if (fileName.endsWith("ir.png"))
            fileName.replace("ir.png", ".ply");
        else
            fileName.replace(".png", ".ply");
    }

    loadPly(fileName);
}

void TestWindow::toggelUi(bool flag)
{
    ui->loadDataBtn->setEnabled(flag);
}

void TestWindow::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_Escape)
    {
        close();
    }
}

void TestWindow::loadPly(QString fileName)
{
    g_f2.disconnect();
    std::cout << "data: " << fileName.toStdString() << "\n";
    g_f2.setFuture( QtConcurrent::run([=,this]()
    {
        timer.start();
        float rr[6];
        float cc[3];
        sis::readPly(fileName.toStdString(), m_points, m_colors, rr, cc);
        auto sz = m_points.size();
        auto t1 = timer.elapsed();
        qDebug() << "loading time: " << t1 << " ms.";
        sis::BoundingBox box(rr[0], rr[1], rr[2], rr[3], rr[4], rr[5]);
        m_window->setBoundingBox(box);
        m_window->setDataCloud(m_points, m_colors);
    }));
}
