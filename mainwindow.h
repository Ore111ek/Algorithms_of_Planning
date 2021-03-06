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

    void draw();
    void drawShadow();
    void move(int dx, int dy);

    int getX();
    int getY();
    int getLength();
    int getStartLength();
    int getHeight();
    QString getName();
    int getNumber();
    QColor getColor();
    unsigned long long getStart();

    void setX(int ax);
    void setY(int ay);
    void setLength(int len);
    void setPainter(QPainter *paint);
    void setNumber(int num);
    void setColor(QColor col);
    void setStartLength(int len);
    void setStart(unsigned long long t);
  private:
    int x, y;
    int length;
    int startlength;
    int height;
    int number;
    QString name;
    QColor color;
    QPainter *painter;
    unsigned long long start = 0;
};

class Area{
public:
  void restart();

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
  Queue(int aoriginX = 30, int aoriginY = 460,
          int alengthX = 385, int aheightY = 395, int astepY = 40);
  ~Queue();

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
  void drawLabel();

  int stepY;
  int lastNumber = 0;
  int slider;
  QString sorting;
  bool waiting = true;
};

class Graph : public Area{
public:
  Graph(Queue *q, int aoriginX = 450, int aoriginY = 460,
          int alengthX = 740, int aheightY = 395, int astepY = 40);
  ~Graph();
  void draw();
  void update(int dx);
  void createProcess(QString name = "", int len = 100, int height = 25, QColor col = QColor(0,180,0,120));
  void sendToQueue();

  int getStepY();

  void setStepY(int step);
private:
  void drawAxis();
  int stepY;
  Queue *queue;
  unsigned long long time = 0;
};

class ExecutionBar : public Area{
public:
    ExecutionBar(Queue *q, int aoriginX = 30, int aoriginY = 545,
            int alengthX = 1170, int aheightY = 50);
    ~ExecutionBar();

    void restart();
    void draw();
    void update(int dx);
    void requestForNext();
    void clear();
    bool enough();
    void setSlider(int sl);
    void setAlg(QString a);
    void setQuantum(int q);
    void setDelay(int d);
    void setWindow(QMainWindow *w);
private:
    void drawLabel();
    QMainWindow *window;
    Queue *queue;
    int slider;
    QString alg;
    int endX = 0;
    int quantum = 20;
    bool waiting = true;

    int delay = 0;
    int remainingDelay = 0;

    int usetime = 0;
    int idletime = 0;
    int delaytime = 0;

    unsigned long long time = 0;
    unsigned long long turnaround = 0;
    int count = 0;
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

    void on_quantSpin_valueChanged(int arg1);

    void on_delaySpin_valueChanged(int arg1);

    void on_restartButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    ExecutionBar * bar;
    Queue *queue;
    Graph *graph;
    QPainter *painter;
    bool timeFlag = false;
    long long count = 0;
    unsigned long long time = 0;
};

#endif // MAINWINDOW_H
