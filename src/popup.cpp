#include "popup.hpp"

Popup::Popup(QWidget *parent, const QStringList &content) : QMenu(parent)
{
    foreach(auto const item,content)
    {
        addAction(item);
    }
    setActiveAction(actions()[0]);
    connect(this,&QMenu::triggered,this,[this](QAction *act){
        emit textSelected(act->text());
    });
}

void Popup::keyPressEvent(QKeyEvent *e)
{
    const auto triggeredKey{e->key()};
    switch (triggeredKey)
    {
        case Qt::Key_Down:
            if(actions().size() > actions().indexOf(activeAction())+1)
            {
                setActiveAction(actions().at(actions().indexOf(activeAction())+1));
            }
            break;
        case Qt::Key_Up:
            if(0 <= actions().indexOf(activeAction())-1)
            {
                setActiveAction(actions().at(actions().indexOf(activeAction())-1));
            }
            break;
        case Qt::Key_Return:
        case Qt::Key_Tab:
            if(activeAction() != nullptr)
            {
                emit triggered(activeAction());
            }
            close();
            break;
        default:
            const QChar pressed{e->text().at(0)};
            if(pressed.isLetterOrNumber())
            {
                emit charCancel(pressed);
            }
            close();
    }
}