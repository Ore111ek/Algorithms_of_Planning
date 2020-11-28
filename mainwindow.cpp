#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>

Process::Process(QString aname, int ax, int ay, int alength, int priory, int aheight, QColor acolor){
    x = ax;
    y = ay;
    length = startlength = alength;
    height = aheight;
    color = acolor;
    name = aname;
    priority = priory;
}

Process::~Process(){
}

void Process::draw(/*QColor col*/){
    painter->fillRect(x, y, length, height, QBrush(color));
    painter->setPen(/*col*/Qt::black);
    painter->drawRect(x, y, length, height);
    painter->drawText(QRect(x, y, length, height),name,(QTextOption)Qt::AlignCenter);
}
/*
void Process::show(){
    draw();
}

void Process::hide(){
    QColor col = color;
    color = QColor(255,255,255,0);
    draw(color);
    color = col;
}*/

void Process::move(int dx, int dy){
    //hide();
    x += dx;
    y += dy;
    //show();
    draw();
}

int Process::getX(){ return x; }
int Process::getY(){ return y; }
int Process::getLength(){ return length; }
int Process::getStartLength(){ return startlength; }
int Process::getHeight(){ return height; }
QString Process::getName(){ return name; }
int Process::getNumber(){ return number; }
int Process::getPriority(){ return priority; }

void Process::setX(int ax){ x = ax; }
void Process::setY(int ay){ y = ay; }
void Process::setLength(int len){ length = len; }
void Process::setPainter(QPainter *paint){ painter = paint; }
void Process::setNumber(int num){ number = num; }

int Area::getOriginX(){ return originX; }
int Area::getOriginY(){ return originY; }
int Area::getLengthX(){ return lengthX; }
int Area::getHeightY(){ return heightY; }
void Area::setPainter(QPainter *paint){ painter = paint; }


Queue::Queue(int aoriginX, int aoriginY,
             int alengthX, int aheightY, int astepY){
    originX = aoriginX; originY = aoriginY;
    lengthX = alengthX; heightY = aheightY;
    stepY = astepY; lastNumber = 0;
}

Queue::~Queue(){}

void Queue::drawLabel(){
    painter->fillRect(originX, originY - heightY - 40, lengthX, 40, QBrush(QColor(255,255,255)));//Закрасить белым над областью, на месте лейбла
    painter->drawText(QRect(originX, originY - heightY - 40, lengthX, 40),"Очередь",(QTextOption)Qt::AlignCenter);
    painter->fillRect(originX, originY, lengthX, 40, QBrush(QColor(255,255,255)));//Закрасить белым над областью, на месте лейбла
    painter->drawRect(originX, originY - heightY, lengthX, heightY); //Рамка области
}
void Queue::draw(){
    int y = originY - 35;
    int heightOfAll = 0;
    for(auto &process : processes){
        heightOfAll += stepY;
    }
    int space = heightOfAll - heightY;
    int margin = slider->value()*space/100;
    for(auto &process : processes){

        if(y > (originY - heightY - margin - process.getHeight()) && y < originY - margin/* - process.getHeight()*/){
            process.setPainter(painter);
            //process.setX(originX+10);
            process.setX(originX + (lengthX - process.getLength())/2);
            process.setY(y + margin);
            process.draw();

        }
         y -= stepY;
    }
    drawLabel();
}
bool Queue::greater(Process pr1, Process pr2){
    if(sorting->currentText() == "Первым пришёл - первым обслужен" || sorting->currentText() == "Циклическое планирование")
        return pr1.getNumber() > pr2.getNumber();
    if(sorting->currentText() == "Краткосрочное планирование" || sorting->currentText() == "Наименьшее оставшееся время")
        return pr1.getLength() > pr2.getLength();
    if(sorting->currentText() == "Приоритетное планирование")
        return pr1.getPriority() > pr2.getPriority();
}
void Queue::sort(){
    for(int i = 1; i < (int)processes.size(); i++){
            for(int j = i; j > 0 && greater(processes[j-1], processes[j]); j--){
                Process tmp = processes[j-1];
                processes[j-1] = processes[j];
                processes[j] = tmp;
            }
    }
}
void Queue::addProcess(Process proc){
    lastNumber++;
    proc.setNumber(lastNumber);
    processes.push_back(proc);
    sort();
}
Process Queue::findProcess(){
}

int Queue::getStepY(){ return stepY; }
void Queue::setStepY(int step){ stepY = step; }
void Queue::setSlider(QSlider *sl){ slider = sl; }
void Queue::setSorting(QComboBox *sort){ sorting = sort; }


ExecutionBar::ExecutionBar(int aoriginX, int aoriginY,
             int alengthX, int aheightY){
    originX = aoriginX; originY = aoriginY;
    lengthX = alengthX; heightY = aheightY;
}
ExecutionBar::~ExecutionBar(){}
void ExecutionBar::drawLabel(){
    painter->drawText(QRect(originX, originY - heightY - 30, lengthX, 30),"Выполняемые процессы",(QTextOption)Qt::AlignCenter);
    painter->drawRect(originX, originY - heightY, lengthX, heightY);
}
void ExecutionBar::draw(){
    for(auto process: processes){

    }
    drawLabel();
}
void ExecutionBar::requestForNext(){

}
void ExecutionBar::clear(){

}

Graph::Graph(Queue *q, int aoriginX, int aoriginY,
             int alengthX, int aheightY, int astepY){
    originX = aoriginX; originY = aoriginY;
    lengthX = alengthX; heightY = aheightY;
    stepY = astepY; queue = q;
}

Graph::~Graph(){
}

void Graph::drawAxis(){
    painter->setPen(Qt::black);
    painter->drawLine(originX, originY, originX + lengthX, originY);
    painter->drawLine(originX, originY, originX, originY - heightY);
    painter->drawLine(originX + lengthX, originY, originX + lengthX - 15, originY - 5);
    painter->drawLine(originX + lengthX, originY+1, originX + lengthX - 15, originY + 5);
    painter->drawLine(originX, originY - heightY, originX - 5, originY - heightY + 15);
    painter->drawLine(originX, originY - heightY, originX + 5, originY - heightY + 15);
/*
        for(int i = stepY; i < heightY; i+=stepY){
            painter->setPen(Qt::gray);
            painter->drawLine(originX, originY - i, originX + lengthX, originY - i);
            painter->setPen(Qt::black);
        }*/
}

void Graph::draw(){
    drawAxis();
    for(auto &process: processes){
        process.setPainter(painter);
        process.draw();
    }
}

void Graph::sendToQueue(Process proc){
    queue->addProcess(proc);
    processes.erase(processes.begin());
}

void Graph::update(int dx){
    drawAxis();
    for(auto &process: processes){
        if(process.getX()<originX + dx){
            sendToQueue(process);
        }
        else{
            process.setPainter(painter);
            process.move(dx,0);
        }
    }
}

void Graph::createProcess(QString name, int len, int height, QColor col){
    bool free = false;
    int y = originY - stepY;
    while(y > originY - heightY && !free){
        free = true;
        for(auto process: processes){
            if(process.getY() == y && process.getX() + process.getLength() >= originX + lengthX - 1)
                free = false;
        }
        if(free){
             processes.push_back(Process(name, originX + lengthX, y, len, 1, height, col));
             processes[processes.size()-1].setPainter(painter);
             processes[processes.size()-1].draw();
        }
        y -= stepY;
    }
}

int Graph::getStepY(){ return stepY; }
void Graph::setStepY(int step){ stepY = step; }




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bar = new ExecutionBar;
    queue = new Queue();
    queue->setSlider(ui->queueSlider);
    queue->setSorting(ui->algorithmBox);
    graph = new Graph(queue);
    //proc = new Process("П1", 1000, 120, 100);
    //update();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerActivation()));
    timer->start(10);
}

void MainWindow::paintEvent(QPaintEvent *event){
    painter = new QPainter(this);
    bar->setPainter(painter);
    queue->setPainter(painter);

    graph->setPainter(painter);

   // proc->setPainter(painter);
    int xzero = 11;
    int yzero = 40;
    int width = 1208;
    int height = 650;
    painter->fillRect(xzero,yzero,width,height, QBrush(QColor(255,255,255)));
    painter->setPen(Qt::black);
    if(timeFlag){
        // proc->move(-1,0);

        queue->draw();
        bar->drawLabel();
         graph->update(-1);
         if(rand()%200 + 198 < 200){
             count++;
            graph->createProcess("П" + QString::number(count),rand()%170 + 25, 25, QColor(rand()%156+100,rand()%156+100,rand()%156+100,120));
         }
    }

    painter->fillRect(0,0,xzero,height+yzero, QBrush(QColor("#f0f0f0")));
    painter->fillRect(xzero + width,0,xzero*5,height+yzero, QBrush(QColor("#f0f0f0")));
        painter->fillRect(0,0,xzero*2 + width,yzero, QBrush(QColor("#f0f0f0")));
    painter->end();
}

void MainWindow::timerActivation()
{
    ui->labelSpeed->setText(QTime::currentTime().toString("hh:mm:ss"));
    timeFlag = true;
    this->repaint();
    timeFlag = false;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete painter;
    delete graph;
    delete queue;
    delete bar;
}

void MainWindow::on_algorithmBox_currentTextChanged(const QString &arg1)
{
    queue->sort();
}
