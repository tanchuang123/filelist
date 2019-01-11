#ifndef LISTVIEWDELEGATE_H
#define LISTVIEWDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QToolTip>
#include <QDebug>
#include<QLabel>
class CustomListView;
class ListViewDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum Type
    {
      DELEGATE_LOCAL,
      DELEGATE_RBK
    };
public:
    ListViewDelegate( QWidget *parent = 0);
    ~ListViewDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
    void setType(Type type);
    void setButtonList(const QList<QPushButton *> &value);
//    void setEditorData(QWidget *editor, const QModelIndex &index)const;
signals:
//    void sigShift(const QModelIndex &index);
    void sigDownLoad(const QModelIndex &index);
    void sigDeleteData(const QModelIndex &index);
    void sigChangeCursor(bool flag); //true表示手动光标，否则表示箭头光标
private:
    QPoint m_mousePoint;  // 鼠标位置.

    QScopedPointer<QPushButton> m_pDownLoad;//下载
    QScopedPointer<QPushButton> m_pDeleteButton;//删除

    QScopedPointer<QPushButton> m_pLabel1;//下载
    QScopedPointer<QPushButton> m_pLabel2;//删除

    QStringList m_list;
    QList<QPushButton*>ButtonList;
    int m_nSpacing;  // 按钮之间的间距.
    int m_nWidth;  // 按钮宽度.
    int m_nHeight;  // 按钮高度.
    int m_nType;  // 按钮状态-1：划过 2：按下.

    Type m_type;

    CustomListView* m_parent;//listView基类

};


#endif // LISTVIEWDELEGATE_H
