#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include <QTextStream>
#include <QFile>
#include <QString>
#include <qdesktopservices.h>
#include <QUrl>


void MainWindow::buscaYconectaArduino(){
    //Parte # 1, declaración inicial de las variables
    arduino = new QSerialPort(this);
    arduino_esta_disponible = false;
    arduino_puerto = "";

    QString nombreDispositivoSerial = "";
    int nombreProductID = 0;

    //Parte # 2,buscar puertos con los identificadores de Arduino
    qDebug() << "Puertos disponibles: " << QSerialPortInfo::availablePorts().length();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        qDebug() << "Identificador del fabricante (VENDOR ID): " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "ID Vendedor " << serialPortInfo.vendorIdentifier();
            qDebug() << "ID Producto: " << serialPortInfo.productIdentifier();

            if(serialPortInfo.productIdentifier() == arduino_UNO || serialPortInfo.productIdentifier() == arduino_MEGA){
                arduino_esta_disponible = true;
                nombreDispositivoSerial = serialPortInfo.portName();
                nombreProductID = serialPortInfo.productIdentifier();
            }
        }

    }
    if(arduino_esta_disponible){
        ui->ConectCOM->setText(nombreDispositivoSerial);
    }else{
        ui->ConectCOM->setText("COM");

    }


    //Parte # 3, conexión del puerto encontrado con Arduino
    if(arduino_esta_disponible && Start && ui->ConectCOM->text()!="COM"){
        qDebug() << "Producto: " << nombreProductID;
        arduino_puerto = nombreDispositivoSerial;
        arduino ->setPortName(arduino_puerto);
        arduino->setDataBits(QSerialPort::Data8);
        arduino ->setBaudRate(QSerialPort::Baud115200);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->open(QIODevice::ReadWrite);
        m_timer = new QTimer(this);
        connect(arduino,SIGNAL(readyRead()),this,SLOT(SerialReceived()));
        m_timer->start(300);
        ui->StatusInform->clear();


        //if(nombreProductID == 29987) ui->label->setText("Arduino UNO R3 conectado");
        //else if(nombreProductID == 68) ui->label->setText("Arduino Mega conectado");
        //else ui->label->setText("Error 3")
        enable();
        if(RL_CONFIG){
            ui->upZ->setEnabled(0);
            ui->HOMEZ->setEnabled(0);
            ui->LOWZ->setEnabled(0);
        }else{
            ui->upZ->setEnabled(1);
            ui->HOMEZ->setEnabled(1);
            ui->LOWZ->setEnabled(1);
        }
    }else{
        disable();
        ui->listWidget->clear();
    }
}

QByteArray Data_entry;
bool recieved;
void MainWindow::SerialReceived(){
    QByteArray ba;
    ba = arduino->readAll();
    //QByteArray to QString
    //QString DataAsString = QString::fromStdString(ba.toStdString());
    if(ba.lastIndexOf("\r\nGrbl 1.1h ['$' for help]\r\n")==-1){
        if(ba.lastIndexOf("\r\nok\r\n")!=-1){
            Data_entry.append(ba);
            Data_entry.replace(QByteArray("ok\r\n"), QByteArray(""));
            Data_entry.replace(QByteArray("\r\nok"), QByteArray(""));
            Data_entry.replace(QByteArray("\r\nok\r\n"), QByteArray(""));
            Data_entry=Data_entry.trimmed();
            //ui->listWidget->addItem(Data_entry);
            //qDebug()<<Data_entry;
            recieved=true;
        }else{
            Data_entry.append(ba);
        }
    }
    //qDebug()<<ba;
    if(recieved){
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<Actual_Position>>>>>>>>>>>>>>>>>>>>>>>*/

        if(Data_entry.lastIndexOf("MPos:")!=-1){
            QByteArray xa;
            QByteArray ya;
            QByteArray za;
            //buscar en la libreria forma de solo extraer un parte del arreglo
            for(int x=Data_entry.indexOf(":",1)+1;x<=Data_entry.indexOf(",",1)-1;x++){
                xa.append(Data_entry[x]);
            }
            for(int x=Data_entry.indexOf(",",1)+1;x<=Data_entry.indexOf(",",2)-1;x++){
                ya.append(Data_entry[x]);
            }
            for(int x=Data_entry.indexOf(",",2)+1;x<=Data_entry.indexOf("|",1)-1;x++){
                za.append(Data_entry[x]);
            }
            //actual_x=Data_entry.mid(Data_entry.indexOf(":",1)+1,Data_entry.indexOf(",",1)-1).toDouble();
            actual_x=xa.toDouble();
            actual_y=ya.toDouble();
            actual_z=za.toDouble();
            ui->lcdNumber_5->display(actual_x);
            ui->lcdNumber_6->display(actual_y);
            ui->lcdNumber_2->display(actual_z);
            if(actual_x == job_x && actual_y == job_y && actual_z == job_z ){
                qDebug()<<"para";
            }else{

                arduino->write("?\n");
            }

        }
    /*<<<<<<<<<<<< <<<<<<<<<<<<<<<<<clear_Variables>>>>>>>>>>>>>>>>>>>>>>>*/
        Data_entry.clear();
        recieved=false;
    }
}

