#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QColor>
#include <QWidget>

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:
    //Когда клетка изменила своё состояние,то выпускает сигнал о блокировке FirstGenSize
    void environmentChanged(bool ok);
    //Когда игра закончена или вселенная очищена,то выпускает сигнал о разблокировке FirstGenSize
    void gameEnds(bool ok);

public slots:
    void startGame(const int &number = -1); // Старт
    void stopGame(); // Финиш
    void clear(); // Очистить

    int cellNumber(); // Количество клеток в одной строке
    void setCellNumber(const int &s); // Установка кол-ва клеток в одной строке

    int interval(); // Интервал между поколениями
    void setInterval(int msec); // Установка интервала

    QColor masterColor(); // Цвет клеток
    void setMasterColor(const QColor &color); // Установка цвета

    QString dump(); // Вывод текущей вселенной на экран
    void setDump(const QString &data); // Установка вывода

private slots:
    void paintGrid(QPainter &p);
    void paintFirstGen(QPainter &p);
    void newGeneration();

private:
    QColor ColorCells;
    QTimer* timer;
    int generations;
    bool* FirstGen;
    bool* SecondGen;
    int FirstGenSize;
    bool isAlive(int k, int j); // Возвращает "true",если FirstGen[k][j] удовлетворяет правилам
    void resetFirstGen();// Сброс размера вселенной
};

#endif // GAMEWIDGET_H
