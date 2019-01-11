#include "ListViewDelegate.h"
#include <QApplication>
#include"CustomListView.h"
#include "ListViewWidget.h"
#include <QStyle>

ListViewDelegate::ListViewDelegate(QWidget *parent):
    QStyledItemDelegate(parent),
    m_pDeleteButton(new QPushButton()),
    m_pDownLoad(new QPushButton()),
    m_pLabel1(new QPushButton("xxx")),
    m_pLabel2(new QPushButton()),
    m_nSpacing(9),
    m_nWidth(25),
    m_nHeight(20)
//    m_type(DELEGATE_RBK)
{

//        this->loadStyleSheet(":/qss/myStyleSheet.qss");

    m_parent = dynamic_cast<CustomListView*>(parent);//强制类型转换为CustomListView
    //注意：此处的样式只有使用样式表后才会有效果，不知道是不是qt的bug.
    // 设置按钮正常、划过、按下样式.
//    QString imagePath = StyleObject::init()->getCurrentSkinDirPath()+"/images";
//    QString imagePath ="F:/work/FileList/image/";
    QString imagePath = qApp->applicationDirPath()+"/image/";
    qDebug()<<"imagepath is ====="<<imagePath;
//    注意：此处的样式只有使用样式表后才会有效果，不知道是不是qt的bug.
        // 设置按钮正常、划过、按下样式. "QPushButton{border:none;background-color::red;"
    m_pDownLoad->setStyleSheet(QString("QPushButton{background:none;"
                                          "border:none;"
                                          "image:url(%1download.svg);}"
                                          "QPushButton:hover{image:url(%1download_gray.svg);}"
                                          "QPushButton:pressed{image:url(%1download.svg);}").arg(imagePath));

    m_pDeleteButton->setStyleSheet(QString("QPushButton{background:none;"
                                           "border:none;"
                                           "image:url(%1delete.svg);}"
                                           "QPushButton:hover {image:url(%1delete_gray.svg);}"
                                           "QPushButton:pressed {image:url(%1delete.svg);}").arg(imagePath));

    m_pLabel1->setStyleSheet(QString("QPushButton{background:none;"
                                          "border:none;"
                                          "image:url(%1file.svg);}"
                                          "QPushButton:hover{image:url(%1file_gray.svg);}"
                                          "QPushButton:pressed{image:url(%1file.svg);}").arg(imagePath));

    m_pLabel2->setStyleSheet(QString("QPushButton{background:none;"
                                           "border:none;"
                                           "image:url(%1dir.svg);}"
                                           "QPushButton:hover {image:url(%1dir_gray.svg);}"
                                           "QPushButton:pressed {image:url(%1dir.svg);}").arg(imagePath));




    m_list<<tr("DownLoad")<<tr("Delete")<<tr("111")<<tr("222");



    connect(this,SIGNAL(sigChangeCursor(bool)),m_parent,SLOT(slot_changeCursor(bool)));
}

ListViewDelegate::~ListViewDelegate()
{

}
//绘制按钮
void ListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const//QStyledItemDelegate是专门用于项的(item)的视图委托
{
    painter->save();//保存绘制状态
    QStyledItemDelegate::paint(painter, option, index);
    painter->restore();//恢复绘制状态

    if( ! (option.state & QStyle::State_MouseOver)){//鼠标移过悬浮
        return;
    }

    //计算按钮显示区域
    int nCount = m_list.count();
    qreal nHalf = (option.rect.width() - m_nWidth * nCount - m_nSpacing * (nCount - 1)) / 2.0;
    qreal nTop = (option.rect.height() - m_nHeight) / 2.0;

//    if(m_type == DELEGATE_LOCAL)
//    {
//        // 绘制按钮.
//        QStyleOptionButton button;

//        button.rect = QRect(option.rect.left() + nHalf*2 + m_nWidth + m_nSpacing,
//                            option.rect.top() + nTop,  m_nWidth, m_nHeight);
//        button.state |= QStyle::State_Enabled;

//        if (button.rect.contains(m_mousePoint))
//        {
//            if (m_nType == 0)
//            {
//                button.state |= QStyle::State_MouseOver;//hover鼠标悬停
//            }
//            else if (m_nType == 1)
//            {
//                button.state |= QStyle::State_Sunken;//press凹陷
//            }
//        }
//        QWidget *pWidget =  m_pDeleteButton.data();
//        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
//    }
//    else if(m_type == DELEGATE_RBK)
//    {
        for (int i = 0; i < nCount; ++i)
        {
            // 绘制按钮.
            QStyleOptionButton button;
            button.rect = QRect(option.rect.left() + nHalf*2 + m_nWidth * i + m_nSpacing * i,
                                option.rect.top() + nTop,  m_nWidth, m_nHeight);
            button.state |= QStyle::State_Enabled;
            if (button.rect.contains(m_mousePoint))
            {
                if (m_nType == 0)
                {
                    button.state |= QStyle::State_MouseOver;//hover鼠标滑过
                }
                else if (m_nType == 1)
                {
                    button.state |= QStyle::State_Sunken;//press鼠标按下
                }
            }
//            QWidget *pWidget = (i == 0) ? m_pShiftButton.data() : m_pDeleteButton.data();
            if(i==0)
            {
                QWidget *pWidget = m_pDownLoad.data();
                QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
            }
           if(i==1)
            {

                QWidget *pWidget = m_pDeleteButton.data();
                QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
            }
           if(i==2)
           {
               QWidget *pWidget = m_pLabel1.data();
               QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
           }
           if(i==3)
           {
               QWidget *pWidget = m_pLabel2.data();
               QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
           }

        }

//    }

}
//按钮响应按下，滑过
bool ListViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    m_nType = -1;

    QMouseEvent *pEvent = static_cast<QMouseEvent *> (event);
    m_mousePoint = pEvent->pos();

    int nCount = m_list.count();
    int nHalf = (option.rect.width() - m_nWidth * nCount - m_nSpacing * (nCount - 1)) / 2;
    int nTop = (option.rect.height() - m_nHeight) / 2;

    //还原鼠标光标
    emit sigChangeCursor(false);

    for (int i = 0; i < nCount; ++i)
    {
        QStyleOptionButton button;
        button.rect = QRect(option.rect.left() + nHalf*2 + m_nWidth * i + m_nSpacing * i,
                            option.rect.top() + nTop,  m_nWidth, m_nHeight);

        // 鼠标不在按钮之上.
        if (!button.rect.contains(m_mousePoint))
        {
            if(QToolTip::isVisible())
            {
                QToolTip::hideText();
            }
            continue;
        }
        else
        {
        }
        // 鼠标位于按钮之上.
        switch (event->type())
        {
        // 鼠标滑过.
        case QEvent::MouseMove:
        {
            // 设置鼠标样式为手型.
            if(i==0&&m_type == DELEGATE_LOCAL)
            {
                //若是本地列表并且是切换按钮区域则鼠标光标不显示手型
                emit sigChangeCursor(false);
            }
            else
            {
                emit sigChangeCursor(true);
            }
            m_nType = 0;
            if(m_type != DELEGATE_LOCAL){
                QToolTip::showText(pEvent->globalPos(), m_list.at(i));
            }
            else
            {
                QToolTip::showText(pEvent->globalPos(),m_list.at(1));
            }
            break;
        }
            // 鼠标按下.
        case QEvent::MouseButtonPress:
        {
            m_nType = 1;
            break;
        }
            // 鼠标释放.
        case QEvent::MouseButtonRelease:
        {
            if (i == 0)
            {
//
                emit sigDownLoad(index);
            }
            else if(i==1)
            {
                emit sigDeleteData(index);
            }
            else if(i==2)
            {

            }
            else {

            }
            break;
        }

        default:
            break;
        }
        return true;
    }

    return false;
}

void ListViewDelegate::setType(ListViewDelegate::Type type)
{
    m_type = type;
}

void ListViewDelegate::setButtonList(const QList<QPushButton *> &value)
{
    ButtonList = value;
}
//void ListViewDelegate::setEditorData(QWidget *editor, const QModelIndex &index)const
//{
//    //返回该索引的模型，继而返回该模型中此索引的编辑角色数据
//    int value = index.model()->data(index, Qt::EditRole).toInt();
//    //给控件赋值
//    QPushButton * btn = static_cast<QPushButton*>(editor);
//    if(index.column()==2)
//    {
//        btn->setText(QStringLiteral("下载"));
//        connect(btn,SIGNAL(clicked()),this,SLOT(slotDownload()));
//    }
//    else if(index.column()==3){
//        btn->setText(QStringLiteral("删除"));
//        QIcon icon("F:/work/FileList/delete.svg");
//        btn->setIcon(icon);
//        connect(btn,SIGNAL(clicked()),this,SLOT(slotDelete()));
//    }

//}


