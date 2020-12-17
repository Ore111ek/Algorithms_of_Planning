#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>

Process::Process(QString aname, int ax, int ay, int alength, int aheight, QColor acolor){
    x = ax;
    y = ay;
    length = startlength = alength;
    height = aheight;
    color = acolor;
    name = aname;
}

Process::~Process(){
}

void Process::draw(/*QColor col*/){
    painter->fillRect(x, y, length, height, QBrush(color));
    painter->setPen(/*col*/Qt::black);
    painter->drawRect(x, y, length, height);
    painter->drawText(QRect(x, y, length, height),name,static_cast<QTextOption>(Qt::AlignCenter));
}
void Process::drawShadow(){
    painter->fillRect(x, y, startlength, height, QBrush(color));
    painter->setPen(/*col*/Qt::lightGray);
    painter->drawRect(x, y, startlength, height);
    painter->setPen(/*col*/Qt::black);
}
void Process::move(int dx, int dy){
    x += dx;
    y += dy;
    draw();
}

int Process::getX(){ return x; }
int Process::getY(){ return y; }
int Process::getLength(){ return length; }
int Process::getStartLength(){ return startlength; }
int Process::getHeight(){ return height; }
QString Process::getName(){ return name; }
int Process::getNumber(){ return number; }
QColor Process::getColor(){ return color; }

void Process::setX(int ax){ x = ax; }
void Process::setY(int ay){ y = ay; }
void Process::setLength(int len){ length = len; }
void Process::setPainter(QPainter *paint){ painter = paint; }
void Process::setNumber(int num){ number = num; }
void Process::setColor(QColor col){ color = col; }
void Process::setStartLength(int len){ startlength = len; }

void Area::restart(){
    processes.clear();
}
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
    painter->drawText(QRect(originX, originY - heightY - 20, lengthX, 20),"Очередь",static_cast<QTextOption>(Qt::AlignCenter));
    painter->fillRect(originX, originY, lengthX, 40, QBrush(QColor(255,255,255)));//Закрасить белым над областью, на месте лейбла
    painter->drawRect(originX, originY - heightY, lengthX, heightY); //Рамка области
}
void Queue::draw(){
    int y = originY - 35;
    int heightOfAll = 0;
    for(auto process : processes){
        heightOfAll += stepY;
    }
    int space = heightOfAll - heightY;
    int margin = slider*space/100;
    for(auto &process : processes){
        if(y > (originY - heightY - margin - process.getHeight()) && y < originY - margin){
            process.setPainter(painter);
            process.setX(originX + (lengthX - process.getLength())/2);
            process.setY(y + margin);
            process.draw();
        }
         y -= stepY;
    }
    drawLabel();
}
bool Queue::greater(Process pr1, Process pr2){//!Операция сравнения в зависимости от алгоритма
    if(sorting == "Первым пришёл - первым обслужен" || sorting == "Циклическое планирование")
        return pr1.getNumber() > pr2.getNumber(); //Сравниваем порядковый номер процессов(pr1 пришёл позже pr2?)
    if(sorting == "Краткосрочное планирование" || sorting == "Наименьшее оставшееся время")
        return pr1.getLength() > pr2.getLength(); //Сравниваем время для обработки(размер процессов)
    return pr1.getNumber() > pr2.getNumber();  //По умолчанию используем сравнение порядкового номера
}
void Queue::sort(){//!Сортировка процессов в очереди
    for(unsigned long long i = 1; i < processes.size(); i++){
            for(unsigned long long j = i; j > 0 && greater(processes[j-1], processes[j]); j--){
                Process tmp = processes[j-1];
                processes[j-1] = processes[j];
                processes[j] = tmp;
            }
    }
}
void Queue::addProcess(Process proc){//!Добавление процесса в очередь
    lastNumber++;   //Увеличиваем номер последнего процесса в очереди
    proc.setNumber(lastNumber); //Присвоение номера полученному процессу
    processes.push_back(proc);  //Добавляем процесс в массив всех процессов очереди
    sort();                     //И сортируем массив
}
Process Queue::findProcess(){//!Поиск первого процесса в очереди и его отправка на выполнение процессором
    Process proc = processes[0];    //Запоминаем данные первого процесса
    processes.erase(processes.begin()); //Удаляем его из массива всех процессов очереди
    return proc;    //Отдаём его процессору
}
Process Queue::firstProcess(){  //!Просмотр первого процесса в очереди
    return processes[0];        //Возвращает процесс с наибольшим приоритетом для анализа процессором
}
bool Queue::empty(){ //!Очередь пуста?
    return (processes.size() == 0);
}

int Queue::getStepY(){ return stepY; }
void Queue::setStepY(int step){ stepY = step; }
void Queue::setSlider(int sl){ slider = sl; }
void Queue::setSorting(QString sort){ sorting = sort; }


ExecutionBar::ExecutionBar(Queue *q, int aoriginX, int aoriginY,
             int alengthX, int aheightY){
    originX = aoriginX; originY = aoriginY;
    lengthX = alengthX; heightY = aheightY;
    queue = q;
}
ExecutionBar::~ExecutionBar(){}
void ExecutionBar::restart(){
    processes.clear();
    idletime = 0;
    usetime = 0;
    delaytime = 0;
    endX = 0;
}
void ExecutionBar::drawLabel(){
    int percent = usetime!=0 ? delaytime*100/(delaytime + usetime) : 0;
    painter->drawText(QRect(originX, originY - heightY - 30, lengthX, 30),"Выполняемые процессы",static_cast<QTextOption>(Qt::AlignHCenter));
    painter->drawText(QRect(originX, originY - heightY - 30, lengthX, 30),
                      "Время всех переключений: " + QString::number(delaytime) + " (" +QString::number(percent) + "%) ",
                      static_cast<QTextOption>(Qt::AlignLeft));
    painter->drawText(QRect(originX + 250, originY - heightY - 30, lengthX - 100, 30),
                      "Время работы процессора: " + QString::number(usetime),static_cast<QTextOption>(Qt::AlignLeft));
    painter->drawText(QRect(originX, originY - heightY - 30, lengthX, 30),
                      "Время ожидания процессора: " + QString::number(idletime),static_cast<QTextOption>(Qt::AlignRight));

    //painter->drawRect(originX, originY - heightY, lengthX, heightY);
}
void ExecutionBar::draw(){
    drawLabel();
    int margin = slider*endX/100;
    for(auto process: processes){
        process.setX(process.getX() - margin);
        if(process.getX() + process.getLength() > originX - 20 && process.getX() < originX + lengthX){
            process.setPainter(painter);
            process.drawShadow();
            process.draw();
        }
        process.setX(process.getX() + margin);
    }
}
bool ExecutionBar::enough(){        //!Достаточно ли работать с текущим процессом?
    auto last = processes.size()-1; //Записываем индекс текущего(последнего) процесса
    if(processes[last].getLength() >= processes[last].getStartLength()) //Если обработан весь процесс
        return true;                                                    //То работа с процессом завершена
    if(alg == "Циклическое планирование"){              //Если выбрано циклическое планирование
        return processes[last].getLength() >= quantum;  //И процесс уже отработал свой квант времени, то достаточно
    }
    if(alg == "Наименьшее оставшееся время" && !queue->empty()){    //Если выбрано "Наименьшее оставшееся время" и очередь не пуста
        int curlen = processes[last].getStartLength() - processes[last].getLength();//Считаем оставшееся время текущего процесса
        return (queue->firstProcess().getLength() < curlen);//Достаточно, если первый процесс в очереди короче оставшегося текущего
    }
    return false;   //Иначе, если процесс ещё не завершён и нет повод для его прерывания, то продолжаем работу с ним
}
void ExecutionBar::update(int dx){  //!Обновить область после шага работы процессора
    draw(); //Отрисовать область для отображения работы процессора
    if(!waiting && static_cast<int>(processes.size())>0){   //Если процессор обрабатывает процесс
        auto last = processes.size()-1; //Записываем индекс текущего(последнего) процесса
        processes[last].setLength(processes[last].getLength()+dx);  //Увеличиваем время работы с процессом(его размер отрисовки)
        if(enough()){//Если с процессом нужно закончить работу
            endX += processes[last].getLength();//Записываем, где кончился процесс, чтобы оттуда рисовать следующий
            //Если выбран один из этих алгоритмов и процесс ещё не закончен, то вызвано его прерывание и нужно отправить его в очередь
            if((alg == "Циклическое планирование" || alg == "Наименьшее оставшееся время") &&
                    processes[last].getLength() < processes[last].getStartLength()){
                Process proc = processes[last]; //Копируем недообработанный процесс
                proc.setLength(proc.getStartLength()-proc.getLength()); //Считаем, сколько осталось обработать
                proc.setStartLength(proc.getLength());      //Удаляем длину уже обработанной части
                queue->addProcess(proc);                    //Отправляем в очередь оставшуюся часть процесса
                processes[last].setStartLength(processes[last].getLength());//В панели выполненных оставляем уже обработанную часть
            }
            requestForNext();//Делаем запрос в очередь на обработку следующего процесса
        }
        usetime++;//Увеличиваем время работы процессора
    }
    else{   //Если процессор ожидает процесса
        requestForNext();   //то делаем запрос на следующий процесс в очередь
    }
}
void ExecutionBar::requestForNext(){
    if(!queue->empty()){
        if(remainingDelay<=0){
            clear();
            Process nextPr = queue->findProcess();
            nextPr.setX(originX + endX);
            nextPr.setY(originY - heightY);
            nextPr.setLength(0);
            processes.push_back(nextPr);
            waiting = false;
            remainingDelay = delay;
        }else{
            remainingDelay--;
            delaytime++;
            waiting = true;
        }
    }else{
        idletime++;
        waiting = true;
        //if(processes.size()>0) Пропуски в отсутствие процессов
          //  endX += 1;
    }
}
void ExecutionBar::clear(){
    if(processes.size()>99){
        endX = 0;
        for(int i = 99; i > -1; i--){
            processes.erase(processes.begin() + i);
        }
    }
}
void ExecutionBar::setSlider(int sl){ slider = sl; }
void ExecutionBar::setAlg(QString a){ alg = a; }
void ExecutionBar::setQuantum(int q){ quantum = q; }
void ExecutionBar::setDelay(int d){ delay = d; }

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
        if(process.getX() + process.getLength() > originX + lengthX)
            process.drawShadow();
    }
}

void Graph::sendToQueue(){//Удалить параметр
    queue->addProcess(processes[0]);
    processes.erase(processes.begin());
}

void Graph::update(int dx){
    drawAxis();
    if(processes.size() > 0 && processes[0].getX()<originX - dx + 1){
        sendToQueue();//удалить параметр функции
    }
    for(auto &process: processes){
        process.setPainter(painter);
        process.move(dx,0);
        if(process.getX() + process.getLength() > originX + lengthX)
            process.drawShadow();
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
             processes.push_back(Process(name, originX + lengthX, y, len, height, col));
             processes[processes.size()-1].setPainter(painter);
             processes[processes.size()-1].drawShadow();
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
    queue = new Queue();
    queue->setSlider(ui->queueSlider->value());
    queue->setSorting(ui->algorithmBox->currentText());
    graph = new Graph(queue);
    bar = new ExecutionBar(queue);
    bar->setAlg(ui->algorithmBox->currentText());
    bar->setSlider(ui->barSlider->value());
    bar->setQuantum(ui->quantSpin->value());
    bar->setDelay(ui->delaySpin->value());
    //proc = new Process("П1", 1000, 120, 100);
    //update();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerActivation()));
    //timer->start(20);
}

void MainWindow::paintEvent(QPaintEvent *event){
    painter = new QPainter(this);
    bar->setPainter(painter);
    queue->setPainter(painter);
    graph->setPainter(painter);
    int xzero = 11;
    int yzero = 45;
    int width = 1208;
    int height = 520;
    painter->fillRect(xzero,yzero,width,height, QBrush(QColor(255,255,255)));
    painter->setPen(Qt::black);
    if(timeFlag){
        queue->draw();
        bar->update(1);
         graph->update(-1);
         if(rand()%200 + 200 - ui->frequencySlider->value() < 200){
             count++;
            graph->createProcess("П" + QString::number(count),rand()%170 + 25, 25, QColor(rand()%156+100,rand()%156+100,rand()%156+100,120));
         }
    }
    else{
        queue->draw();
        bar->draw();
        graph->draw();
    }
    painter->fillRect(0,0,xzero,height+yzero, QBrush(QColor("#f0f0f0")));
    painter->fillRect(xzero + width,0,xzero*5,height+yzero, QBrush(QColor("#f0f0f0")));
    painter->fillRect(0,0,xzero*2 + width,yzero, QBrush(QColor("#f0f0f0")));
    painter->end();
}

void MainWindow::timerActivation()
{
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
    queue->setSorting(arg1);
    queue->sort();
    bar->setAlg(arg1);
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    timer->setInterval(value);
}

void MainWindow::on_startButton_clicked()
{
    if(timer->isActive()){
        timer->stop();
        ui->startButton->setText("Старт");
    }
    else {
        timer->start(ui->speedSlider->value());
        ui->startButton->setText("Стоп");
    }
}

void MainWindow::on_barSlider_sliderMoved(int position)
{
    bar->setSlider(position);
    this->update();
}

void MainWindow::on_queueSlider_valueChanged(int value)
{
    queue->setSlider(value);
    this->update();
}

void MainWindow::on_quantSpin_valueChanged(int arg1)
{
    bar->setQuantum(arg1);
}

void MainWindow::on_delaySpin_valueChanged(int arg1)
{
    bar->setDelay(arg1);
}

void MainWindow::on_restartButton_clicked()
{
    graph->restart();
    queue->restart();
    bar->restart();
    count = 0;
}
