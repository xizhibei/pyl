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

#define LABEL_NUM 8
#define LAYOUT_NUM 6
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

    void delDBFile(QString url);

private:
    Ui::Widget *ui;

    QProcess* dataCollector;
    QSqlDatabase db;
    QSqlQuery* query;

    QHBoxLayout* hlayout[LAYOUT_NUM];
    QLabel* icon_labels[LABEL_NUM];
    QLabel* visit_labels[LABEL_NUM];
    QLabel* del_labels[LABEL_NUM];
};

#endif // WIDGET_H
