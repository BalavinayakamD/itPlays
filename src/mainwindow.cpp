/**
 * @file mainwindow.cpp
 * @brief Implementation of the MainWindow class.
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "videoscreen.h"
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QKeyCombination>
#include <QFileDialog>

/**
 * @brief Constructs the main window and initializes the video player.
 * @param parent Optional parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the video screen widget with shared player state
    m_player = new VideoScreen(&state, this);

    connect(m_player, &VideoScreen::subtitleToastRequested, this, &MainWindow::showToast);

    // Set video screen as the central widget (fills the window)
    setCentralWidget(m_player);
    m_player->setFocus();

    // Set black background for a cinema-like experience
    this->setStyleSheet("background-color: black");
}

/**
 * @brief Destructor - cleans up the UI.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Handles keyboard shortcuts for window-level controls.
 * @param event The key press event to handle.
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Ctrl+O: Open file dialog to select and play a video
    if (event->keyCombination() == QKeyCombination(Qt::ControlModifier, Qt::Key_O))
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Open Video", QDir::homePath());
        if (!fileName.isEmpty())
        {
            m_player->playVideo(fileName);
        }
        return;
    }

    // F or PageUp: Toggle fullscreen mode
    if (event->key() == Qt::Key_F || event->key() == Qt::Key_PageUp)
    {
        if (isFullScreen())
        {
            showNormal(); // Exit fullscreen
        }
        else
        {
            showFullScreen(); // Enter fullscreen
        }
    }
    else
    {
        // Pass unhandled events to the base class
        QWidget::keyPressEvent(event);
    }
}

void MainWindow::showToast(const QString &message)
{
    QLabel *toast = new QLabel(this);
    toast->setText(message);
    toast->setAttribute(Qt::WA_DeleteOnClose);
    toast->setAttribute(Qt::WA_TransparentForMouseEvents);
    toast->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(30, 30, 30, 230);"
        "   color: white;"
        "   padding: 10px 20px;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "}"
    );

    toast->setAlignment(Qt::AlignCenter);
    toast->adjustSize();
    toast->setGeometry((width() - toast->width()) / 2,
                       height() - toast->height() - 30,
                       toast->width(),
                       toast->height());
    toast->raise();
    toast->show();

    QTimer::singleShot(2000, toast, &QLabel::deleteLater);
}
