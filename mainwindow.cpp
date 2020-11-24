#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QColor>

#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    update();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerActivation()));
    timer->start(50);
    ui->axisXLabel->setGeometry(31, 75, 250, 100);

}

void MainWindow::paintEvent(QPaintEvent *event){
    int xzero = 11;
    int yzero = 55;
    int width = 1208;
    int height = 635;
    QPainter painter(this);
    painter.fillRect(xzero,yzero,width,height, QBrush(QColor(255,255,255)));
    painter.setPen(Qt::black);

    painter.fillRect(xzero+20,yzero+20,100,25, QBrush(QColor(0,180,0,120)));
    painter.drawRect(xzero+20,yzero+20,100,25);
   // painter.drawRect(xzero+19,yzero+19,102,22);

    QWidget::paintEvent(event);
}

void MainWindow::timerActivation()
{
    /* Ежесекундно обновляем данные по текущему времени
     * Перезапускать таймер не требуется
     * */
    ui->labelSpeed->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->axisXLabel->move(ui->axisXLabel->x()+1,ui->axisXLabel->y());
}

MainWindow::~MainWindow()
{
    delete ui;
}
