#include "minesweeper.h"

#include <gtest/gtest.h>

#include <QApplication>
#include <QFile>

namespace
{
    struct QAppHolder
    {
        QApplication* app;
        QAppHolder()
        {
            int argc = 0;
            char* argv[] = { nullptr };
            app = new QApplication(argc, argv);
        }
        ~QAppHolder() { delete app; }
    } qapp;
}
class MinesweeperTest : public ::testing::Test, public Minesweeper
{
  protected:
    void SetUp() override { minesweeper = new Minesweeper(10, 10, 15); }

    void TearDown() override { delete minesweeper; }

    Minesweeper* minesweeper;
};

TEST_F(MinesweeperTest, ConstructorTest)
{
    EXPECT_EQ(minesweeper->rows(), 10);
    EXPECT_EQ(minesweeper->cols(), 10);
    EXPECT_EQ(minesweeper->mines(), 15);
    EXPECT_FALSE(minesweeper->isGameOver());
    EXPECT_TRUE(minesweeper->isFirstClick());
}

TEST_F(MinesweeperTest, NewGame)
{
    minesweeper->newGame(8, 8, 10);
    EXPECT_EQ(minesweeper->rows(), 8);
    EXPECT_EQ(minesweeper->cols(), 8);
    EXPECT_EQ(minesweeper->mines(), 10);
}

TEST_F(MinesweeperTest, BigNewMapewGame)
{
    minesweeper->newGame(100, 100, 4444);
    EXPECT_EQ(minesweeper->rows(), 100);
    EXPECT_EQ(minesweeper->cols(), 100);
    EXPECT_EQ(minesweeper->mines(), 4444);
}

TEST_F(MinesweeperTest, CellValidationTest)
{
    EXPECT_TRUE(minesweeper->isValidCell(5, 7));
    EXPECT_TRUE(minesweeper->isValidCell(9, 9));
    EXPECT_TRUE(minesweeper->isValidCell(0, 0));
}
TEST_F(MinesweeperTest, NegCellValidationTest)
{
    EXPECT_FALSE(minesweeper->isValidCell(-1, -333));
    EXPECT_FALSE(minesweeper->isValidCell(-1, 0));
    EXPECT_FALSE(minesweeper->isValidCell(-7, -5));
}
TEST_F(MinesweeperTest, UncorCellValidationTest)
{
    EXPECT_FALSE(minesweeper->isValidCell(323, 232));
    EXPECT_FALSE(minesweeper->isValidCell(15, 23));
}
TEST_F(MinesweeperTest, GameOverWin)
{
    minesweeper->newGame(7, 8, 15);

    for (int row = 0; row < 7; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            if (!minesweeper->cellHasMine(row, col))
            {
                minesweeper->revealCell(row, col);
            }
        }
    }

    EXPECT_FALSE(minesweeper->isGameOver());
}
TEST_F(MinesweeperTest, GameOverLose)
{
    minesweeper->newGame(10, 15, 15);

    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 15; ++col)
        {
            if (minesweeper->cellHasMine(row, col))
            {
                minesweeper->revealCell(row, col);
            }
        }
    }

    EXPECT_FALSE(minesweeper->isGameOver());
}
TEST_F(MinesweeperTest, GameOverZero)
{
    minesweeper->newGame(10, 15, 0);

    minesweeper->revealCell(0, 0);

    EXPECT_TRUE(minesweeper->isGameOver());
}

TEST_F(MinesweeperTest, InvalidSaveLoad)
{
    Minesweeper loadedGame(10, 10, 15);
    EXPECT_FALSE(loadedGame.loadGame("nonexistent_file.ini"));

    QFile invalidFile("invalid_file.ini");
    if (invalidFile.open(QIODevice::WriteOnly))
    {
        invalidFile.write("This is not a valid save file");
        invalidFile.close();
    }
    EXPECT_FALSE(loadedGame.loadGame("invalid_file.ini"));
    QFile::remove("invalid_file.ini");
}

TEST_F(MinesweeperTest, CellReveal)
{
    minesweeper->newGame(3, 3, 0);
    minesweeper->revealCell(0, 0);
    EXPECT_TRUE(minesweeper->isGameOver());
}
TEST_F(MinesweeperTest, FlagToggle)
{
    minesweeper->toggleCellMark(0, 0);
    minesweeper->toggleCellMark(0, 0);
}

TEST_F(MinesweeperTest, RevealAlreadyOpenedCell)
{
    newGame(5, 5, 5);
    revealCell(2, 2);
    int prevRevealed = revealedCells();
    revealCell(2, 2);
    EXPECT_EQ(revealedCells(), prevRevealed);
}

TEST_F(MinesweeperTest, FlagOpenedCell)
{
    newGame(5, 5, 5);
    revealCell(2, 2);
    CellState prevState = m_cells[2][2].state;
    toggleCellMark(2, 2);
    EXPECT_EQ(m_cells[2][2].state, prevState);
}

TEST_F(MinesweeperTest, WinWithAllFlagsCorrect)
{
    newGame(3, 3, 3);
    m_firstClick = false;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (m_cells[row][col].hasMine)
            {
            }
            else
            {
                revealCell(row, col);
            }
        }
    }

    EXPECT_FALSE(isGameOver());
}

TEST_F(MinesweeperTest, MaxFieldSize)
{
    const int maxSize = 50;
    newGame(maxSize, maxSize, maxSize * maxSize / 4);
    EXPECT_EQ(rows(), maxSize);
    EXPECT_EQ(cols(), maxSize);

    revealCell(maxSize - 1, maxSize - 1);
    EXPECT_GT(revealedCells(), 0);
}

TEST_F(MinesweeperTest, TooManyMines)
{
    Minesweeper game(5, 5, 26);
    revealCell(0, 0);
    EXPECT_TRUE(isFirstClick());
}

TEST_F(MinesweeperTest, RevealFlaggedCell)
{
    newGame(5, 5, 5);
    toggleCellMark(2, 2);
    revealCell(2, 2);
    EXPECT_EQ(m_cells[2][2].state, Flagged);
}

TEST_F(MinesweeperTest, CellAppearanceUpdate)
{
    newGame(3, 3, 1);
    m_firstClick = false;

    EXPECT_EQ(m_cells[1][1].button->text(), "");

    revealCell(1, 1);

    if (m_cells[1][1].adjacentMines > 0)
    {
        EXPECT_FALSE(m_cells[1][1].button->text().isEmpty());
    }
    else
    {
        EXPECT_EQ(m_cells[1][1].button->text(), "");
    }
}
TEST_F(MinesweeperTest, RevealAllNonMineCells)
{
    newGame(4, 4, 2);
    m_firstClick = false;

    m_cells[0][0].hasMine = true;
    m_cells[3][3].hasMine = true;

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            if (!(row == 0 && col == 0) && !(row == 3 && col == 3))
            {
                revealCell(row, col);
            }
        }
    }

    EXPECT_EQ(revealedCells(), 14);
}

TEST_F(MinesweeperTest, ClickOutsideGrid)
{
    newGame(5, 5, 5);
    EXPECT_FALSE(isValidCell(-1, -1));
    EXPECT_FALSE(isValidCell(5, 5));

    revealCell(-1, -1);
    toggleCellMark(5, 5);
}
TEST_F(MinesweeperTest, AdjacentFlagsCount)
{
    newGame(3, 3, 1);
    m_firstClick = false;

    toggleCellMark(0, 0);
    toggleCellMark(0, 1);

    EXPECT_EQ(countAdjacentFlags(1, 1), 2);
    EXPECT_EQ(countAdjacentFlags(2, 2), 0);
}
