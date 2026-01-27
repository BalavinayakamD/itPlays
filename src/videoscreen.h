#ifndef VIDEOSCREEN_H
#define VIDEOSCREEN_H

#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>

class VideoScreen : public QWidget
{
    Q_OBJECT

public:
    explicit VideoScreen(QWidget *parent = nullptr);

    void playVideo(const QString &filePath);

private:
    QMediaPlayer *m_player;
    QVideoWidget *m_video;
    QAudioOutput *m_audio;


signals:
};

#endif // VIDEOSCREEN_H
