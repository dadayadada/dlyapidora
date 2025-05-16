#ifndef PLAYOPTION_H
#define PLAYOPTION_H
#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <inputbtn.h>
#include <minesweeper.h>

class PlayOption : public QWidget
{
   Q_OBJECT
public:
    PlayOption(QWidget *parent = nullptr,bool dbg = false);



private slots:
    void startPlaying();
private:
    QStackedWidget *stackedWidget;
    QWidget *playOptionPage;
    QWidget *play;
    NumberEditButton *rows;
    NumberEditButton *columns;
    NumberEditButton *mines;
    Minesweeper *game;
    QAction *quickSave;
    QAction *quickLoad;
    QAction *Load;
    QAction *Save;
    QMenu *toolMenu;
    QToolButton *saveBar;




};

#endif

