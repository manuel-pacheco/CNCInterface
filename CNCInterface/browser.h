#ifndef BROWSER_H
#define BROWSER_H

#include <QDialog>
#include <ui_browser.h>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>

namespace Ui {
class Browser;
}

class Browser : public QDialog
{
    Q_OBJECT

public:
    explicit Browser( QWidget *parent = nullptr);
    ~Browser();
signals:
    void dlgReturn(QString);

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_Cerrrar_clicked();

    void on_Aceptar_clicked();

private:
    Ui::Browser *ui;
    QFileSystemModel *directorio;
    //QFileSystemModel *archivo;
};

#endif // BROWSER_H
