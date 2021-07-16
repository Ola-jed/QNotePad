#include "popup.hpp"

Popup::Popup(QWidget *parent, const QStringList &content) : QMenu(parent)
{
    for(auto const &item : qAsConst(content))
    {
        addAction(item);
    }
    setActiveAction(actions()[0]);
    connect(this,&QMenu::triggered,this,[this](QAction *act){
        emit textSelected(act->text());
    });
}

/// Handle the keyboard event in the popup window for autocompletion
/// Switch between the popup choices/close the popup/ validate the choice
/// \param e
void Popup::keyPressEvent(QKeyEvent *e)
{
    const auto triggeredKey{e->key()};
    switch (triggeredKey)
    {
        case Qt::Key_Down:
            if(actions().indexOf(activeAction()) + 1 < actions().size())
            {
                setActiveAction(actions().at(actions().indexOf(activeAction())+1));
            }
            break;
        case Qt::Key_Up:
            if(actions().indexOf(activeAction()) - 1 >= 0)
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
            const auto pressed{e->text().at(0)};
            if(pressed.isLetterOrNumber())
            {
                emit charCancel(pressed);
            }
            close();
    }
}