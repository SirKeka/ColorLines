#ifndef COLORLINESMODEL_H
#define COLORLINESMODEL_H

#include <QAbstractItemModel>
#include <QColor>
#include <QSqlDatabase>
#include <QItemSelection>

struct Ball
{
    QColor m_color;
    bool m_visible;


    Ball() : m_color(), m_visible(false) {}
    Ball(const QColor& color, bool visible = true) : m_color(color), m_visible(visible) {}

    bool operator ==(const Ball& b) {
        if (m_color == b.m_color && m_visible == b.m_visible) {
            return true;
        }
        return false;
    }
};

struct Position
{
    int x, y;

    bool operator ==(const Position & p) {
        if(x != p.x && y != p.y) {
            return false;
        }
        return true;
    }
};

enum Role
{
    ColorRole = Qt::UserRole,
    VisibleRole,
    SelectedRole
};

class ColorLinesModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(int score READ score NOTIFY scoreChanged)

public:
    explicit ColorLinesModel(QObject *parent = nullptr);

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int score() const;

    /// @brief Функция добавляет шары на поле
    Q_INVOKABLE bool addBall();

    /// @brief Функция выделяет шар сохраняя его координаты
    /// @param row строка(координата х) в которой находится шар
    /// @param column колонка(координата y) в которой нажодится шар
    /// @return true если шар выделен, иначе false
    Q_INVOKABLE bool selectBall(int row, int column);

    /// @brief Функция перемещает выделенный шар в указанные координаты
    /// @param row строка(координата х) куда нужно переместить шар
    /// @param column колонка(координата y) куда нужно переместить шар
    Q_INVOKABLE bool ballMove(int row, int column);

    Q_INVOKABLE QModelIndex previousIndex();

    bool checkLines();
    bool checkRows();
    bool checkColumns();

    Q_INVOKABLE void update();

    /// @brief Функция сбрасывает все данные модели в первоначальный вариант
    Q_INVOKABLE void reset();

    /// @brief Функция сохраняет текущее состояние
    Q_INVOKABLE void save();

    /// @brief Функция загружает сохраненное состояние
    void loadSave();
signals:
    void scoreChanged();
    void gameOverChanged();

private:
    Position m_SelectedBall;
    uint m_score;
    uchar m_EmptyCells;
    QSqlDatabase m_save;

    union {
        Ball m_cells[9][9];                 // Массив ячеек
        char m_charCells [sizeof(m_cells)]; // Массив байтов
    };
};

#endif // COLORLINESMODEL_H
