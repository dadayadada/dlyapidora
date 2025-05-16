#include "playoption.h"
#include "minesweeper.h"
#include "menu.h"
#include "inputbtn.h"
PlayOption::PlayOption(QWidget *parent,bool dbg) : QWidget(parent)
{
    stackedWidget = new QStackedWidget(this);
    playOptionPage = new QWidget();
    QHBoxLayout *playLayout = new QHBoxLayout(playOptionPage);
    QPushButton *playBtn = new QPushButton("Update map",playOptionPage);




    QVBoxLayout *bottomGroup = new QVBoxLayout;
    rows = new NumberEditButton("Rows:", 10, this);
    columns = new NumberEditButton("Columns:", 10, this);
    mines = new NumberEditButton("Mines:", 15, this);
    bottomGroup->addWidget(rows);
    bottomGroup->addWidget(columns);
    bottomGroup->addWidget(mines);
    playLayout->addLayout(bottomGroup);

    playLayout->addWidget(playBtn);






    stackedWidget->addWidget(playOptionPage);


    game = new Minesweeper(10, 10, 15, this,dbg);
    game->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidget *gameContainer = new QWidget(playOptionPage);
    QVBoxLayout *gameLayout = new QVBoxLayout(gameContainer);
    gameLayout->addWidget(game);

    playLayout->addWidget(game);



    QVBoxLayout *playmainLayout = new QVBoxLayout(this);
    playmainLayout->addWidget(stackedWidget);

    connect(playBtn, &QPushButton::clicked, this, &PlayOption::startPlaying);


    saveBar = new QToolButton(playOptionPage);
    saveBar->setText("Files");
    saveBar->setPopupMode(QToolButton::InstantPopup);
    toolMenu = new QMenu(saveBar);
    quickLoad = toolMenu->addAction("Quick Load",game,&Minesweeper::onQuickLoad);
    quickSave = toolMenu->addAction("Quick Save",game,&Minesweeper::onQuickSave);
    Load = toolMenu->addAction("Load Game",game,&Minesweeper::onLoadClicked);
    Save = toolMenu->addAction("Save Game",game,&Minesweeper::onSaveClicked);
    saveBar->setMenu(toolMenu);




}
void PlayOption::startPlaying()
{
    int row = rows->value();
    int cols = columns->value();
    int min = mines->value();
    if (min   >=   0.9 * row * cols)
    {
            QMessageBox::warning(this, "Error", "nekaya oshibka(");
            return;

    }


   game->newGame(row, cols, min);


}




