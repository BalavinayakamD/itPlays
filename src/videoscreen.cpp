/**
 * @file videoscreen.cpp
 * @brief Implementation of the VideoScreen class.
 */

#include "videoscreen.h"

#include <QVBoxLayout>
#include <QKeyEvent>
#include <QMediaMetaData>
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

    // Log when tracks become available (subtitles load asynchronously)
    connect(m_player, &QMediaPlayer::tracksChanged, this, [this]()
            {
        auto subs = m_player->subtitleTracks();
        qDebug() << "Tracks changed — subtitle tracks available:" << subs.size();
        for (int i = 0; i < subs.size(); ++i)
            qDebug() << "  [" << i << "]" << subs[i].stringValue(QMediaMetaData::Language)
                     << subs[i].stringValue(QMediaMetaData::Title);

        if (subs.isEmpty())
        {
            m_currentSubtitleTrack = -1;
            m_player->setActiveSubtitleTrack(-1);
            return;
        }

        if (m_state->isSubtitlesOn)
        {
            if (m_currentSubtitleTrack < 0 || m_currentSubtitleTrack >= subs.size())
                m_currentSubtitleTrack = m_lastSubtitleTrack;
            if (m_currentSubtitleTrack < 0 || m_currentSubtitleTrack >= subs.size())
                m_currentSubtitleTrack = 0;
            m_lastSubtitleTrack = m_currentSubtitleTrack;
            m_player->setActiveSubtitleTrack(m_currentSubtitleTrack);
        }
        else
        {
            m_currentSubtitleTrack = -1;
            m_player->setActiveSubtitleTrack(-1);
        }
    });
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
void VideoScreen::seekBy(qint64 seekOffset)
{
    // Calculate new position and apply it
    qint64 newPos = m_player->position() + seekOffset;
    m_player->setPosition(newPos);
}

void VideoScreen::setVolumeBy(float volumeOffset)
{
    float currentVolume = m_audio->volume();
    m_audio->setVolume(currentVolume + volumeOffset);
}

void VideoScreen::toggleSubtitles(bool isSubtitlesOn)
{
    m_state->isSubtitlesOn = isSubtitlesOn;

    const auto tracks = m_player->subtitleTracks();
    if (tracks.isEmpty())
    {
        m_currentSubtitleTrack = -1;
        if (isSubtitlesOn)
            emit subtitleToastRequested("No Subtitles to Load");
        return;
    }

    if (isSubtitlesOn)
    {
        if (m_lastSubtitleTrack < 0 || m_lastSubtitleTrack >= tracks.size())
            m_lastSubtitleTrack = 0;

        m_currentSubtitleTrack = m_lastSubtitleTrack;
        m_player->setActiveSubtitleTrack(m_currentSubtitleTrack);
    }
    else
    {
        if (m_currentSubtitleTrack >= 0 && m_currentSubtitleTrack < tracks.size())
            m_lastSubtitleTrack = m_currentSubtitleTrack;
        m_currentSubtitleTrack = -1;
        m_player->setActiveSubtitleTrack(-1);
    }
}

void VideoScreen::cycleSubtitleTrack(bool shouldCycle)
{
    const auto tracks = m_player->subtitleTracks();
    const int trackCount = tracks.size();

    if (trackCount == 0)
    {
        qDebug() << "No subtitle tracks available (media may still be loading)";
        emit subtitleToastRequested("No Subtitles to Load");
        return;
    }

    if (shouldCycle)
    {
        if (m_currentSubtitleTrack == -1)
            m_currentSubtitleTrack = m_lastSubtitleTrack;

        // Cycle: -1 → 0 → 1 → ... → (n-1) → -1
        m_currentSubtitleTrack++;
        if (m_currentSubtitleTrack >= trackCount)
            m_currentSubtitleTrack = 0;
        m_lastSubtitleTrack = m_currentSubtitleTrack;
    }

    qDebug() << "Subtitle track set to:" << m_currentSubtitleTrack
             << "of" << trackCount << "available";

    if (m_currentSubtitleTrack == -1)
    {
        // Qt 6: set to trackCount (out-of-range) to disable, or use -1
        m_player->setActiveSubtitleTrack(-1);
    }
    else
    {
        m_player->setActiveSubtitleTrack(m_currentSubtitleTrack);
    }
}
/**
 * @brief Handles keyboard shortcuts for playback control.
 * @param event The key press event.
 */
void VideoScreen::keyPressEvent(QKeyEvent *event)
{
    // BASIC CONTROLS

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
    else if (event->key() == Qt::Key_Up)
    {
        setVolumeBy(m_state->volumeOffset);
    }
    else if (event->key() == Qt::Key_Down)
    {
        setVolumeBy(-m_state->volumeOffset);
    }

    // FEATURES

    // Shift + S : This cycles subtitles if the subtitles are on , else this shows a toast
    else if (event->keyCombination() == QKeyCombination(Qt::ShiftModifier, Qt::Key_S))
    {
        cycleSubtitleTrack(true);
    }
    // S : This toggles subtitles for the video
    else if (event->key() == Qt::Key_S)
    {
        toggleSubtitles(!m_state->isSubtitlesOn);
    }
    // Pass unhandled events to the base class
    else
    {
        QWidget::keyPressEvent(event);
    }
}
