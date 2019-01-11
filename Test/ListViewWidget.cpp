#include "ListViewWidget.h"
#include "ui_ListViewWidget.h"

#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QTemporaryFile>
#include <QHeaderView>
#include <QDateTime>
ListViewWidget::ListViewWidget( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListViewWidget)
{
    ui->setupUi(this);
    ui->listView->setMouseTracking(true);//鼠标跟随

//    ui->listView->setSelectionBehavior(QTableView::NoSelection);
    ui->listView->setSelectionMode(QTableView::NoSelection);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    ui->listView->setAlternatingRowColors(true);



    delegate = new ListViewDelegate(ui->listView);
//    ui->listView->setItemDelegate(delegate);

    ui->listView->setItemDelegateForColumn(3,delegate);
//    ui->listView->columnWidth(30);
//    ui->listView->setColumnWidth(0,40);

    connect(ui->listView,SIGNAL(sig_press(QString)),this,SLOT(slot_listView_pressed(QString)));
    connect(delegate,SIGNAL(sigDownLoad(const QModelIndex &)),this,SLOT(slotDownLoad(const QModelIndex&)));
    connect(delegate,SIGNAL(sigDeleteData(const QModelIndex&)),this,SLOT(slotDeleteData(const QModelIndex&)));
    model = new QStandardItemModel(ui->listView);//创建本地地图列表listView model


    model->setColumnCount(4);

    ui->listView->setModel(model);


    ui->directoryComboBox->setEditable(true);//可编辑
    ui->directoryComboBox->addItem(QDir::toNativeSeparators(QDir::currentPath()));//加入文本
    ui->directoryComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

//    ui->listView->horizontalHeader()->setStretchLastSection(true);
     ui->listView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//     ui->listView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );//根据列内容来定列宽
        ui->listView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);//对第0列单独设置固定宽度
        ui->listView->setColumnWidth(3, 50);//设置固定宽度


//        ui->listView->setStyleSheet("QTableView::item{selection-background-color:white}"

//                                           "QTableView{font-family:\"wenquanyi\";font-size:13px}");

     ui->listView->clearSelection();
     ui->listView->setShowGrid(false);
     ui->listView->verticalHeader()->hide();
     ui->listView->horizontalHeader()->hide();




}

ListViewWidget::~ListViewWidget()
{
    delete ui;
}


void ListViewWidget::loadLocalFiles(QString &FilePath)
{
    if(!FilePath.isEmpty())
    {
       QFileInfoList list = GetFileList(FilePath);
       showFileList(list);
    }
}
QFileInfoList ListViewWidget::GetFileList(QString path)
{
    QDir dir(path);
    //查看路径中后缀为.smap格式的文件
    QStringList filters;
    filters<<QString("*");
    QFileInfoList file_list   = dir.entryInfoList(filters, QDir::Files | QDir::Hidden | QDir::NoSymLinks);//设置要读取的文件格式
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath() ;
        qDebug()<<name<<"_______________name____________";
        QFileInfoList child_file_list = GetFileList(name);
        file_list.append(child_file_list);
    }
    return file_list;
}

void ListViewWidget::showFileList(QFileInfoList list)
{
    QStringList namelist;
    QStringList pathlist;
    for(int i = 0; i < list.size();i++)
    {
        namelist<<list.at(i).fileName().remove("");

//        qDebug()<<list.at(i).fileName().remove("")<<"**************list.at(i).fileName().remove("")********";
        pathlist<<list.at(i).absoluteFilePath();
//        qDebug()<<list.at(i).absoluteFilePath()<<"**************list.at(i).absoluteFilePath()********";
    }

    setMapsList(namelist,pathlist);

//    setFileInfo(namelist,pathlist);
}

void ListViewWidget::setMapsList(QStringList maplist, QStringList pathList)
{
    clearMapsModel();
    _dataList.clear();

    for(int i = 0; i < maplist.size();i++)
    {
        QStandardItem* tempItem = new QStandardItem;
        tempItem->setText(maplist.at(i));
        tempItem->setData(pathList.at(i),Qt::UserRole);
        tempItem->setIcon(fileIcon(maplist.at(i)));

        QFileInfo info(pathList.at(i));
        qint64 size = info.size();//文件大小,单位byte
        QDateTime created = info.created();//创建时间
        QDateTime lastModified = info.lastModified();//最后修改时间

        QString str = lastModified.toString();
//        qDebug()<<pathList.at(i)<<str<<"ccccccccccccccccccccccccccccccccccccccccccccccc";
//        QDateTime lastRead = info.lastRead();//最后读取时间
        QStandardItem* tempItemSize = new QStandardItem;
        tempItemSize->setText(QString("%1 KB").arg(int((size + 1023) / 1024)));
        tempItemSize->setData(QString("%1 KB").arg(int((size + 1023) / 1024)),Qt::UserRole);
//        tempItemSize->setIcon(fileIcon(maplist.at(i)));

        QStandardItem* tempItemData = new QStandardItem;
        tempItemData->setText(str);
        tempItemData->setData(str,Qt::UserRole);
//        tempItemData->setIcon(fileIcon(maplist.at(i)));

        qobject_cast<QStandardItemModel*>(ui->listView->model())->setItem(i,0,tempItem);
        qobject_cast<QStandardItemModel*>(ui->listView->model())->setItem(i,1,tempItemSize);
        qobject_cast<QStandardItemModel*>(ui->listView->model())->setItem(i,2,tempItemData);

        _dataList<<maplist.at(i) ;//记录所有数据
    }

   //将当前被使用地图调整到一个行
//    QStandardItem* curItem = new QStandardItem;
//    for(int i = 0; i<ui->listView->model()->rowCount();i++)
//    {
//        if(dynamic_cast<QStandardItemModel*>(ui->listView->model())->item(i)->text()==getCurrentUsedMap())
//        {
//            curItem->setText(getCurrentUsedMap());
//            curItem->setData(dynamic_cast<QStandardItemModel*>(ui->listView->model())->item(i)->data(Qt::UserRole).toString());
//            ui->listView->model()->removeRow(i);
//        }

//    }
//    qobject_cast<QStandardItemModel*>(ui->listView->model())->insertRow(0,curItem);
    //加粗变色当前正在被使用的地图
    updateCurrentMapFlag(getCurrentUsedMap());

}

QStringList ListViewWidget::getDataList()
{
    return _dataList;
}

void ListViewWidget::clearMapsModel()
{
    if(ui->listView->model())
    {
        int size = qobject_cast<QStandardItemModel*>(ui->listView->model())->rowCount();
        for(int i = 0; i < size;i++)
        {
            qobject_cast<QStandardItemModel*>(ui->listView->model())->removeRow(0);//removeRow(0)每次删除model数据会变化
        }
    }
}

void ListViewWidget::updateCurrentMapFlag(QString itemname)
{
//    qDebug()<<"itemname is ================"<<itemname;
    if(ui->listView->model())
    {
        int size  = ui->listView->model()->rowCount();

        for(int i = 0; i <size;i++)
        {
            QString name =  qobject_cast<QStandardItemModel*>(ui->listView->model())->item(i)->text();
            if(name == itemname)
            {
                QFont font=dynamic_cast<QStandardItemModel*>(ui->listView->model())->item(i)->font();
                font.setBold(true);
                font.setPointSize(12);
                dynamic_cast<QStandardItemModel*>(ui->listView->model())->item(i)->setFont(font);
                dynamic_cast<QStandardItemModel*>(ui->listView->model())->item(i)->setForeground(QColor(18,150,219));
            }
            else
            {

            }

        }

    }
}

CustomListView *ListViewWidget::getlistView()
{
    return ui->listView;
}

ListViewWidget::mapType ListViewWidget::getType()
{
    return type_;
}

void ListViewWidget::setType(ListViewWidget::mapType ty)
{
    type_ = ty;
}

void ListViewWidget::setSelectedMap(QString selemap)
{
    _selectedMap = selemap;
}

QString ListViewWidget::getSelectedMap()
{
    return _selectedMap;
}

QString ListViewWidget::getCurrentProcessMap()
{
    return _currentProcessedMap;
}

void ListViewWidget::setCurrentProcessMap(QString curmap)
{
    _currentProcessedMap = curmap;
}

void ListViewWidget::setDelSuccessFlag(bool success)
{
    _del_success = success;
}

bool ListViewWidget::getDelSuccessFlag()
{
    return _del_success;
}

void ListViewWidget::setCurrentUsedMap(QString usedMap)
{
    _currentUsedMap = usedMap;
}

QString ListViewWidget::getCurrentUsedMap()
{
    return _currentUsedMap;
}

void ListViewWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() ==Qt::Key_Delete)
    {
      slotDeleteData(ui->listView->currentItem());
    }
}

void ListViewWidget::slotDeleteData(const QModelIndex &index)
{
    //确定删除时的所点击的map
      qDebug()<<"=========delete================="<<index.data().toString();


    if(_dataList.contains(model->item(index.row(),0)->text()))
    {
        qDebug()<<"=========delete================="<<index.data().toString();
        _dataList.removeOne(index.data().toString());
    }


    QFile file(_filePath+"/"+model->item(index.row(),0)->text());
    if(file.remove())
    {
        model->removeRow(index.row());
    }

    qDebug()<<file.remove()<<_filePath+"/"+index.data().toString()<<"2222222222222222222222222222222222";
}
void ListViewWidget::slotDownLoad(const QModelIndex &index)
{
    QFile file(_filePath+"/"+index.data().toString());
    if(file.open(QIODevice::ReadOnly))
    {
//      QByteArray str= file.readAll();
      QByteArray t ;
             while(!file.atEnd())
             {
                 t += file.readLine();
             }
       QFile file1;
       file1.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), QString("/"),QString("TEXT(*.txt)")));
       file1.open(QIODevice::WriteOnly /*| QIODevice::Text*/);
       file1.write(t);
       file1.close();
       file.close();

    }
}

void ListViewWidget::on_listView_clicked(const QModelIndex &index)
{
    _selectedMap = index.data().toString();
//    qDebug()<<"slect map --------------------1"<<_selectedMap;
    emit sig_click(index.row());
}

void ListViewWidget::slot_listView_pressed(QString itemName)
{
    _selectedMap = itemName;
//    qDebug()<<"slect map --------------------2"<<_selectedMap;
}

void ListViewWidget::on_browseButton_clicked()
{
    QString directory =
        QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files"), QDir::currentPath()));

    if (!directory.isEmpty()) {
        if (ui->directoryComboBox->findText(directory) == -1)
            ui->directoryComboBox->addItem(directory);
        ui->directoryComboBox->setCurrentIndex(ui->directoryComboBox->findText(directory));
    }
    loadLocalFiles(directory);
    _filePath = directory.replace("\\","/");

    qDebug()<<_filePath<<"11111111111111111111";
}


QIcon ListViewWidget::fileIcon(const QString &extension) const
{
    QFileIconProvider provider;
    QIcon icon;
    QString strTemplateName = QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "_XXXXXX." + extension;
    QTemporaryFile tmpFile(strTemplateName); tmpFile.setAutoRemove(false);
    if (tmpFile.open())
    {
        tmpFile.close();
        icon = provider.icon(QFileInfo(strTemplateName));
        // tmpFile.remove();
    }
    else
    {
        qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
    }
    return icon;
}
QString ListViewWidget::fileType(const QString &extension) const

 {

    QFileIconProvider provider;
    QString strType;
    QString strFileName = QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "_XXXXXX." + extension;
    QTemporaryFile tmpFile(strFileName);
    tmpFile.setAutoRemove(false);
    if (tmpFile.open())
    {
        tmpFile.close();
        strType = provider.type(QFileInfo(tmpFile.fileName()));
        // tmpFile.remove();
    }
    else
    {
        qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
    }
    return strType;
}
void ListViewWidget::setFileInfo(QStringList maplist, QStringList pathList)
{
//    clearMapsModel();
//    _dataList.clear();

    for(int i = 0; i < maplist.size();i++)
    {
        QStandardItem* tempItem = new QStandardItem;



        QFileInfo info(pathList.at(i));
//        qint64 size = info.size();//文件大小,单位byte
//        QDateTime created = info.created();//创建时间
        QDateTime lastModified = info.lastModified();//最后修改时间

        QString str = lastModified.toString();
        qDebug()<<pathList.at(i)<<str<<"ccccccccccccccccccccccccccccccccccccccccccccccc";
//        QDateTime lastRead = info.lastRead();//最后读取时间
//        qDebug() << size;
//        qDebug() << created;
//        qDebug() << lastModified;
//        qDebug() << lastRead;
//        bool isDir = info.isDir();//是否是目录
//        bool isFile = info.isFile();//是否是文件
//        bool isSymLink = info.isSymLink();//是否是符号链接
//        bool isHidden = info.isHidden();//是否隐藏
//        bool isReadable = info.isReadable();//是否可读
//        bool isWritable = info.isWritable();//是否可写
//        bool isExecutable = info.isExecutable();//是否是可执行文件
//         model->setItem(i,1,new QStandardItem(str));
//         ui->listView->setModel(model);
        tempItem->setText(str);
//        tempItem->setData(str,Qt::UserRole);

        qobject_cast<QStandardItemModel*>(ui->listView->model())->setItem(i,1,tempItem);
//        _dataList<<maplist.at(i) ;//记录所有数据

    }




}
