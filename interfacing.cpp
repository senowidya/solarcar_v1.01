#include "interfacing.h"
#include "ui_interfacing.h"


interfacing::interfacing(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::interfacing)
{
    ui->setupUi(this);
    ui->txt_Terminal->installEventFilter(this);
    obj_port= new QSerialPort(this);
    connect(obj_port,SIGNAL(readyRead()),SLOT(readData()));
    obj_timer= new QTimer(this);

    vcount=0;
    ui->cmb_Baud->setCurrentIndex(4);
    ui->cmb_Port->setCurrentText("pilih");



}

interfacing::~interfacing()
{
    delete ui;
}

void interfacing::on_actionQuit_triggered()
{
    if(obj_port->isOpen()){obj_port->close();}
    QApplication::quit();
}

void interfacing::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}

QVector<double> datax(1000),datay1(1000),datay2(1000),datay3(1000);

void interfacing::readData(){
    QByteArray rawdata=obj_port->readAll();
    ui->txt_Terminal->insertPlainText(rawdata);

    QString indata=ui->txt_Terminal->toPlainText();
    QStringList lsindata= indata.split("\n");
    uint cdata= lsindata.count();

    if(cdata<=2){return;}

    QStringList lsdata = lsindata[cdata-2].split(",");

    uint vdata1=lsdata[0].toInt();
    uint vdata2=lsdata[1].toInt();
    uint vdata3=lsdata[2].toInt();

    datax[vcount]=vcount;
    datay1[vcount]=vdata1;
    datay2[vcount]=vdata2;
    datay3[vcount]=vdata3;
    vcount++;

    ui->wgt_GraphPlot->addGraph();
    ui->wgt_GraphPlot->graph()->setData(datax,datay1);
    ui->wgt_GraphPlot->graph()->setPen(QPen(Qt::red));

    ui->wgt_GraphPlot->addGraph();
    ui->wgt_GraphPlot->graph()->setData(datax,datay2);
    ui->wgt_GraphPlot->graph()->setPen(QPen(Qt::yellow));

    ui->wgt_GraphPlot->addGraph();
    ui->wgt_GraphPlot->graph()->setData(datax,datay3);
    ui->wgt_GraphPlot->graph()->setPen(QPen(Qt::black));

    ui->wgt_GraphPlot->yAxis->setRange(0,4100);
    ui->wgt_GraphPlot->xAxis->setRange(0,1500);
    ui->wgt_GraphPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->wgt_GraphPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->wgt_GraphPlot->replot();
}

void interfacing::on_btn_Open_clicked()
{
    QString dev_name="/dev/";

    dev_name += ui->cmb_Port->currentText();

    QSerialPort::BaudRate dev_baud;
    QSerialPort::DataBits dev_DataBits;
    QSerialPort::StopBits dev_StopBits;
    QSerialPort::FlowControl dev_FlowControl;
    QSerialPort::Parity dev_Parity;

    int baud_index=ui->cmb_Baud->currentIndex();
    int DataBits_index= ui->cmb_DataBits->currentIndex();
    int StopBits_index= ui->cmb_StopBits->currentIndex();
    int FlowControl_index= ui->cmb_FlowControl->currentIndex();
    int Parity_index= ui->cmb_Parity->currentIndex();

    switch(baud_index){
    case 0: dev_baud=QSerialPort::Baud9600;break;
    case 1: dev_baud=QSerialPort::Baud38400;break;
    case 2: dev_baud=QSerialPort::Baud57600;break;
    case 3: dev_baud=QSerialPort::Baud115200;break;
    }
    switch(DataBits_index){
    case 0: dev_DataBits=QSerialPort::Data5;break;
    case 1: dev_DataBits=QSerialPort::Data6;break;
    case 2: dev_DataBits=QSerialPort::Data7;break;
    case 3: dev_DataBits=QSerialPort::Data8;break;
    }
    switch(StopBits_index){
    case 0: dev_StopBits=QSerialPort::OneStop;break;
    case 1: dev_StopBits=QSerialPort::OneAndHalfStop;break;
    case 2: dev_StopBits=QSerialPort::TwoStop;break;
    }
    switch (FlowControl_index) {
    case 0: dev_FlowControl=QSerialPort::NoFlowControl;break;
    case 1: dev_FlowControl=QSerialPort::HardwareControl;break;
    case 2: dev_FlowControl=QSerialPort::SoftwareControl;break;
    }
    switch(Parity_index){
    case 0: dev_Parity=QSerialPort::NoParity;break;
    case 1: dev_Parity=QSerialPort::EvenParity;break;
    case 2: dev_Parity=QSerialPort::OddParity;break;
    case 3: dev_Parity=QSerialPort::SpaceParity;break;
    case 4: dev_Parity=QSerialPort::MarkParity;break;
    }


    if(ui->btn_Open->text()=="Open"){

       obj_port->setPortName(dev_name);

       if(obj_port->open(QIODevice::ReadWrite)){
//           obj_port->setBaudRate(dev_baud,QSerialPort::AllDirections);
           obj_port->setBaudRate(dev_baud);
           obj_port->setDataBits(dev_DataBits);
           obj_port->setStopBits(dev_StopBits);
           obj_port->setFlowControl(dev_FlowControl);
           obj_port->setParity(dev_Parity);
           QMessageBox::information(this,"success","port success on "+dev_name );
           ui->btn_Open->setText("Close");
           ui->txt_portname->setText(dev_name);
           ui->cmb_Baud->setEnabled(false);
                }
       else{
       QMessageBox::critical(this,"Failed","port failed on "+dev_name);

       ui->btn_Open->setText("Open");
       ui->txt_portname->clear();

       }

           }else{if(obj_port->isOpen()){obj_port->close();
               }
               ui->btn_Open->setText("Open");
               ui->txt_portname->clear();
               ui->cmb_Baud->setEnabled(true);
               ui->cmb_Baud->setCurrentIndex(4);
               ui->cmb_Port->setCurrentText("pilih");
           }
    }

bool interfacing::eventFilter(QObject *obj, QEvent *event){
    if(event->type()== QEvent::KeyPress){
        QKeyEvent* keyEvent = static_cast<QKeyEvent* >(event);
        if(obj_port->isOpen()){
            QString s = keyEvent->text();
            if(s.length()){
                char ch = s.at(0).toLatin1();
                QByteArray chb= QByteArray::fromRawData(&ch,1);
                obj_port->write(chb);

                if(ui->chk_LocalEcho->isChecked()){
                    ui->txt_Terminal->insertPlainText(chb);
                }
            }
        }
        return true;
    }
    else{
        return QObject::eventFilter(obj, event);
    }
}

void interfacing::on_btn_Clear_clicked()
{
    ui->txt_Terminal->clear();
}



void interfacing::on_pushButton_clicked()
{
    int i;
    for(i=0;i<1000;i++){
        datax[i]=0;
        datay1[i]=0;
        datay2[i]=0;
        datay3[i]=0;
    }
    vcount=0;

    if(obj_timer->isActive()){
        obj_timer->stop();
    }

    ui->wgt_GraphPlot->clearGraphs();

    ui->wgt_GraphPlot->replot();


}
