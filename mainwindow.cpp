#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString servername = ui->l_servName->toPlainText();    //"DESKTOP-EMM81CA";
    QString dbName = ui->l_dbName->toPlainText();          //"test_drive";
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
    db.setConnectOptions();
    QString dsn = QString("Driver={SQL Server};SERVER=%1;DATABASE=%2;Trusted_Connection=Yes;").arg(servername).arg(dbName);
    db.setDatabaseName(dsn);

    QString name = ui->l_employee->toPlainText();


    if(db.open())
    {
        ui->list->clear();
        QSqlQuery qry;
        if(qry.exec("SELECT  DATEDIFF(minute, pTime, time) as time FROM(SELECT  *, LAG(time) OVER (ORDER BY time) pTime FROM    archive) q WHERE   pTime IS NOT NULL and card_id = (select id from card where info='"+name+"')"))
        {
            int i(1), j(1);
            while(qry.next())
            {
                if((j++%2)!=0)
                    ui->list->addItem( "Entrance "+QString::number(i++)+":\t"+qry.value(0).toString()+" minutes");
            }
        }
        db.close();
    }
    else
    {
        qDebug()<<"Error = "<<db.lastError();
    }
}


