#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <QSlider>
#include <QComboBox>
#include <QLabel>

class Process{
  public:
    Process(QString aname = "", int ax = 0, int ay = 0,
            int alength = 100, int aheight = 25, QColor acolor = QColor(0,180,0,120));
    ~Process();
    //void hide();
    void draw(/*QColor col = Qt::black*/);
    void drawShadow();
    //void show(); //тоже самое, что и draw?
    //void moveleft();
    void move(int dx, int dy);

    int getX();
    int getY();
    int getLength();
    int getStartLength();
    int getHeight();
    QString getName();
    int getNumber();
    QColor getColor();

    void setX(int ax);
    void setY(int ay);
    void setLength(int len);
    void setPainter(QPainter *paint);
    void setNumber(int num);
    void setColor(QColor col);
    void setStartLength(int len);
  private:
    int x, y;
    int length;
    int startlength;
    int height;
    int number;
    //int status;
    QString name;
    QColor color;
    QPainter *painter;
};

class Area{
public:
  int getOriginX();
  int getOriginY();
  int getLengthX();
  int getHeightY();

  void setPainter(QPainter *paint);
protected:
  int originX, originY;
  int lengthX, heightY;
  std::vector <Process> processes;
  QPainter *painter;
};

class Queue : public Area{
public:
  Queue(int aoriginX = 30, int aoriginY = 610,
          int alengthX = 385, int aheightY = 550, int astepY = 40);
  ~Queue();
  void drawLabel();
  void draw();
  void sort();
  bool greater(Process pr1, Process pr2);
  void addProcess(Process proc);
  Process findProcess();
  Process firstProcess();
  bool empty();

  int getStepY();

  void setStepY(int step);
  void setSlider(int sl);
  void setSorting(QString sort);
private:
  int stepY;
  int lastNumber = 0;
  int slider;
  QString sorting;
  Area *bar;
  bool waiting = true;
};

class Graph : public Area{
public:
  Graph(Queue *q, int aoriginX = 450, int aoriginY = 610,
          int alengthX = 740, int aheightY = 550, int astepY = 40);
  ~Graph();
  void drawAxis();
  void draw();
  void update(int dx);
  void createProcess(QString name = "", int len = 100, int height = 25, QColor col = QColor(0,180,0,120));
  void sendToQueue();

  int getStepY();

  void setStepY(int step);
private:
  int stepY;
  Queue *queue;
};

class ExecutionBar : public Area{
public:
    ExecutionBar(Queue *q, int aoriginX = 30, int aoriginY = 700,
            int alengthX = 1170, int aheightY = 50);
    ~ExecutionBar();
    void drawLabel();
    void draw();
    void update(int dx);
    void requestForNext();
    void clear();
    bool enough();
    void setSlider(int sl);
    void setAlg(QString a);
    void setQuantum(int q);
private:
    Queue *queue;
    int slider;
    QString alg;
    int endX = 0;
    int quantum = 20;
    bool waiting = true;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void paintEvent(QPaintEvent *event);
    void timerActivation();

    void on_algorithmBox_currentTextChanged(const QString &arg1);

    void on_speedSlider_valueChanged(int value);

    void on_startButton_clicked();

    void on_barSlider_sliderMoved(int position);

    void on_queueSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    ExecutionBar * bar;
    Queue *queue;
    Graph *graph;
    //Process *proc;
    QPainter *painter;
    bool timeFlag = false;
    long long count = 0;
};

#endif // MAINWINDOW_H
