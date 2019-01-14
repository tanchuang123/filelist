
#include <QApplication>
#include "Test/TableViewWidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    FileWidget w;
    //    QFile styleFile(qApp->applicationDirPath()+"/style.css");
    //    styleFile.open(QIODevice::ReadOnly);
    //    QString styles = styleFile.readAll();
    ////    CStyleManager::transStyles(styles);


    //    a.setStyleSheet(styles);

    //    styleFile.close();


    QFile file(qApp->applicationDirPath()+"/style.css");
    qDebug()<< qApp->applicationDirPath()<<qApp->applicationFilePath()<< QCoreApplication::applicationFilePath()<<"jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj";
    if(file.open(QFile::ReadOnly))
    { QString style = QString::fromLatin1(file.readAll());
        a.setStyleSheet(style);
        file.close();
    }


    TableViewWidget w;
    w.show();

    return a.exec();
}
