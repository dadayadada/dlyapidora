#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include <QMenu>
class Menu : public QWidget {
    Q_OBJECT
public:
    Menu(QWidget *parent = nullptr,bool dbg = false);







public slots:
    void switchToGame();
    void BackToMenu();
private:
    QStackedWidget *stackedWidget;
    QWidget *gamePage;
    void formatBtn(QPushButton*);
    QWidget *menuPage;
    QAction *playAction;
    QAction *quitAction;
    QAction *Back_To_Menu;
    QMenu *toolMenu;
    QToolButton *toolBar;



};
