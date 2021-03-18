#include "about.hpp"

About::About(QWidget *parent) : QDialog(parent)
{
    buildWindow();
    buildComponents();
    applyLayout();
    adjustSize();
    connect(okBtn,&QPushButton::clicked,this,[&]() -> void {
        close();
    });
}

void About::buildComponents()
{
    iconLabel = new QLabel(this);
    label     = new QLabel(this);
    okBtn     = new QPushButton("Ok",this);
    iconLabel->setPixmap(QIcon(":assets/notepad.ico").pixmap(48, 48));
    label->setTextFormat(Qt::RichText);
    label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    label->setWordWrap(true);
    label->setLineWidth(200);
    label->setText(INFO);
}

void About::applyLayout()
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addWidget(iconLabel,2);
    lay->addWidget(label,6);
    lay->addWidget(okBtn,2);
    lay->setAlignment(Qt::AlignVCenter);
}

void About::buildWindow()
{
    setWindowTitle( tr("About QNotePad"));
    setFixedSize(250,250);
    setWindowIcon(QIcon(":assets/notepad.ico"));
}
