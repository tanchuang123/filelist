#include "CustomTableView.h"
#include<QDebug>
#include<QScrollBar>
#include <QApplication>
CustomTableView::CustomTableView(QWidget *parent):QTableView(parent)
{
    //允许拖放
    setAcceptDrops(true);//允许拖放
    setDragDropMode(QAbstractItemView::DragDrop);
    setDragEnabled(true);//可拖放的
    setDropIndicatorShown(true);//设置拖放模式为移动项目，否则为复制项目
    connect(this,SIGNAL(clicked(QModelIndex)),SLOT(on_clicke_item(QModelIndex)));
}

CustomTableView::~CustomTableView()
{

}

void CustomTableView::dragEnterEvent(QDragEnterEvent *event)//拖拽事件（鼠标移入）
{
    qDebug()<<QStringLiteral("鼠标移入");
    CustomTableView* source =  qobject_cast<CustomTableView*>(event->source());
    if(source && source != this)
    {
        //确认是一个移动的操作
        event->acceptProposedAction();
        event->setDropAction(Qt::MoveAction);
        QModelIndex index = this->indexAt(event->pos());
        m_currentItem = index;
    }
}

void CustomTableView::dragMoveEvent(QDragMoveEvent *event)//鼠标移动
{
    qDebug()<<QStringLiteral("鼠标移动");
    CustomTableView* source =  qobject_cast<CustomTableView*>(event->source());
    if(source && source != this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CustomTableView::dropEvent(QDropEvent *event)//拖拽放下事件
{

    qDebug()<<QStringLiteral("拖拽放下事件");
    CustomTableView* source =  qobject_cast<CustomTableView*>(event->source());
    if(source && source != this)
    {
        if(m_type == RBK_ITEM)
        {
            emit  sig_upload();//上传
        }
        else
        {
            emit sig_download();//下载
        }
    }

}

void CustomTableView::mousePressEvent(QMouseEvent *event)//鼠标点击事件
{
     qDebug()<<QStringLiteral("鼠标点击事件");
    QModelIndex index = indexAt(event->pos());
    emit sig_press(index.data().toString());//点击事件
    /*QListView*/QTableView::mousePressEvent(event);

}

QModelIndex CustomTableView::currentItem()
{
    return m_currentItem;
}

QString CustomTableView::getItemName()
{
    return m_currentName;
}

void CustomTableView::setItemName(QString itemName)
{
   m_currentName = itemName;
}

CustomTableView::ItemType CustomTableView::getTableViewItemType()//item类型
{
    return m_type;
}

void CustomTableView::setTableViewItemType(CustomTableView::ItemType type)
{
    m_type = type;
}

void CustomTableView::on_clicke_item(QModelIndex index)
{
    m_currentItem = index;
}

void CustomTableView::slot_changeCursor(bool flag)
{
    if(flag)
    {
        setCursor(Qt::PointingHandCursor);//手型光标
    }
    else
    {
        setCursor(Qt::ArrowCursor);//箭头光标
    }
}

