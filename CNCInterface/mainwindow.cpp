#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "browser.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QMessageBox>
#include <QString>
#include <QTime>

// Change to any path you wish



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ProyectoMecatronico");

    // Creación de base de datos apertura/////////////////////////////////
    Start=false;
    Start_data=false;
    ui->Conectar->setStyleSheet("border-image: url(:/Iconos/off.png)");
    loadingValues();
    buscaYconectaArduino();

    ventana = new Browser(this);
    connect(ventana,SIGNAL(dlgReturn(QString)),this,SLOT(showValue(QString)));
    connClose();
}

MainWindow::~MainWindow()
{
    connopen();
    registera("BaudiosWork",BaudiosWork);
    registera("RL_CONFIG",RL_CONFIG);
    registera("Manual_Distance",manual_Distance_Aux);
    registera("Manual_Speed",manual_Speed_Aux);

    if(arduino->isOpen()){
        arduino->close();
    }
    connClose();
    delete ui;
}

void MainWindow::showValue(QString direccion){
    ui->Direccion->setText(direccion);
    //qDebug()<<direccion;
}

void MainWindow::on_comboBox_activated(int index)
{
    switch (index) {
        case 0: BaudiosWork=4800;
        break;
        case 1: BaudiosWork=9600;
        break;
        case 2: BaudiosWork=19200;
        break;
        case 3: BaudiosWork=38400;
        break;
        case 4: BaudiosWork=57600;
        break;
        case 5: BaudiosWork=115200;
        break;
        case 6: BaudiosWork=230400;
        break;
        case 7: BaudiosWork=460800;
        break;
        case 8: BaudiosWork=921600;
        break;
    }
    //connopen();

    //connClose();
}

void MainWindow::on_actionInfo_triggered()
{
    Info *ventana = new Info(this);
    ventana->setModal(true);
    ventana->show();

}

void MainWindow::on_Folder_clicked()
{
    ventana->setModal(true);
    ventana->show();
}



void MainWindow::on_lineEdit_returnPressed()
{

    QString temp=ui->lineEdit->text();
    ui->listWidget->addItem(temp);
    char *str=(char *)malloc(10);
    QByteArray ba=temp.toLatin1();
    strcpy(str,ba.data());

    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write(ba+"\n");
    }

}


void MainWindow::on_ConectCOM_clicked()
{
    buscaYconectaArduino();
}


void MainWindow::on_Conectar_clicked()
{
    if(ui->ConectCOM->text()!="COM"){
        if(Start==false){
            Start=true;
            buscaYconectaArduino();
            ui->ConectCOM->setEnabled(0);
            ui->comboBox->setEnabled(0);
            ui->Conectar->setStyleSheet("border-image: url(:/Iconos/on.png)");

        }else{
            Start=false;

            if(arduino->isOpen()){
                arduino->close();
            }
            buscaYconectaArduino();
            ui->ConectCOM->setEnabled(1);
            ui->comboBox->setEnabled(1);
            ui->Conectar->setStyleSheet("border-image: url(:/Iconos/off.png)");

            Start_data=false;
            ui->StartSend->setStyleSheet("border-image: url(:/Iconos/off.png)");
            job_x=0;
            job_y=0;
            job_z=0;
            job_c=0;
            ui->lcdNumber->display(job_x);
            ui->lcdNumber_3->display(job_y);
            ui->lcdNumber_8->display(job_z);
            ui->lcdNumber_7->display(job_c);
        }
    }
}


void MainWindow::on_changeROUTER_clicked()
{
    //connopen();
    //registera("RL_CONFIG",false);
    //connClose();
    RL_CONFIG=false;
    ui->upZ->setEnabled(1);
    ui->HOMEZ->setEnabled(1);
    ui->LOWZ->setEnabled(1);
}

void MainWindow::on_changeLASER_clicked()
{
    //connopen();
    //registera("RL_CONFIG",true);
    //connClose();
    RL_CONFIG=true;
    qDebug()<<"entro";
    ui->upZ->setDisabled(1);
    ui->HOMEZ->setEnabled(0);
    ui->LOWZ->setEnabled(0);
}

void MainWindow::on_StartSend_clicked()
{
    if(Start){
        if(Start_data){
            Start_data=false;
            ui->StartSend->setStyleSheet("border-image: url(:/Iconos/off.png)");
        }else{
            Start_data=true;
            ui->StartSend->setStyleSheet("border-image: url(:/Iconos/on.png)");
        }
    }
}

void MainWindow::on_verticalSlider_2_valueChanged(int value)
{
    //connopen();
    switch(value){
    case 0://registera("Manual_Distance",0);
           manual_Distance="0.1";
    break;
    case 1://registera("Manual_Distance",1);
           manual_Distance="0.2";
    break;
    case 2://registera("Manual_Distance",2);
           manual_Distance="0.5";
    break;
    case 3://registera("Manual_Distance",3);
           manual_Distance="1";
    break;
    case 4://registera("Manual_Distace",4);
           manual_Distance="2";
    break;
    case 5://registera("Manual_Distance",5);
           manual_Distance="5";
    break;
    case 6://registera("Manual_Distance",6);
           manual_Distance="10";
    break;
    case 7://registera("Manual_Distance",7);
           manual_Distance="20";
    break;
    case 8://registera("Manual_Distance",8);
           manual_Distance="50";
    break;
    case 9://registera("Manual_Distance",9);
           manual_Distance="100";
    break;
    case 10://registera("Manual_Distance",10);
           manual_Distance="200";
    break;
    }
    manual_Distance_Aux=value;
    ui->label_13->setText(manual_Distance+"mm");
    //connClose();
}

void MainWindow::on_upLeft_9_clicked()
{
    //home
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G90X0Y0\n");
            job_x=0;
            job_y=0;
            job_c=0;
            ui->lcdNumber->display(job_x);
            ui->lcdNumber_3->display(job_y);
            ui->lcdNumber_7->display(job_c);
    }
}

void MainWindow::on_upLeft_2_clicked()
{
    //izquierda
    arduino->write("$J=G91X-"+manual_Distance+"F"+manual_Speed+"\n");
    job_x-=manual_Distance.toDouble();
    ui->lcdNumber->display(job_x);
    arduino->write("?\n");
}


void MainWindow::on_upLeft_10_clicked()
{
    //derecha
    arduino->write("$J=G91X+"+manual_Distance+"F"+manual_Speed+"\n");
    job_x+=manual_Distance.toDouble();
    ui->lcdNumber->display(job_x);
    arduino->write("?\n");

}
void MainWindow::on_upLeft_3_clicked()
{
    //arriba
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G91Y+"+manual_Distance+"F"+manual_Speed+"\n");
            job_y+=manual_Distance.toInt();
            ui->lcdNumber_3->display(job_y);
    }
}

void MainWindow::on_upLeft_4_clicked()
{
    //abajo
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G91Y-"+manual_Distance+"F"+manual_Speed+"\n");
            job_y-=manual_Distance.toInt();
            ui->lcdNumber_3->display(job_y);
    }
}

void MainWindow::on_upLeft_6_clicked()
{
    //superior_izquierda
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G91X-"+manual_Distance+"Y+"+manual_Distance+"F"+manual_Speed+"\n");
            job_x-=manual_Distance.toInt();
            ui->lcdNumber->display(job_x);
            job_y+=manual_Distance.toInt();
            ui->lcdNumber_3->display(job_y);
    }
}

void MainWindow::on_upLeft_11_clicked()
{
    //superior_derecha
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G91X+"+manual_Distance+"Y+"+manual_Distance+"F"+manual_Speed+"\n");
            job_x+=manual_Distance.toInt();
            ui->lcdNumber->display(job_x);
            job_y+=manual_Distance.toInt();
            ui->lcdNumber_3->display(job_y);
    }
}

void MainWindow::on_upLeft_7_clicked()
{
    //inferior_izquierda
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G91X-"+manual_Distance+"Y-"+manual_Distance+"F"+manual_Speed+"\n");
            job_x-=manual_Distance.toInt();
            ui->lcdNumber->display(job_x);
            job_y-=manual_Distance.toInt();
            ui->lcdNumber_3->display(job_y);
    }
}

void MainWindow::on_upLeft_8_clicked()
{
    //inferior_derecha
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G91X+"+manual_Distance+"Y-"+manual_Distance+"F"+manual_Speed+"\n");
            job_x+=manual_Distance.toInt();
            ui->lcdNumber->display(job_x);
            job_y-=manual_Distance.toInt();
            ui->lcdNumber_3->display(job_y);
    }
}

void MainWindow::on_upZ_clicked()
{
    //arriba_z
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G91Z+"+manual_Distance+"F"+manual_Speed+"\n");
            job_z+=manual_Distance.toInt();
            ui->lcdNumber_8->display(job_z);
    }
}

void MainWindow::on_LOWZ_clicked()
{
    //abajo_z
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G91Z-"+manual_Distance+"F"+manual_Speed+"\n");
            job_z-=manual_Distance.toInt();
            ui->lcdNumber_8->display(job_z);
    }
}

void MainWindow::on_HOMEZ_clicked()
{
    //home_z
    if(arduino_esta_disponible && arduino->isWritable()){
            arduino->write("$J=G90Z0\n");
            job_z=0;
            ui->lcdNumber_8->display(job_z);
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{
    manual_Speed_Aux=value;
    manual_Speed.setNum(value);
    ui->label_12->setText(manual_Speed+"mm/min");
    //connopen();
    //registera("Manual_Speed",value);
    //connClose();
}

