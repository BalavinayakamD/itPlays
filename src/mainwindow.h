/**
 * @file mainwindow.h
 * @brief Header file for the MainWindow class.
 *
 * Defines the main application window that hosts the video player.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videoscreen.h"
#include "PlayerState.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main application window for the video player.
 *
 * This class manages the main window, including fullscreen toggling,
 * file opening dialogs, and keyboard shortcuts for application-level controls.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the MainWindow.
     * @param parent Optional parent widget (default: nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor - cleans up UI resources.
     */
    ~MainWindow();

    /**
     * @brief Handles keyboard input for window-level controls.
     * @param event The key event to process.
     *
     * Supported shortcuts:
     * - Ctrl+O: Open file dialog to select a video
     * - F / PageUp: Toggle fullscreen mode
     */
    void keyPressEvent(QKeyEvent *event) override;

    /** @brief Shared player state for configuration settings. */
    PlayerState state;

private slots:
    void showToast(const QString &message);

private:
    VideoScreen *m_player; ///< The video screen widget for playback
    Ui::MainWindow *ui;    ///< Pointer to the UI form
};

#endif // MAINWINDOW_H
