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
    //void show(); //тоже самое, что и draw?
    //void moveleft();
    void move(int dx, int dy);

    int getX();
    int getY();
    int getLength();
    int getStartLength();
    int getHeight();
    QString getName();

    void setX(int ax);
    void setY(int ay);
    void setLength(int len);
    void setPainter(QPainter *paint);
  private:
    int x, y;
    int length;
    int startlength;
    int height;
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

class ExecutionBar : public Area{
public:
    ExecutionBar(int aoriginX = 30, int aoriginY = 680,
            int alengthX = 1170, int aheightY = 50);
    ~ExecutionBar();
    void drawLabel();
    void draw();
    void requestForNext();
    void clear();
};

class Queue : public Area{
public:
  Queue(int aoriginX = 30, int aoriginY = 600,
          int alengthX = 385, int aheightY = 520, int astepY = 40);
  ~Queue();
  void drawLabel();
  void draw();
  void sort();
  void addProcess(Process proc);
  Process findProcess();

  int getStepY();

  void setStepY(int step);
  void setSlider(QSlider *sl);
  void setSorting(QComboBox *sort);
private:
  int stepY;
  QSlider *slider;
  QComboBox *sorting;
};

class Graph : public Area{
public:
  Graph(Queue *q, int aoriginX = 450, int aoriginY = 600,
          int alengthX = 740, int aheightY = 520, int astepY = 40);
  ~Graph();
  void drawAxis();
  void draw();
  void update(int dx);
  void createProcess(QString name = "", int len = 100, int height = 25, QColor col = QColor(0,180,0,120));
  void sendToQueue(Process proc);

  int getStepY();

  void setStepY(int step);
private:
  int stepY;
  Queue *queue;
};


//class Graph{
//public:
//  Graph(int aoriginX = 100, int aoriginY = 600,
//          int alengthX = 1000, int aheightY = 500, int astepY = 40);
//  ~Graph();
//  void drawAxis();
//  void draw();
//  void update(int dx);
//  void createProcess(QString name = "", int len = 100, int height = 25, QColor col = QColor(0,180,0,120));
//  void sendToQueue(Process proc);

//  int getOriginX();
//  int getOriginY();
//  int getLengthX();
//  int getHeightY();
//  int getStepY();

//  void setStepY(int step);
//  void setPainter(QPainter *paint);
//private:

//  int originX, originY;
//  int lengthX, heightY;
//  int stepY;
//  std::vector <Process> processes;
//  QPainter *painter;
//};




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
