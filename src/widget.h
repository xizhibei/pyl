#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QStandardItemModel>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void readOutput();
    void showDataBase();

private:
    Ui::Widget *ui;

    QProcess* dataCollector;
    QSqlDatabase db;
    QSqlQuery* query;
};

#endif // WIDGET_H