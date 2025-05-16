#ifndef MINESWEEPERWIDGET_H
#define MINESWEEPERWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
class Minesweeper : public QWidget
{
    Q_OBJECT


public:
    int rows() const { return m_rows; }
    int cols() const { return m_cols; }
    int mines() const { return m_mines; }
    bool isGameOver() const { return m_gameOver; }
    bool isFirstClick() const { return m_firstClick; }
    int revealedCells() const { return m_revealedCells; }
    explicit Minesweeper(int rows = 10, int cols = 10, int mines = 15, QWidget *parent = nullptr,bool dbgMode = false);
    void newGame(int rows, int cols, int mines);
    void saveGame(const QString& filename);
    bool loadGame(const QString& filename);
    void onSaveClicked();
    void onLoadClicked();
    void onQuickSave();
    void onQuickLoad();
    void safeSave();
    bool isValidCell(int row, int col) const;
    bool cellHasMine(int row, int col) const { return m_cells[row][col].hasMine; }
    void revealCell(int row, int col);
    void toggleCellMark(int row, int col);




public slots:
    void handleCellClick();
    void handleCellRightClick();


protected:
    enum CellState {
        Closed,
        Opened,
        Flagged,
    };
    struct CellData {
        bool hasMine;
        CellState state;
        int adjacentMines;
    };

    struct Cell {
        QPushButton* button;
        bool hasMine;
        CellState state;
        int adjacentMines;
    };

    void initializeGame();
    void placeMines(int firstClickRow, int firstClickCol);
    void revealEmptyArea(int row, int col);
    void updateCellAppearance(int row, int col);
    void gameOver(bool won);
    int countAdjacentFlags(int row, int col) const;
    int m_rows;
    int m_cols;
    int m_mines;
    int m_revealedCells;
    bool m_gameOver;
    bool m_firstClick;
    bool m_lose;
    bool m_debugMode;
    QVector<QVector<Cell>> m_cells;
    QGridLayout* m_gridLayout;
};

#endif
