#ifndef TABLEVIEWWIDGET_H
#define TABLEVIEWWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QFileInfoList>
#include "TableViewDelegate.h"
#include "CustomTableView.h"
#include <QKeyEvent>
#include <QFileSystemModel>
#include <QIcon>
namespace Ui {
class TableViewWidget;
}

class TableViewWidget : public QWidget
{
    Q_OBJECT
public:
    enum mapType
    {
        MAP_LOCAL,
        MAP_RBK
    };
public:
    explicit TableViewWidget(QWidget *parent = 0);
    ~TableViewWidget();
    //加载本地地图文件
    void  loadLocalMaps();
    void  loadLocalFiles(QString &FilePath);
    void  loadMaps(QString& path);
    QFileInfoList GetFileList(QString path);
    void showFileList(QFileInfoList list);
    void setMapsList(QStringList maplist);
    void setMapsList(QStringList maplist,QStringList pathList);
    void setFileInfo(QStringList maplist,QStringList pathList);
    QStringList getDataList();
    void clearMapsModel();//清楚本地地图model数据
    void updateCurrentMapFlag(QString itemname);
    CustomTableView* getTableView();
    mapType getType();
    void setType(mapType ty);

    void setSelectedMap(QString selemap);
    QString getSelectedMap();
    QString getCurrentProcessMap();
    void    setCurrentProcessMap(QString curmap);
    void setDelSuccessFlag(bool success);
    bool getDelSuccessFlag();

    void setCurrentUsedMap(QString usedMap);
    QString getCurrentUsedMap();
    QString fileType(const QString &extension) const;
    QIcon fileIcon(const QString &extension) const;
public:
     void keyPressEvent(QKeyEvent *event);
signals:
    void sig_deletRBK(int idex);

    void sig_deletLocal(const QString& path,int idex);
    void sig_click(int idx);
public slots:
    void slotDeleteData(const QModelIndex &index);

//    void on_listView_clicked(const QModelIndex &index);
//    void slot_listView_pressed(QString itemName);
    void slotDownLoad(const QModelIndex &index);
private:
    Ui::TableViewWidget *ui;
    //model
    QStandardItemModel *model;
//    QFileSystemModel *model;
    QString _selectedMap;//被选中的地图
    QString _currentProcessedMap;//当前操作(切换或删除)的地图
    QString _currentUsedMap;//当前崩机器人使用的地图
    mapType  type_;
    bool _del_success;//删除是否成功
    QStringList _dataList;//名称列表
    QString _filePath;
public:
    TableViewDelegate * delegate;
private slots:
    void on_browseButton_clicked();
};

#endif // LISTVIEWWIDGET_H
