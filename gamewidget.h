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
    //����� ������ �������� ��� ���������,�� ��������� ������ � ���������� FirstGenSize
    void environmentChanged(bool ok);
    //����� ���� ��������� ��� ��������� �������,�� ��������� ������ � ������������� FirstGenSize
    void gameEnds(bool ok);

public slots:
    void startGame(const int &number = -1); // �����
    void stopGame(); // �����
    void clear(); // ��������

    int cellNumber(); // ���������� ������ � ����� ������
    void setCellNumber(const int &s); // ��������� ���-�� ������ � ����� ������

    int interval(); // �������� ����� �����������
    void setInterval(int msec); // ��������� ���������

    QColor masterColor(); // ���� ������
    void setMasterColor(const QColor &color); // ��������� �����

    QString dump(); // ����� ������� ��������� �� �����
    void setDump(const QString &data); // ��������� ������

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
    bool isAlive(int k, int j); // ���������� "true",���� FirstGen[k][j] ������������� ��������
    void resetFirstGen();// ����� ������� ���������
};

#endif // GAMEWIDGET_H
