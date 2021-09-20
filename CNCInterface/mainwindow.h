#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <info.h>
#include <browser.h>

#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QSerialPort>


static const QString path = "example.db";

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Qtimer;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QSqlDatabase m_db;
    QString Change(int valor);

    void connClose()
    {
       m_db.close();
       m_db.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connopen()
    {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        //qDebug() << "Error: connection with database fail";
        return false;
    }
    else
    {
        //qDebug() << "Database: connection ok";
        return true;
    }
}

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool createTable();
    bool removeAllData();
    bool dataExist(const QString& name) const;
    void addData(const QString&name ,const QString&valor);
    void printAllData() const;
    void UpdateData(const QString&name,const QString& valor);
    void registera(const QString& name,int valor);
    int Extraer(const QString& name);
    void menuinfo();
    void loadingValues();
    void enable();
    void disable();

public slots:
    void buscaYconectaArduino();

private slots:
    void on_comboBox_activated(int index);
    void on_actionInfo_triggered();
    void on_upLeft_9_clicked();
    void on_upLeft_2_clicked();
    void SerialReceived();
    void on_Folder_clicked();
    void showValue(QString direccion);


    void on_lineEdit_returnPressed();

    void on_ConectCOM_clicked();



    void on_Conectar_clicked();

    void on_upLeft_10_clicked();

    void on_changeROUTER_clicked();

    void on_changeLASER_clicked();

    void on_StartSend_clicked();

    void on_verticalSlider_2_valueChanged(int value);

    void on_upLeft_3_clicked();

    void on_upLeft_4_clicked();

    void on_upLeft_6_clicked();

    void on_upLeft_11_clicked();

    void on_upLeft_7_clicked();

    void on_upLeft_8_clicked();

    void on_upZ_clicked();

    void on_LOWZ_clicked();

    void on_HOMEZ_clicked();

    void on_verticalSlider_valueChanged(int value);


private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    //variable de para recepcion de direccion de archivo
    Browser *ventana;
    QTimer *m_timer;

    static const quint16 arduino_uno_vendor_id = 6790;
    static const quint16 arduino_UNO = 29987;
    static const quint16 arduino_MEGA = 67;
    QString arduino_puerto;
    int BaudiosWork;
    bool arduino_esta_disponible;
    bool Start;
    bool Start_data;
    bool RL_CONFIG;
    int manual_Speed_Aux;
    int manual_Distance_Aux;
    QByteArray manual_Speed;
    QByteArray manual_Distance;

    double actual_x;
    double actual_y;
    double actual_z;
    double actual_c;

    double job_x;
    double job_y;
    double job_z;
    double job_c;
};
#endif // MAINWINDOW_H
