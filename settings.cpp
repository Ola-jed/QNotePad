#include "settings.hpp"

Settings::Settings(QWidget *parent,const QString &themeName) : QDialog(parent)
{
    setWindowTitle("Settings");
    setFixedSize(400,300);
    setWindowIcon(QIcon(":assets/settings.ico"));
    buildElements(themeName);
    applyLayout();
    connect(ok,&QPushButton::clicked,this,[this](){close();});
    connect(cancel,&QPushButton::clicked,this,[this,themeName](){
        themeChange->setCurrentText(themeName);
        spinTab->setValue(4);
        close();
    });
    connect(themeChange,&QComboBox::currentTextChanged,this,[this](){emit themeChanged(themeChange->currentText());});
    connect(spinTab, QOverload<int>::of(&QSpinBox::valueChanged),this,[=](int i){emit changeTabWidth(i);});
}

void Settings::buildElements(const QString &themeName)
{
    tabSpaceIndication = new QLabel("Tab width : ",this);
    spinTab            = new QSpinBox(this);
    themeIndication    = new QLabel("Theme : ",this);
    themeChange        = new QComboBox(this);
    ok                 = new QPushButton("Ok",this);
    cancel             = new QPushButton("Cancel",this);
    spinTab->setRange(1,10);
    spinTab->setValue(4);
    themeChange->addItems({"Adaptic","Amoled","Aqua","Console","Diffness","Dtor","Elegant Dark",
                           "Fibrary","Genetive","Irrorater","Mac","Manjaro","Material Dark","Obit","Ubuntu","World"});
    themeChange->setCurrentText(themeName);
}

void Settings::applyLayout()
{
    QGridLayout *lay = new QGridLayout(this);
    lay->addWidget(tabSpaceIndication,0,0);
    lay->addWidget(spinTab,0,1);
    lay->addWidget(themeIndication,1,0);
    lay->addWidget(themeChange,1,1);
    lay->addWidget(ok,2,0);
    lay->addWidget(cancel,2,1);
    setLayout(lay);
}
