#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::UpdateData(const QString& name,const QString& valor){
    QSqlQuery updateDa;
    updateDa.prepare("update data set NameData='"+name+"',Datos='"+valor+"' WHERE  NameData='"+name+"'");
    updateDa.exec();

}
bool MainWindow::dataExist(const QString& name) const
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT NameData FROM Data WHERE NameData = (:NameData)");
    checkQuery.bindValue(":NameData", name);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "person exists failed: " << checkQuery.lastError();
    }

    return exists;
}

bool MainWindow::removeAllData()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM Data");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all persons failed: " << removeQuery.lastError();
    }

    return success;
}

QString MainWindow::Change(int valor)
{
    QString conversion;
    conversion=QString::fromStdString(std::to_string(valor));
    return conversion;
}

bool MainWindow::createTable()
{
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE Data(NameData,Datos);");

    if (!query.exec())
    {
        //qDebug() << "Couldn't create the table 'people': one might already exist.";
        success = false;
    }

    return success;
}

void MainWindow::addData(const QString& name,const QString& valor)
{
      QSqlQuery query;
      query.prepare("insert into Data (NameData,Datos) values('"+name+"','"+valor+"')");
            query.exec();

}

void MainWindow::printAllData() const
{

    qDebug() << "Data:";
    QSqlQuery query("SELECT * FROM Data");
    int idName = query.record().indexOf("NameData");
    int valorName = query.record().indexOf("Datos");
    while (query.next())
    {
        QString names = query.value(idName).toString();
        QString val = query.value(valorName).toString();
        qDebug() << "===" << names<<val;
    }
}

int MainWindow::Extraer(const QString &name){
    int valor=0;
    if(dataExist(name)){
        QSqlQuery q;
        q.prepare("SELECT * FROM Data where NameData='"+name+"'");
        if(q.exec()){
            while(q.next()){
                valor=q.value(1).toInt();
            }
        }
    }else{
        qDebug()<<"not eXIST";
    }
    return valor;
}

void MainWindow::registera(const QString& name,int valor){
    QString cambio= Change(valor);
    if(dataExist(name)){
        UpdateData(name,cambio);
        ui->StatusInform->setText("Ha sido Actualizada la base de datos");
    }else{
        addData(name, cambio);
        ui->StatusInform->setText("Se ha agregado un nuevo valor");
    }
}
