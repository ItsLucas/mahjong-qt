#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include<QPixmap>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include<QSound>

namespace Ui {
class startWindow;
}

class startWindow : public QMainWindow
{
    Q_OBJECT


public:
    QMediaPlayer* player;
    explicit startWindow(QWidget *parent = nullptr);


    ~startWindow();

private slots:
    void on_gameMode_clicked();

    void on_startGame_clicked();

    void on_setting_clicked();

    void on_volumecontrol_clicked();

private:
    Ui::startWindow *ui;

};

#endif // STARTWINDOW_H
