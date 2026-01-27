#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "videoscreen.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    VideoScreen *m_player = new VideoScreen(this);

    setCentralWidget(m_player);

    this->setStyleSheet("background-color: grey");

    // m_player->playVideo("Provide path here");

}

MainWindow::~MainWindow()
{
    delete ui;
}
