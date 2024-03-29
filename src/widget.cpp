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
        <<".*(swp|swx|o|a|so|~)"
        << "--excludei"
        <<".*/\\..*"
        << "--excludei"
        <<".*user\\.db.*"
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
    //query->exec("create table files(id integer PRIMARY KEY,date integer,filename text,type text);");

    connect(dataCollector,SIGNAL(readyReadStandardOutput()),this,SLOT(readOutput()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(updateLabels()));

    for(int i = 0;i < LAYOUT_NUM;i++)
    {
        hlayout[i] = new QHBoxLayout();
        ui->verticalLayout->addLayout(hlayout[i]);
    }
    for(int i = 0;i < LABEL_NUM ;i++)
    {
        icon_labels[i] = new QLabel(QString("Test"));
        visit_labels[i] = new QLabel(QString("Test"));
        del_labels[i] = new QLabel(QString("Test"));
        //labels[i]->setFixedSize(200,200);
        connect( visit_labels[i],SIGNAL( linkActivated(QString) ),this,SLOT( visitAddr(QString) ) );
        connect( del_labels[i],SIGNAL( linkActivated(QString) ),this,SLOT( delDBFile(QString) ) );

        hlayout[ i / 4 * 3 ]->addWidget(icon_labels[i]);
        hlayout[ i / 4 * 3 + 1 ]->addWidget(visit_labels[i]);
        hlayout[ i / 4 * 3 + 2 ]->addWidget(del_labels[i]);
    }

    updateLabels();
}

Widget::~Widget()
{
    for(int i = 0;i < LAYOUT_NUM;i++)
        delete hlayout[i];
    for(int i = 0;i < LABEL_NUM;i++)
    {
        delete icon_labels[i];
        delete visit_labels[i];
        delete del_labels[i];
    }

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

        if(buffer.indexOf("/.") != -1)
            continue;

        qDebug() << buffer;

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
        usleep(100000);
    }
}

void Widget::showDataBase()
{
    //QStandardItemModel *model = new QStandardItemModel;
    //model->setColumnCount(1);
    //model->setHeaderData(0,Qt::Horizontal,tr("Id"));
    //model->setHeaderData(1,Qt::Horizontal,tr("Date"));
    //model->setHeaderData(2,Qt::Horizontal,tr("File"));
    //model->setHeaderData(3,Qt::Horizontal,tr("Type"));

    query->exec("select filename,count(*) as num from files group by filename order by num limit 0,8;");
    while(query->next())
    {
        qDebug() << query->value(0).toString();
        //QString time = QDateTime::fromTime_t(query->value(1).toInt()).toString("yyyy-MM-dd hh:mm:ss");
        //QStandardItem* item1 = new QStandardItem(query->value(0).toString());
        //QStandardItem* item2 = new QStandardItem(time);
        //QStandardItem* item2 = new QStandardItem(query->value(2).toString());
        //QStandardItem* item4 = new QStandardItem(query->value(3).toString());
        //QList<QStandardItem*> item;
        //item << item1;//  << item2 << item3 << item4;
        //model->appendRow(item);
    }

    //ui->tableView->setModel(model);
    //ui->tableView->set
    //ui->tableView->resizeRowsToContents();
}

void Widget::updateLabels()
{
    query->exec("select filename,count(*) as num from files group by filename order by num desc limit 0,8;");
    int i = 0;
    while(query->next())
    {
        qDebug() << query->value(0).toString() << "   " << query->value(1).toString();
        QFileInfo fi = QFileInfo(query->value(0).toString());
        QFileIconProvider fip;

        visit_labels[i]->setText("<a href = " +query->value(0).toString() + ">"+fi.fileName()+"</a>");
        del_labels[i]->setText("<a href = " +query->value(0).toString() + ">Delete from db</a>");
        icon_labels[i]->setPixmap( fip.icon(fi).pixmap(200,200) );
        i++;
    }
}

void Widget::visitAddr(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}

void Widget::delDBFile(QString url)
{
    if( query->exec(QString("delete from files where filename = '%1'").arg( url ) ) )
        qDebug() << "Delete " << url << " success!";
    updateLabels();
}
