#include "about.hpp"

About::About(QWidget *parent) : QDialog(parent)
{
    buildWindow();
    buildComponents();
    applyLayout();
    adjustSize();
    connect(okBtn,&QPushButton::clicked,[this]{
        close();
    });
}

/// Build the window components
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

/// Build and apply a layout to the window
void About::applyLayout()
{
    auto lay = new QVBoxLayout(this);
    lay->addWidget(iconLabel,2);
    lay->addWidget(label,6);
    lay->addWidget(okBtn,2);
    lay->setAlignment(Qt::AlignVCenter);
}

/// Resize and set title and winow icon
void About::buildWindow()
{
    setWindowTitle( tr("About QNotePad"));
    setFixedSize(250,250);
    setWindowIcon(QIcon(":assets/notepad.ico"));
}