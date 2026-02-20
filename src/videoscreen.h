/**
 * @file videoscreen.h
 * @brief Header file for the VideoScreen class.
 *
 * Defines the video playback widget with media controls.
 */

#ifndef VIDEOSCREEN_H
#define VIDEOSCREEN_H
#include "PlayerState.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>

/**
 * @class VideoScreen
 * @brief Widget that handles video playback and media controls.
 *
 * This class encapsulates the Qt multimedia components for video playback,
 * including the media player, video output, and audio output. It handles
 * playback controls like play/pause and seeking via keyboard shortcuts.
 */
class VideoScreen : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the video screen widget.
     * @param state Pointer to shared player state for configuration.
     * @param parent Optional parent widget.
     */
    explicit VideoScreen(PlayerState *state, QWidget *parent = nullptr);

    /**
     * @brief Loads and plays a video file.
     * @param filePath Path to the video file to play.
     */
    void playVideo(const QString &filePath);

    /**
     * @brief Toggles between play and pause states.
     */
    void togglePlay();

    /**
     * @brief Seeks the video by a relative offset.
     * @param offset Time offset in milliseconds (positive = forward, negative = backward).
     */
    void seekBy(qint64 seekOffset);
    void setVolumeBy(float volumeOffset);

private:
    QMediaPlayer *m_player;          ///< Qt media player for video decoding
    QVideoWidget *m_video;           ///< Widget for rendering video frames
    QAudioOutput *m_audio;           ///< Audio output handler
    PlayerState *m_state;            ///< Shared player configuration state
    int m_currentSubtitleTrack = -1; ///< Current subtitle track index (-1 = off)

protected:
    /**
     * @brief Handles keyboard input for playback controls.
     * @param event The key event to process.
     *
     * Supported shortcuts:
     * - Space / K: Toggle play/pause
     * - Right Arrow: Seek forward
     * - Left Arrow: Seek backward
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Cycles through available subtitle tracks.
     */
    void cycleSubtitleTrack(bool shouldCycle);

signals:
};

#endif // VIDEOSCREEN_H
