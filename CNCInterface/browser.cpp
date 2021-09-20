#include "browser.h"
#include "ui_browser.h"
#include <mainwindow.h>
QString dir;

Browser::Browser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Browser)
{
    ui->setupUi(this);
    this->setWindowTitle("ExploradorDeArchivos");

    QString dir="/";
    directorio = new QFileSystemModel(this);
    directorio->setFilter(QDir::NoDotAndDotDot| QDir::AllEntries);
    directorio->setRootPath(dir);
    ui->treeView->setModel(directorio);

/*
    archivo = new QFileSystemModel(this);
    archivo->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    archivo->setRootPath(dir);

    ui->listView->setModel(archivo);
    */

}

Browser::~Browser()
{
    delete ui;
}

void Browser::on_treeView_clicked(const QModelIndex &index)
{
     dir= directorio->fileInfo(index).absoluteFilePath();
     //ui->listView->setRootIndex(archivo->setRootPath(dir));
     ui->lineEdit->setText(dir);
}

void Browser::on_Cerrrar_clicked()
{
    this->close();
}

void Browser::on_Aceptar_clicked()
{
    emit dlgReturn(dir);
    //qDebug()<<dir;
    this->close();
}
