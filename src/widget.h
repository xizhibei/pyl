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
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QFileIconProvider>

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
    void updateLabels();
    void visitAddr(QString url);

private:
    Ui::Widget *ui;

    QProcess* dataCollector;
    QSqlDatabase db;
    QSqlQuery* query;

    QHBoxLayout* hlayout[2];
    QLabel* labels[8];
};

#endif // WIDGET_H
