#include "colorlinesmodel.h"
#include <random>
#include <QSqlQuery>

#define TABLE_NAME "DataTable"

QString colors[4] = {"darkRed", "darkGreen", "darkCyan", "darkMagenta"};

ColorLinesModel::ColorLinesModel(QObject *parent)
    : QAbstractItemModel(parent), m_SelectedBall({-1, -1}), m_score(), m_EmptyCells(9 * 9), m_save(QSqlDatabase::addDatabase("QSQLITE")), m_cells()
{
    m_save.setDatabaseName("save"); //Задаем базе данных имя. Данная функция автоматически создает фаил если его нет
    m_save.open();

    // Проверяем наличие таблицы с именем TABLE_NAME в базе данных,
    // если нет, то создаем и добавляем строку с нулевыми значениями.
    if (!m_save.tables(QSql::AllTables).contains(TABLE_NAME)) {
        QSqlQuery query{m_save};
        query.exec("CREATE TABLE " TABLE_NAME " (Score INTEGER, Empty INTEGER, Data BLOB)");
        query.prepare("INSERT INTO " TABLE_NAME " (Score, Empty, Data) VALUES (:score, :empty, :data)");
        query.bindValue(":score", m_score);
        query.bindValue(":empty", m_EmptyCells);
        query.bindValue(":data", m_charCells);

        query.exec();
        addBall();
        m_save.close();
    } else {
        loadSave();
    }
}

QModelIndex ColorLinesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    return createIndex(row, column);
}

QModelIndex ColorLinesModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int ColorLinesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 9;
}

int ColorLinesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 9;
}

QVariant ColorLinesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto& ball = m_cells[index.row()][index.column()];
    if (role == ColorRole) {
        return ball.m_color;
    } else if (role == VisibleRole) {
        return ball.m_visible ? "visible" : "notVisible";
    } else if (role == SelectedRole) {
        if (m_SelectedBall.x == index.row() && m_SelectedBall.y == index.column()) {
            return true;
        } else {
            return false;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ColorLinesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ColorRole] = "color";
    roles[VisibleRole] = "visible";
    roles[SelectedRole] = "selected";
    return roles;
}

int ColorLinesModel::score() const
{
    return m_score;
}

bool ColorLinesModel::addBall()
{
    // Генератор случайных чисел
    static std::random_device seed;
    static std::mt19937 generator(seed());
    static std::uniform_int_distribution<uint> DistributeCell(0, 8);
    static std::uniform_int_distribution<uint> DistributeColor(0, 3);

    Position temp[3];
    if (m_EmptyCells < 3) {
        return false;
    }
    for(int i = 0; i < 3; i++) {
        auto& pos = temp[i];

        pos.x = DistributeCell(generator);
        pos.y = DistributeCell(generator);
        uint color = DistributeColor(generator);

        auto& cell = m_cells[pos.x][pos.y];
        if (cell.m_visible == true) {
            i--;
        } else {
            cell.m_color = colors[color];
            cell.m_visible = true;
        }
    }
    m_EmptyCells -= 3;
    return true;
}

bool ColorLinesModel::selectBall(int row, int column)
{
    if (!m_cells[row][column].m_visible) {
        return false;
    }
    if (m_SelectedBall.x == row && m_SelectedBall.y == column) {
        m_SelectedBall = {-1, -1};
        return false;
    } else {
        m_SelectedBall = {row, column};
        return true;
    }
}

bool ColorLinesModel::ballMove(int row, int column)
{
    if (m_SelectedBall.x == -1) {
        return false;
    }

    auto& cell = m_cells[row][column];

    if (cell.m_visible) {
        return false;
    }

    auto& ball = m_cells[m_SelectedBall.x][m_SelectedBall.y];

    cell = ball;
    ball.m_visible = false;
    m_SelectedBall = {-1, -1};

    emit dataChanged(index(0, 0), index(8, 8)); // update();
    return true;

}

bool ColorLinesModel::checkLines()
{
    if (checkRows() || checkColumns()) {
        return true;
    }
    return false;
}

bool ColorLinesModel::checkRows()
{
    uint line = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (m_cells[i][j].m_visible) {
                if (m_cells[i][j] == m_cells[i][j + 1]) {
                    line++;
                    if (line == 4) {
                        m_cells[i][j - 3].m_visible = false;
                        m_cells[i][j - 2].m_visible = false;
                        m_cells[i][j - 1].m_visible = false;
                        m_cells[i][j].m_visible     = false;
                        m_cells[i][j + 1].m_visible = false;
                        return true;
                    }
                } else {
                    line = 0;
                }
            } else if (i == 8 && j == 7) {
                return false;
            }
        }
    }
    return false;
}

bool ColorLinesModel::checkColumns()
{
    uint line = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (m_cells[j][i].m_visible) {
                if (m_cells[j][i] == m_cells[j + 1][i]) {
                    line++;
                    if (line == 4) {
                        m_cells[j - 3][i].m_visible = false;
                        m_cells[j - 2][i].m_visible = false;
                        m_cells[j - 1][i].m_visible = false;
                        m_cells[j][i].m_visible     = false;
                        m_cells[j + 1][i].m_visible = false;
                        return true;
                    }
                } else {
                    line = 0;
                }
            } else if (i == 8 && j == 7) {
                return false;
            }
        }
    }
    return false;
}

void ColorLinesModel::update()
{
    if (!checkLines()) {
        if (!addBall()) {
           emit gameOverChanged();
        }
    } else {
        m_EmptyCells += 5;
        m_score += 10;
        emit scoreChanged();
    }

    emit dataChanged(index(0, 0), index(8, 8));
}

void ColorLinesModel::reset()
{
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            m_cells[i][j].m_visible = false;
        }
    }
    m_score = 0;
    m_EmptyCells = 9 * 9;
    addBall();
    emit dataChanged(index(0, 0), index(8, 8));
}

void ColorLinesModel::save()
{
    QByteArray data;
    data.append(m_charCells, sizeof(m_charCells));

    m_save.open();
    QSqlQuery query{m_save};
    query.prepare("UPDATE " TABLE_NAME " SET Score=:score, Empty=:empty, Data=:data");
    query.bindValue(":score", m_score);
    query.bindValue(":empty", m_EmptyCells);
    query.bindValue(":data", data);
    query.exec();
    m_save.close();
}

void ColorLinesModel::loadSave()
{
    QSqlQuery query{m_save};
    query.exec("SELECT Score, Empty, Data FROM " TABLE_NAME);
    query.first();
    m_score = query.value(0).toUInt();
    m_EmptyCells = query.value(1).toUInt();
    for (int i = 0; i < sizeof(m_charCells); i++) {
        m_charCells[i] = query.value(2).toByteArray().data()[i];
    }
    m_save.close();
}

QModelIndex ColorLinesModel::previousIndex()
{
    if (m_SelectedBall.x != -1) {
        return createIndex(m_SelectedBall.x, m_SelectedBall.y);
    }
    return QModelIndex();
}
