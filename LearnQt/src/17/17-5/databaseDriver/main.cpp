#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QStringList>
#include "connection.h"
#include <QVariant>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建数据库连接
    if (!createConnection()) return 1;

    // 使用QSqlQuery查询连接1的整张表，先要使用连接名获取该连接
    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    QSqlQuery query1(db1);
    qDebug() << "connection1:";
    query1.exec("select * from student");
    while(query1.next())
    {
        qDebug() << query1.value(0).toInt() << query1.value(1).toString();
    }
    // 使用QSqlQuery查询连接2的整张表
    QSqlDatabase db2 = QSqlDatabase::database("connection2");
    QSqlQuery query2(db2);
    qDebug() << "connection2:";
    query2.exec("select * from student");
    while(query2.next())
    {
        qDebug() << query2.value(0).toInt() << query2.value(1).toString();
    }

    // 以下是在例程17-4中添加的代码
    int numRows;
    // 先判断该数据库驱动是否支持QuerySize特性，如果支持，则可以使用size()函数，
    // 如果不支持，那么就使用其他方法来获取总行数
    if (db2.driver()->hasFeature(QSqlDriver::QuerySize)) {
        qDebug() << "has feature: query size";
        numRows = query2.size();
    } else {
        qDebug() << "no feature: query size";
        query2.last();
        numRows = query2.at() + 1;
    }
    qDebug() << "row number: " << numRows;

    // 指向索引为1的记录，即第二条记录
    query2.seek(1);
    // 返回当前索引值
    qDebug() << "current index: " << query2.at();
    // 获取当前行的记录
    QSqlRecord record = query2.record();
    // 获取记录中“id”和“name”两个属性的值
    int id = record.value("id").toInt();
    QString name = record.value("name").toString();
    qDebug() << "id: " << id << "name: " << name;
    // 获取索引为1的属性，即第二个属性
    QSqlField field = record.field(1);
    // 输出属性名和属性值，结果为“name”和“MaLiang”
    qDebug() << "second field: " << field.name()
             << "field value: " << field.value().toString();

    // 以下是在例程17-5中添加的代码
    query2.exec("insert into student (id, name) values (100, 'ChenYun')");

    // 下面是与上面代码等价的名称绑定实现代码
    //    query2.prepare("insert into student (id, name) values (:id, :name)");
    //    int idValue = 100;
    //    QString nameValue = "ChenYun";
    //    query2.bindValue(":id", idValue);
    //    query2.bindValue(":name", nameValue);
    //    query2.exec();

    // 下面是与上面代码等价的位置绑定实现代码
    //    query2.prepare("insert into student (id, name) values (?, ?)");
    //    int idValue = 100;
    //    QString nameValue = "ChenYun";
    //    query2.addBindValue(idValue);
    //    query2.addBindValue(nameValue);
    //    query2.exec();

    // 批处理
    query2.prepare("insert into student (id, name) values (?, ?)");
    QVariantList ids;
    ids << 20 << 21 << 22;
    query2.addBindValue(ids);
    QVariantList names;
    names << "xiaoming" << "xiaoliang" << "xiaogang";
    query2.addBindValue(names);
    if(!query2.execBatch()) qDebug() << query2.lastError();

    // 更新
    query2.exec("update student set name = 'xiaohong' where id = 20");
    // 删除
    query2.exec("delete from student where id = 21");

    query2.exec("select * from student");
    while(query2.next())
    {
        qDebug() << query2.value(0).toInt() << query2.value(1).toString();
    }

    return a.exec();
}








