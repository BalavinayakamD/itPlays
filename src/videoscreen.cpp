/**
 * @file videoscreen.cpp
 * @brief Implementation of the VideoScreen class.
 */

#include "videoscreen.h"

#include <QVBoxLayout>
#include <QKeyEvent>

/**
 * @brief Constructs the video screen and initializes multimedia components.
 * @param state Pointer to shared player state.
 * @param parent Optional parent widget.
 */
VideoScreen::VideoScreen(PlayerState *state, QWidget *parent)
    : QWidget{parent}, m_state(state)
{
    // Enable keyboard focus so this widget can receive key events
    setFocusPolicy(Qt::StrongFocus);

    // Create layout for the video widget
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Edge-to-edge video display for minimalist design
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create and add the video widget to the layout
    m_video = new QVideoWidget(this);
    layout->addWidget(m_video);

    // Initialize the media player and audio output
    m_player = new QMediaPlayer(this);
    m_audio = new QAudioOutput(this);

    // Connect player to video and audio outputs
    m_player->setVideoOutput(m_video);
    m_player->setAudioOutput(m_audio);
}

/**
 * @brief Loads and starts playing a video file.
 * @param filePath Absolute path to the video file.
 */
void VideoScreen::playVideo(const QString &filePath)
{
    // Set the video source from local file path
    m_player->setSource(QUrl::fromLocalFile(filePath));
    m_player->play();
}

/**
 * @brief Toggles between play and pause states.
 */
void VideoScreen::togglePlay()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState)
    {
        m_player->pause();
    }
    else
    {
        m_player->play();
    }
}

/**
 * @brief Seeks the playback position by a relative offset.
 * @param offset Time offset in milliseconds.
 */
void VideoScreen::seekBy(qint64 offset)
{
    // Calculate new position and apply it
    int newPos = m_player->position() + offset;
    m_player->setPosition(newPos);
}

/**
 * @brief Handles keyboard shortcuts for playback control.
 * @param event The key press event.
 */
void VideoScreen::keyPressEvent(QKeyEvent *event)
{
    // Space or K: Toggle play/pause (K matches YouTube shortcut)
    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_K)
    {
        togglePlay();
    }
    // Right arrow: Seek forward by configured offset
    else if (event->key() == Qt::Key_Right)
    {
        seekBy(m_state->seekOffset);
    }
    // Left arrow: Seek backward by configured offset
    else if (event->key() == Qt::Key_Left)
    {
        seekBy(-m_state->seekOffset);
    }
    // Pass unhandled events to the base class
    else
    {
        QWidget::keyPressEvent(event);
    }
}
