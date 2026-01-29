#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "videoscreen.h"
#include <QKeyEvent>
#include <QKeyCombination>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_player = new VideoScreen(this);

    setCentralWidget(m_player);

    this->setStyleSheet("background-color: black");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_O))
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Open Video", QDir::homePath());
        if (!fileName.isEmpty()) {
            m_player->playVideo(fileName);
        }
        return;
    }
    if(event->key() == Qt::Key_F || event->key() == Qt::Key_PageUp){
        if(isFullScreen()){
            showNormal();
        }else{
            showFullScreen();
        }
    }else {
        QWidget::keyPressEvent(event);
    }
}
