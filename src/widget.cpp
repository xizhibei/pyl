#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    dataCollector = new QProcess(this);
    QStringList args;
    args <<  "-mrq"
        << "--excludei"
        <<"'.*(swp|swx|o|a|so|~)'"
        << "--excludei"
        <<"'.*/\\..*'"
        << "--timefmt"
        <<"'%Y-%m-%d %H:%M:%S'"
        <<"--format"
        <<"'%T#%w%f#%e' "
        <<"-e"
        <<"open,modify,create,delete"
        <<"/home/x";

    dataCollector->setProcessChannelMode(QProcess::MergedChannels);
    dataCollector->start("inotifywait",args);

    if(!dataCollector->waitForStarted(3000))
    {
        qDebug() << "Error while start inotifywait";
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");
    if(!db.open())
    {
        qDebug() << "Error while open database!";
    }
    query = new QSqlQuery(db);
    query->exec("create table files(id integer PRIMARY KEY,date integer,filename text,type text);");

    connect(dataCollector,SIGNAL(readyReadStandardOutput()),this,SLOT(readOutput()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(showDataBase()));

}

Widget::~Widget()
{
    dataCollector->close();
    delete dataCollector;
    db.close();
    delete ui;
}

void Widget::readOutput()
{
    while(dataCollector->canReadLine())
    {
        QByteArray buffer(dataCollector->readLine());

        buffer.remove(0,1);
        buffer.remove(buffer.length() - 3,3);
        if(buffer.endsWith("ISDIR"))
            continue;

        //qDebug() << buffer;

        QStringList vals = QString(buffer).split('#');
        uint time = QDateTime::fromString(vals[0],"'yyyy-MM-dd hh:mm:ss'").toTime_t();

        //        QString sql = "select * from files where file = '%1';";
        //        sql = sql.arg(vals[1]);

        //        if(!query->exec(sql))
        //            qDebug() << "Error select" << query->lastError().text();
        //        if(query->next())
        //        {
        //            sql = "insert into files values(NULL,%1,'%2','%3');";
        //            sql = sql.arg(time)
        //                    .arg(vals[1])
        //                    .arg(vals[2]);
        //        }

        QString sql = "insert into files values(NULL,%1,'%2','%3');";
        sql = sql.arg(time)
                .arg(vals[1])
                .arg(vals[2]);


        //qDebug() << sql;
        if(!query->exec(sql))
            qDebug() << "Error insert data: " << query->lastError().text();
    }
}

void Widget::showDataBase()
{
    QStandardItemModel *model = new QStandardItemModel;
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,tr("Id"));
    model->setHeaderData(1,Qt::Horizontal,tr("Date"));
    model->setHeaderData(2,Qt::Horizontal,tr("File"));
    model->setHeaderData(3,Qt::Horizontal,tr("Type"));

    query->exec("select * from files limit 0,7;");
    while(query->next())
    {
        QString time = QDateTime::fromTime_t(query->value(1).toInt()).toString("yyyy-MM-dd hh:mm:ss");
        QStandardItem* item1 = new QStandardItem(query->value(0).toString());
        QStandardItem* item2 = new QStandardItem(time);
        QStandardItem* item3 = new QStandardItem(query->value(2).toString());
        QStandardItem* item4 = new QStandardItem(query->value(3).toString());
        QList<QStandardItem*> item;
        item << item1 << item2 << item3 << item4;
        model->appendRow(item);
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeRowsToContents();
}
