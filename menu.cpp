#include "menu.h"
#include "playoption.h"
#include "minesweeper.h"
Menu::Menu(QWidget *parent,bool dbg) : QWidget(parent) {


    stackedWidget = new QStackedWidget(this);

    menuPage = new QWidget();
    gamePage = new  PlayOption(this,dbg);


    QVBoxLayout *menuLayout = new QVBoxLayout(menuPage);
    QPushButton *playBtn = new QPushButton("Play", menuPage);
    QPushButton *exitBtn = new QPushButton("Quit Game", menuPage);





    formatBtn(playBtn);
    formatBtn(exitBtn);
    menuLayout->addWidget(playBtn);
    menuLayout->addWidget(exitBtn);
    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(gamePage);
    toolBar = new QToolButton(this);
    toolBar->setText("Toolbar");
    toolBar->setPopupMode(QToolButton::InstantPopup);
    toolMenu = new QMenu(toolBar);
    playAction = toolMenu->addAction("Start playing",this,&Menu::switchToGame);
    Back_To_Menu = toolMenu->addAction("Back To Menu",this,&Menu::BackToMenu);
    quitAction = toolMenu->addAction("Quit Game",this,&QApplication::quit);

    Back_To_Menu->setVisible(0);
    toolBar->setMenu(toolMenu);
    toolBar->setStyleSheet(
        "QToolButton {"
        "   background: transparent;"
        "   border: none;"
        "}"
        "QToolButton::menu-indicator { image: none; }"
    );


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);


    connect(playBtn, &QPushButton::clicked, this, &Menu::switchToGame);
    connect(exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);


}


void Menu::BackToMenu()
{

    stackedWidget->setCurrentWidget(menuPage);
    Back_To_Menu->setVisible(0);
    playAction->setVisible(1);

}

void Menu::switchToGame()
{
   stackedWidget->setCurrentWidget(gamePage);
   Back_To_Menu->setVisible(1);
   playAction->setVisible(0);

}


void Menu::formatBtn(QPushButton *btn)
{
    btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

}

