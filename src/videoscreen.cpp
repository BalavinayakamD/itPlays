#include "videoscreen.h"

#include <QVBoxLayout>

VideoScreen::VideoScreen(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    //Videos to edge to edge for minimalist design
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    //Create and add the player to the UI
    m_video = new QVideoWidget(this);
    layout->addWidget(m_video);

    //Create the player and audio logic
    m_player = new QMediaPlayer(this);
    m_audio = new QAudioOutput(this);

    //use this to output video and audio
    m_player->setVideoOutput(m_video);
    m_player->setAudioOutput(m_audio);
}

void VideoScreen::playVideo(const QString &filePath)
{
    m_player->setSource(QUrl::fromLocalFile(filePath));
    m_player->play();
}
