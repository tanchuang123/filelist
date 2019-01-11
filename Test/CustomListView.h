#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H
#include <QListView>
#include <QMimeData>
#include <QDropEvent>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMouseEvent>
#include <QStandardItem>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QTableView>
//class CustomListView:public QListView
class CustomListView:public QTableView
{
    Q_OBJECT
public:
    enum ItemType
    {
        RBK_ITEM,
        LOCAL_ITEM
    };
public:
    CustomListView(QWidget *parent = Q_NULLPTR);
    ~CustomListView();
    void setListViewItemType(ItemType type);
    ItemType getListViewItemType();
    QModelIndex currentItem();
    QString  getItemName();
    void setItemName(QString itemName);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void sig_upload();
    void sig_download();
    void sig_press(QString currentName);
public slots:
    void on_clicke_item(QModelIndex index);
    void slot_changeCursor(bool flag);
private:
     QPoint startPos;

     QModelIndex m_currentItem ;//当前item
     QString m_currentName;//当前名字
     ItemType m_type;//item类型
};

#endif // CUSTOMLISTVIEW_H
