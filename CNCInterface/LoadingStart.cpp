#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::loadingValues(){
    ui->progressBar->setValue(0);

    if (!connopen())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        //qDebug() << "Database: connection ok";
    }
    createTable();
    BaudiosWork = Extraer("BaudiosWork");
    switch (BaudiosWork) {
    case 4800: ui->comboBox->setCurrentIndex(0);
    break;
    case 9600: ui->comboBox->setCurrentIndex(1);
    break;
    case 19200: ui->comboBox->setCurrentIndex(2);
    break;
    case 38400: ui->comboBox->setCurrentIndex(3);
    break;
    case 57600: ui->comboBox->setCurrentIndex(4);
    break;
    case 115200: ui->comboBox->setCurrentIndex(5);
    break;
    case 230400: ui->comboBox->setCurrentIndex(6);
    break;
    case 460800: ui->comboBox->setCurrentIndex(7);
    break;
    case 921600: ui->comboBox->setCurrentIndex(8);
    break;
    }

    ui->lineEdit->setPlaceholderText("Introduce el codigo y da enter");
    RL_CONFIG=Extraer("RL_CONFIG");
    if(RL_CONFIG){
        ui->changeLASER->setChecked(1);
        ui->upZ->setEnabled(0);
        ui->HOMEZ->setEnabled(0);
        ui->LOWZ->setEnabled(0);
    }else{
        ui->changeLASER->setChecked(0);
        ui->upZ->setEnabled(1);
        ui->HOMEZ->setEnabled(1);
        ui->LOWZ->setEnabled(1);
    }

    //Memoria de Sliders

    manual_Distance_Aux=Extraer("Manual_Distance");
    ui->verticalSlider_2->setValue(manual_Distance_Aux);
    switch(manual_Distance_Aux){
    case 0:
           manual_Distance="0.1";
    break;
    case 1:
           manual_Distance="0.2";
    break;
    case 2:
           manual_Distance="0.5";
    break;
    case 3:
           manual_Distance="1";
    break;
    case 4:
           manual_Distance="2";
    break;
    case 5:
           manual_Distance="5";
    break;
    case 6:
           manual_Distance="10";
    break;
    case 7:
           manual_Distance="20";
    break;
    case 8:
           manual_Distance="50";
    break;
    case 9:
           manual_Distance="100";
    break;
    case 10:
           manual_Distance="200";
    break;
    }
    ui->label_13->setText(manual_Distance+"mm");

    manual_Speed_Aux=Extraer("Manual_Speed");
    manual_Speed.setNum( manual_Speed_Aux);
    ui->label_12->setText(manual_Speed+"mm/min");
    ui->verticalSlider->setValue( manual_Speed_Aux);

}
