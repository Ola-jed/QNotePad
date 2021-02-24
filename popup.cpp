#include "popup.hpp"

Popup::Popup(QWidget *parent, const QStringList &content) : QMenu(parent)
{
    foreach(auto const item,content)
    {
        addAction(item);
    }
    connect(this,&QMenu::triggered,this,[this](QAction *act){
        emit textSelected(act->text());
    });
}

void Popup::keyPressEvent(QKeyEvent *e)
{
    auto const triggeredKey{e->key()};
    if(triggeredKey == Qt::Key_Tab)
    {
        if(activeAction() != nullptr)
        {
            emit triggered(activeAction());
        }
    }
    else if((triggeredKey != Qt::Key_Up) && (triggeredKey != Qt::Key_Down))
    {
        // TODO emit key char when canceled
        const QChar pressed{e->text().at(0)};
        if(pressed.isLetterOrNumber())
        {
            emit charCancel(pressed);
        }
        close();
    }
}
