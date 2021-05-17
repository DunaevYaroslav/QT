#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>

#include "my_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow *mywindow = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar(mywindow);
    my_window *graph_area = new my_window(mywindow);
    QAction *action;


    action = tool_bar->addAction("&Next function", graph_area, SLOT(next_func()));
    action->setShortcut(QString("0"));

    action = tool_bar->addAction("&Zoom in", graph_area, SLOT(double_x()));
    action->setShortcut(QString("1"));

    action = tool_bar->addAction("&Zoom out", graph_area, SLOT(half_x()));
    action->setShortcut(QString("2"));

    action = tool_bar->addAction("&n*2", graph_area, SLOT(double_n()));
    action->setShortcut(QString("3"));

    action = tool_bar->addAction("&n/2", graph_area, SLOT(half_n()));
    action->setShortcut(QString("4"));

    action = tool_bar->addAction("&Disturb", graph_area, SLOT(double_p()));
    action->setShortcut(QString("5"));

    action = tool_bar->addAction("&Calm", graph_area, SLOT(half_p()));
    action->setShortcut(QString("6"));

    tool_bar->setMaximumHeight(30);

    mywindow->setMenuBar(tool_bar);
    mywindow->setCentralWidget(graph_area);

    mywindow->show();

    return app.exec();
}
