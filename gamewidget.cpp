#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <QRectF>
#include <QPainter>
#include <qmath.h>
#include "gamewidget.h"

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    generations(-1),
    FirstGenSize(40) // Размер вселенной (по умолчанию)
{
    timer->setInterval(100); // Интервал (по умолчанию)
    ColorCells = "#000"; // Цвет точек (по умолчанию)
    FirstGen = new bool[(FirstGenSize + 2) * (FirstGenSize + 2)];
    SecondGen = new bool[(FirstGenSize + 2) * (FirstGenSize + 2)];
    connect(timer, SIGNAL(timeout()), this, SLOT(newGeneration()));
    memset(FirstGen, false, sizeof(bool)*(FirstGenSize + 2) * (FirstGenSize + 2));
    memset(SecondGen, false, sizeof(bool)*(FirstGenSize + 2) * (FirstGenSize + 2));
}

GameWidget::~GameWidget()
{
    delete [] FirstGen;
    delete [] SecondGen;
}

void GameWidget::startGame(const int &number) // Функция старта игры
{
    generations = number;
    timer->start();
}

void GameWidget::stopGame() // Функция остановки игры
{
    timer->stop();
}

void GameWidget::clear() // Функция очистки вселенной
{
    for(int k = 1; k <= FirstGenSize; k++) {
        for(int j = 1; j <= FirstGenSize; j++) {
            FirstGen[k*FirstGenSize + j] = false;
        }
    }
    gameEnds(true);
    update();
}

int GameWidget::cellNumber()
{
    return FirstGenSize;
}

void GameWidget::setCellNumber(const int &s)
{
    FirstGenSize = s;
    resetFirstGen();
    update();
}

void GameWidget::resetFirstGen()
{
    delete [] FirstGen;
    delete [] SecondGen;
    FirstGen = new bool[(FirstGenSize + 2) * (FirstGenSize + 2)];
    SecondGen = new bool[(FirstGenSize + 2) * (FirstGenSize + 2)];
    memset(FirstGen, false, sizeof(bool)*(FirstGenSize + 2) * (FirstGenSize + 2));
    memset(SecondGen, false, sizeof(bool)*(FirstGenSize + 2) * (FirstGenSize + 2));
}

QString GameWidget::dump()
{
    char temp;
    QString master = "";
    for(int k = 1; k <= FirstGenSize; k++) {
        for(int j = 1; j <= FirstGenSize; j++) {
            if(FirstGen[k*FirstGenSize + j] == true) {
                temp = '*';
            } else {
                temp = 'o';
            }
            master.append(temp);
        }
        master.append("\n");
    }
    return master;
}

void GameWidget::setDump(const QString &data)
{
    int current = 0;
    for(int k = 1; k <= FirstGenSize; k++) {
        for(int j = 1; j <= FirstGenSize; j++) {
            FirstGen[k*FirstGenSize + j] = data[current] == '*';
            current++;
        }
        current++;
    }
    update();
}

int GameWidget::interval()
{
    return timer->interval();
}

void GameWidget::setInterval(int msec)
{
    timer->setInterval(msec);
}

bool GameWidget::isAlive(int k, int j)
{
    int power = 0;
    power += FirstGen[(k+1)*FirstGenSize +  j];
    power += FirstGen[(k-1)*FirstGenSize + j];
    power += FirstGen[k*FirstGenSize + (j+1)];
    power += FirstGen[k*FirstGenSize + (j-1)];
    power += FirstGen[(k+1)*FirstGenSize + ( j-1)];
    power += FirstGen[(k-1)*FirstGenSize + (j+1)];
    power += FirstGen[(k-1)*FirstGenSize + (j-1)];
    power += FirstGen[(k+1)*FirstGenSize +  (j+1)];
    if (((FirstGen[k*FirstGenSize + j] == true) && (power == 2)) || (power == 3))
           return true;
    return false;
}

void GameWidget::newGeneration()
{
    if(generations < 0)
        generations++;
    int notChanged=0;
    for(int k=1; k <= FirstGenSize; k++) {
        for(int j=1; j <= FirstGenSize; j++) {
            SecondGen[k*FirstGenSize + j] = isAlive(k, j);
            if(SecondGen[k*FirstGenSize + j] == FirstGen[k*FirstGenSize + j])
                notChanged++;
        }
    }
    if(notChanged == FirstGenSize*FirstGenSize) {
        QMessageBox::information(this,
                                 tr(" "),
                                 tr("Game Over"),
                                 QMessageBox::Ok);
        stopGame();
        gameEnds(true);
        return;
    }
    for(int k=1; k <= FirstGenSize; k++) {
        for(int j=1; j <= FirstGenSize; j++) {
            FirstGen[k*FirstGenSize + j] = SecondGen[k*FirstGenSize + j];
        }
    }
    update();
    generations--;
    if(generations == 0) {
        stopGame();
        gameEnds(true);
        QMessageBox::information(this,
                                 tr(" "),
                                 tr("Game Over."), // Конец итерации
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel);
    }
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paintGrid(p);
    paintFirstGen(p);
}

void GameWidget::mousePressEvent(QMouseEvent *e)
{
    emit environmentChanged(true);
    double cellWidth = (double)width()/FirstGenSize;
    double cellHeight = (double)height()/FirstGenSize;
    int k = floor(e->y()/cellHeight)+1;
    int j = floor(e->x()/cellWidth)+1;
    FirstGen[k*FirstGenSize + j] = !FirstGen[k*FirstGenSize + j];
    update();
}

void GameWidget::mouseMoveEvent(QMouseEvent *e)
{
    double cellWidth = (double)width()/FirstGenSize;
    double cellHeight = (double)height()/FirstGenSize;
    int k = floor(e->y()/cellHeight)+1;
    int j = floor(e->x()/cellWidth)+1;
    int currentLocation = k*FirstGenSize + j;
    if(!FirstGen[currentLocation]){   //Если текущая клетка чиста,то заполняет её
        FirstGen [currentLocation]= !FirstGen[currentLocation];
        update();
    }
}

void GameWidget::paintGrid(QPainter &p)
{
    QRect borders(0, 0, width()-1, height()-1); // Границы вселенной
    QColor gridColor = ColorCells; // Цвет сетки
    gridColor.setAlpha(10); // Цвет сетки должен быть светлее,чем основной (чёрный)
    p.setPen(gridColor);
    double cellWidth = (double)width()/FirstGenSize; // Ширина виджета (Количество клеток в одной строке)
    for(double k = cellWidth; k <= width(); k += cellWidth)
        p.drawLine(k, 0, k, height());
    double cellHeight = (double)height()/FirstGenSize; // Высота виджета (Количество клеток в одной строке)
    for(double k = cellHeight; k <= height(); k += cellHeight)
        p.drawLine(0, k, width(), k);
    p.drawRect(borders);
}

void GameWidget::paintFirstGen(QPainter &p)
{
    double cellWidth = (double)width()/FirstGenSize;
    double cellHeight = (double)height()/FirstGenSize;
    for(int k=1; k <= FirstGenSize; k++) {
        for(int j=1; j <= FirstGenSize; j++) {
            if(FirstGen[k*FirstGenSize + j] == true) {
                qreal left = (qreal)(cellWidth*j-cellWidth); // Отступ слева
                qreal top  = (qreal)(cellHeight*k-cellHeight); // Отступ сверху
                QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight);
                p.fillRect(r, QBrush(ColorCells)); // Заполнение клетки основным цветом
            }
        }
    }
}

QColor GameWidget::masterColor()
{
    return ColorCells;
}

void GameWidget::setMasterColor(const QColor &color)
{
    ColorCells = color;
    update();
}
