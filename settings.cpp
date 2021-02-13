#include "settings.hpp"

Settings::Settings(QWidget *parent,const QList<QString> &themes,uint8_t tabspace) : QDialog(parent)
{
    setWindowTitle("Settings");
    setFixedSize(400,300);
    setWindowIcon(QIcon(":assets/settings.ico"));
    buildElements(themes,tabspace);
    applyLayout();
    connect(ok,&QPushButton::clicked,this,[this](){close();});
    connect(cancel,&QPushButton::clicked,this,[this,tabspace](){
        spinTab->setValue(tabspace);
        close();
    });
    connect(loadUserTheme,&QPushButton::clicked,this,&Settings::localThemeLoader);
    connect(themeChange,&QComboBox::currentTextChanged,this,[this](){emit themeChanged(themeChange->currentText());});
    connect(spinTab,QOverload<int>::of(&QSpinBox::valueChanged),this,[=](int i){emit changeTabWidth(i);});
}

void Settings::buildElements(const QList<QString> &themes,uint8_t tabspace)
{
    tabSpaceIndication = new QLabel("Tab width : ",this);
    spinTab            = new QSpinBox(this);
    themeIndication    = new QLabel("Theme : ",this);
    themeChange        = new QComboBox(this);
    ok                 = new QPushButton("Ok",this);
    cancel             = new QPushButton("Cancel",this);
    loadUserTheme      = new QPushButton("Load .qss file theme",this);
    spinTab->setRange(1,10);
    spinTab->setValue(tabspace);
    foreach(auto const tmpTheme,themes)
    {
        themeChange->addItem(tmpTheme);
    }
}

void Settings::applyLayout()
{
    QGridLayout *lay = new QGridLayout(this);
    lay->addWidget(tabSpaceIndication,0,0);
    lay->addWidget(spinTab,0,1);
    lay->addWidget(themeIndication,1,0);
    lay->addWidget(themeChange,1,1);
    lay->addWidget(loadUserTheme);
    lay->addWidget(ok,3,0);
    lay->addWidget(cancel,3,1);
    setLayout(lay);
}

void Settings::localThemeLoader()
{
    auto fileToLoad = QFileDialog::getOpenFileName(this,"Load a local qss file","/home","*.qss");
    emit localThemeSelected(fileToLoad);
    close();
}
