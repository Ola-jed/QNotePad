#ifndef POPUP_HPP
#define POPUP_HPP

#include <QMenu>
#include <QObject>
#include <QWidget>
#include <QStringList>
#include <QAction>
#include <QKeyEvent>

class Popup : public QMenu
{
    Q_OBJECT
public:
    Popup(QWidget *parent,const QStringList &content);
protected:
    void keyPressEvent(QKeyEvent *e);
signals:
    void textSelected(QString text);
};

#endif // POPUP_HPP
