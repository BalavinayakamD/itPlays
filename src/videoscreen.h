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

    void togglePlay();

private:
    QMediaPlayer *m_player;
    QVideoWidget *m_video;
    QAudioOutput *m_audio;

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
};

#endif // VIDEOSCREEN_H
