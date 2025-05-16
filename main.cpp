#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <cursor.h>
#include <menu.h>
#include <minesweeper.h>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    bool dbg = false;
    if (argc > 1 && strcmp(argv[1], "-dbg") == 0)
    {
           dbg = true;
    }
    QApplication::setOverrideCursor(CustomCursor::createHandCursor());
    Menu menu(nullptr,dbg);
    menu.setWindowTitle("Minesweeper");
    menu.resize(800,600);
    menu.show();
    return a.exec();
}
