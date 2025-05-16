#include "minesweeper.h"

#include <QFileDialog>
#include <QIcon>
#include <QMouseEvent>
#include <QPainter>
#include <QRandomGenerator>

Minesweeper::Minesweeper(int rows, int cols, int mines, QWidget* parent, bool dbgMode) :
    QWidget(parent), m_rows(rows), m_cols(cols), m_mines(mines), m_debugMode(dbgMode)
{
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_gridLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    initializeGame();
}

void Minesweeper::newGame(int rows, int cols, int mines)
{
    m_rows = rows;
    m_cols = cols;
    m_mines = mines;

    QLayoutItem* item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    initializeGame();
}

void Minesweeper::initializeGame()
{
    m_lose = true;
    m_cells.resize(m_rows);
    m_revealedCells = 0;
    m_gameOver = false;
    m_firstClick = true;

    for (int row = 0; row < m_rows; ++row)
    {
        m_cells[row].resize(m_cols);
        for (int col = 0; col < m_cols; ++col)
        {
            Cell& cell = m_cells[row][col];
            cell.button = new QPushButton(this);
            cell.button->setMinimumSize(0, 0);
            cell.button->setProperty("row", row);
            cell.button->setProperty("col", col);
            cell.button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            connect(cell.button, &QPushButton::clicked, this, &Minesweeper::handleCellClick);

            cell.button->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(cell.button, &QPushButton::customContextMenuRequested, this, &Minesweeper::handleCellRightClick);

            cell.hasMine = false;
            cell.state = Closed;
            cell.adjacentMines = 0;
            cell.button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            cell.button->setStyleSheet(
                "margin: 0px;"
                "padding: 0px;"
                "border: 1px solid #808080;");

            updateCellAppearance(row, col);
            m_gridLayout->addWidget(cell.button, row, col);
        }
    }
}

void Minesweeper::handleCellClick()
{
    if (m_gameOver)
        return;

    QPushButton* button = qobject_cast< QPushButton* >(sender());
    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    if (m_firstClick)
    {
        placeMines(row, col);
        m_firstClick = false;
    }

    revealCell(row, col);
}

void Minesweeper::handleCellRightClick()
{
    if (m_gameOver)
        return;

    QPushButton* button = qobject_cast< QPushButton* >(sender());
    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    toggleCellMark(row, col);
}

void Minesweeper::placeMines(int firstClickRow, int firstClickCol)
{
    QVector< QPair< int, int > > safeCells;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            int row = firstClickRow + i;
            int col = firstClickCol + j;
            if (isValidCell(row, col))
            {
                safeCells.append(qMakePair(row, col));
            }
        }
    }

    int minesPlaced = 0;
    while (minesPlaced < m_mines)
    {
        int row = QRandomGenerator::global()->bounded(m_rows);
        int col = QRandomGenerator::global()->bounded(m_cols);

        bool isSafe = false;
        for (const auto& cell : safeCells)
        {
            if (cell.first == row && cell.second == col)
            {
                isSafe = true;
                break;
            }
        }

        if (!isSafe && !m_cells[row][col].hasMine)
        {
            m_cells[row][col].hasMine = true;
            minesPlaced++;
        }
    }

    for (int row = 0; row < m_rows; ++row)
    {
        for (int col = 0; col < m_cols; ++col)
        {
            if (m_cells[row][col].hasMine)
                continue;

            int count = 0;
            for (int di = -1; di <= 1; ++di)
            {
                for (int dj = -1; dj <= 1; ++dj)
                {
                    if (di == 0 && dj == 0)
                        continue;
                    int ni = row + di;
                    int nj = col + dj;
                    if (isValidCell(ni, nj) && m_cells[ni][nj].hasMine)
                    {
                        count++;
                    }
                }
            }
            m_cells[row][col].adjacentMines = count;
        }
    }
}

void Minesweeper::revealCell(int row, int col)
{
    if (!isValidCell(row, col))
    {
        safeSave();
        return;
    }

    Cell& cell = m_cells[row][col];
    if (cell.state == Opened || cell.state == Flagged)
    {
        safeSave();
        return;
    }

    if (cell.hasMine)
    {
        cell.state = Opened;
        updateCellAppearance(row, col);

        for (int r = 0; r < m_rows; ++r)
        {
            for (int c = 0; c < m_cols; ++c)
            {
                if (m_cells[r][c].hasMine && m_cells[r][c].state != Flagged)
                {
                    m_cells[r][c].state = Opened;
                    updateCellAppearance(r, c);
                }
            }
        }

        gameOver(false);
        return;
    }

    if (cell.adjacentMines > 0)
    {
        cell.state = Opened;
        m_revealedCells++;
        updateCellAppearance(row, col);
    }
    else
    {
        revealEmptyArea(row, col);
    }

    if (m_revealedCells == (m_rows * m_cols - m_mines))
    {
        for (int r = 0; r < m_rows; ++r)
        {
            for (int c = 0; c < m_cols; ++c)
            {
                if (m_cells[r][c].hasMine && m_cells[r][c].state != Flagged)
                {
                    m_cells[r][c].state = Flagged;
                    updateCellAppearance(r, c);
                }
            }
        }
        safeSave();
        gameOver(true);
    }
}

void Minesweeper::revealEmptyArea(int row, int col)
{
    QVector< QPair< int, int > > queue;
    queue.append(qMakePair(row, col));

    while (!queue.isEmpty())
    {
        auto current = queue.takeFirst();
        int r = current.first;
        int c = current.second;

        if (!isValidCell(r, c))
            continue;

        Cell& cell = m_cells[r][c];
        if (cell.state == Opened || cell.hasMine)
            continue;

        cell.state = Opened;
        m_revealedCells++;
        updateCellAppearance(r, c);

        if (cell.adjacentMines == 0)
        {
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    if (i == 0 && j == 0)
                        continue;
                    queue.append(qMakePair(r + i, c + j));
                }
            }
        }
    }
    safeSave();
}

void Minesweeper::toggleCellMark(int row, int col)
{
    if (!isValidCell(row, col))
        return;

    Cell& cell = m_cells[row][col];
    if (cell.state == Opened)
        return;

    switch (cell.state)
    {
    case Closed:
        cell.state = Flagged;
        break;
    case Flagged:
        cell.state = Closed;
        break;
    default:
        break;
    }

    safeSave();
    updateCellAppearance(row, col);
}

void Minesweeper::updateCellAppearance(int row, int col)
{
    Cell& cell = m_cells[row][col];
    QPushButton* button = cell.button;

    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setText("");
    button->setIcon(QIcon());

    QString baseStyle = "margin: 0px; padding: 0px; border: 1px solid #808080;";

    if (m_debugMode && cell.state != Opened && isFirstClick() == false)
    {
        if (cell.hasMine)
        {
            button->setIcon(QIcon("../mine.png"));
            button->setIconSize(button->size() * 0.8);
            button->setStyleSheet(baseStyle + "background-color: lightgray;");
        }
        else if (cell.adjacentMines > 0)
        {
            button->setText(QString::number(cell.adjacentMines));
            QString color;
            switch (cell.adjacentMines)
            {
            case 1:
                color = "blue";
                break;
            case 2:
                color = "green";
                break;
            case 3:
                color = "red";
                break;
            case 4:
                color = "darkblue";
                break;
            default:
                color = "black";
                break;
            }
            button->setStyleSheet(baseStyle + QString("background-color: lightgray; color: %1;").arg(color));
        }
        else
        {
            button->setStyleSheet(baseStyle + "background-color: lightgray;");
        }
        return;
    }

    switch (cell.state)
    {
    case Closed:
        button->setStyleSheet(baseStyle + "background-color: darkgray;");
        break;
    case Opened:
        if (cell.hasMine)
        {
            button->setIcon(QIcon("../mine.png"));
            button->setIconSize(button->size() * 0.8);
            if (m_lose == true)
            {
                button->setStyleSheet(baseStyle + "background-color: red;");
                m_lose = false;
            }
            else if (m_lose == true)
                button->setStyleSheet(baseStyle + "background-color: darkgray;");
        }
        else if (cell.adjacentMines > 0)
        {
            button->setText(QString::number(cell.adjacentMines));
            QString color;
            switch (cell.adjacentMines)
            {
            case 1:
                color = "blue";
                break;
            case 2:
                color = "green";
                break;
            case 3:
                color = "red";
                break;
            case 4:
                color = "darkblue";
                break;
            default:
                color = "black";
                break;
            }
            button->setStyleSheet(baseStyle + QString("background-color: lightgray; color: %1;").arg(color));
        }
        else
        {
            button->setStyleSheet(baseStyle + "background-color: lightgray;");
        }
        break;
    case Flagged:
        if (isFirstClick() == false)
        {
            button->setText("🚩");
            button->setStyleSheet(baseStyle + "background-color: darkgray;");
        }
        break;
    }
}
void Minesweeper::gameOver(bool won)
{
    m_gameOver = true;

    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Over");

    //QPixmap pix(won ? "" : "../losePic");
    //msgBox.setIconPixmap(pix.scaled(400, 400, Qt::KeepAspectRatio));

    msgBox.setText(won ? "Grats! You won!" : "boom");
    msgBox.exec();
}

int Minesweeper::countAdjacentFlags(int row, int col) const
{
    int count = 0;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0)
                continue;
            int r = row + i;
            int c = col + j;
            if (isValidCell(r, c) && m_cells[r][c].state == Flagged)
            {
                count++;
            }
        }
    }
    return count;
}

bool Minesweeper::isValidCell(int row, int col) const
{
    return row >= 0 && row < m_rows && col >= 0 && col < m_cols;
}
void Minesweeper::saveGame(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error", "Couldn`t create file");
        return;
    }

    QDataStream out(&file);

    out << (quint32)0x1;

    out << m_rows << m_cols << m_mines;
    out << m_revealedCells;
    out << m_gameOver;
    out << m_firstClick;

    for (int row = 0; row < m_rows; ++row)
    {
        for (int col = 0; col < m_cols; ++col)
        {
            const Cell& cell = m_cells[row][col];
            out << cell.hasMine;
            out << (int)cell.state;
            out << cell.adjacentMines;
        }
    }

    file.close();
}

bool Minesweeper::loadGame(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error", "Couldn`t open file");
        return false;
    }

    QDataStream in(&file);

    quint32 version;
    in >> version;
    if (version != 0x1)
    {
        QMessageBox::warning(this, "Error", "it is not ini file");
        file.close();
        return false;
    }

    int rows, cols, mines;
    in >> rows >> cols >> mines;
    in >> m_revealedCells;
    in >> m_gameOver;
    in >> m_firstClick;

    if (rows <= 0 || cols <= 0 || mines <= 0 || rows > 100 || cols > 100 || mines >= rows * cols)
    {
        QMessageBox::warning(this, "Error", "Uncorrect map parametres");
        file.close();
        return false;
    }

    if (rows != m_rows || cols != m_cols || mines != m_mines)
    {
        if (QMessageBox::question(this, "Sure?", "Map that you wanna load has different parametres. Care?", QMessageBox::Yes | QMessageBox::No) ==
            QMessageBox::No)
        {
            file.close();
            return false;
        }

        newGame(rows, cols, mines);
    }

    for (int row = 0; row < m_rows; ++row)
    {
        for (int col = 0; col < m_cols; ++col)
        {
            Cell& cell = m_cells[row][col];
            in >> cell.hasMine;

            int state;
            in >> state;
            cell.state = static_cast< CellState >(state);

            in >> cell.adjacentMines;
            updateCellAppearance(row, col);
        }
    }

    file.close();
    return true;
}

void Minesweeper::onSaveClicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save game", QDir::homePath(), "Minesweeper Save Files (*.ini)");

    if (!filename.isEmpty())
    {
        if (!filename.endsWith(".ini", Qt::CaseInsensitive))
        {
            filename += ".ini";
        }
        saveGame(filename);
    }
}

void Minesweeper::onLoadClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load game", QDir::homePath(), "Minesweeper Save Files (*.ini)");

    if (!filename.isEmpty())
    {
        if (!loadGame(filename))
        {
            QMessageBox::warning(this, "Error", "Couldn`t load game");
        }
    }
}
void Minesweeper::onQuickSave()
{
    QString savePath = "../quicksave.ini";
    saveGame(savePath);
}
void Minesweeper::onQuickLoad()
{
    QString savePath = "../quicksave.ini";
    loadGame(savePath);
}

void Minesweeper::safeSave()
{
    QString savePath = "../safeSaves.ini";
    saveGame(savePath);
}
