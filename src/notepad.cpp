#include "notepad.hpp"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
{
    newFile    = new QPushButton(this);
    openFile   = new QPushButton(this);
    saveFile   = new QPushButton(this);
    quit       = new QPushButton(this);
    colorText  = new QPushButton(this);
    tabView    = new QTabWidget(this);

    tabView->setTabsClosable(true);
    autoSaveCheckBox        = new QCheckBox("AutoSave",this);

    QHBoxLayout *hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(newFile);
    hboxLayout->addWidget(openFile);
    hboxLayout->addWidget(saveFile);
    hboxLayout->addWidget(colorText);
    hboxLayout->addWidget(quit);
    hboxLayout->addWidget(autoSaveCheckBox);
    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->addLayout(hboxLayout,1);
    vboxLayout->addWidget(tabView,9);
    auto central = new QWidget(this);
    central->setLayout(vboxLayout);
    setCentralWidget(central);

    setWindowIcon(QIcon("assets/notepad.ico"));
    newFile->setIcon(QIcon("assets/new.ico"));
    saveFile->setIcon(QIcon("assets/save.ico"));
    openFile->setIcon(QIcon("assets/open.ico"));
    colorText->setIcon(QIcon("assets/color.ico"));
    quit->setIcon(QIcon("assets/quit.ico"));
    setStyleSheet("QPushButton{background-color: rgb(28, 49, 80);color:#fff;}QLabel{color:#27fff8;}");
    tabView->setStyleSheet("QTabBar::tab{width:100px;background:#0E3146;height:25px;border-style:solid;border-color:#898989;border-width:2px} QTabBar::tab:selected{background:#343435;}");
    tabView->setElideMode(Qt::ElideRight);

    connect(quit,&QPushButton::clicked,this,&Notepad::onQuit);
    connect(newFile,&QPushButton::clicked,this,&Notepad::onNewFile);
    connect(openFile,&QPushButton::clicked,this,&Notepad::onOpenFile);
    connect(saveFile,&QPushButton::clicked,this,&Notepad::onSaveFile);
    connect(static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex())),&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateTitle);
    connect(colorText,&QPushButton::clicked,this,&Notepad::onColorChanged);
    connect(tabView,&QTabWidget::tabCloseRequested,this,&Notepad::onCloseFile);
    connect(static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex())),&QPlainTextEdit::textChanged,this,&Notepad::onTextModified);
}

void Notepad::onNewFile()
{
    auto filename = QFileDialog::getSaveFileName(this);
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"Nouveau Fichier","Entrer un nom valide");
    }
    else
    {
        QFile fichier{filename};
        if((!fichier.open(QIODevice::ReadWrite)))
        {
            QMessageBox::critical(this,"Nouveau Fichier","Impossible de créer le fichier");
            return;
        }
        else
        {
            QMessageBox::information(this,"Nouveau Fichier","Le fichier a bien été créé");
            auto fileContent = new QPlainTextEdit(this);
            tabView->addTab(fileContent,filename);
            tabView->setTabText(getIndex(filename),filename);
        }
    }
}

void Notepad::onOpenFile()
{
    auto filename = QFileDialog::getOpenFileName(this);
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"Nouveau Fichier","Choisir fichier valide");
        return;
    }
    else
    {
        QFile fichier{filename};
        if((!fichier.open(QIODevice::ReadWrite)))
        {
            QMessageBox::critical(this,"Nouveau Fichier","Impossible d'ouvrir le fichier");
            return;
        }
        else
        {
            auto tab = new QPlainTextEdit(this);
            tabView->addTab(tab,filename);
            static_cast<QPlainTextEdit*>(tabView->widget(getIndex(filename)))->setPlainText(" ");
            // Reading the file line by line and storing int the textEdit.
            QTextStream in{&fichier};
            QString tempLine;
            while(!in.atEnd())
            {
                tempLine = in.readLine();
                static_cast<QPlainTextEdit*>(tabView->widget(getIndex(filename)))->appendPlainText(tempLine);
            }
        }
    }
}

void Notepad::onSaveFile()
{
    if(fileName().isEmpty() && !(static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty())) // If this is a new file
    {
        auto filename = QFileDialog::getSaveFileName(this);
        if(filename.isEmpty())
        {
            QMessageBox::warning(this,"Nouveau Fichier","Entrer un nom valide");
        }
        else
        {
            QFile fichier{filename};
            if((!fichier.open(QIODevice::ReadWrite)))
            {
                QMessageBox::critical(this,"Sauvegarder","Impossible de sauvegarder le fichier");
                return;
            }
            else
            {
                tabView->setTabText(tabView->currentIndex(),filename);
            }
        }
    }
    else if((!fileName().isEmpty()) && !(static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty()))
    {
        QFile fich{fileName()};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            QTextStream out{&fich};
            out << static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText() << "\n";
            isSaved = true;
            QMessageBox::information(this,"Sauvegarde","Sauvegarde Réussie");
        }
        else
        {
            QMessageBox::critical(this,"Sauvegarde","Impossible de sauvegarder");
            return;
        }
    }
}

void Notepad::onCloseFile(const int &index)
{
    if(index < 0) return;
    QWidget* tabItem = tabView->widget(index);
    if(tabView->tabText(index).isEmpty() && !(static_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText().isEmpty())) // If this is a new file
    {
        auto filename = QFileDialog::getSaveFileName(this);
        if(filename.isEmpty())
        {
            QMessageBox::warning(this,"Nouveau Fichier","Entrer un nom valide");
        }
        else
        {
            QFile fichier{filename};
            if((!fichier.open(QIODevice::ReadWrite)))
            {
                QMessageBox::critical(this,"Sauvegarder","Impossible de sauvegarder le fichier");
            }
        }
    }
    else if((!tabView->tabText(index).isEmpty()) && !(static_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText().isEmpty()))
    {
        QFile fich{tabView->tabText(index)};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            QTextStream out{&fich};
            out << static_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText() << "\n";
        }
        else
        {
            QMessageBox::critical(this,"Sauvegarde","Impossible de sauvegarder");
        }
    }
    tabView->removeTab(index);
    delete(tabItem);
    tabItem = nullptr;
}

void Notepad::onQuit()
{
    if(isEmpty())
        qApp->quit();
    else if(((!fileName().isEmpty()) && !isSaved && !(static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty()))
            ||(fileName().isEmpty() &&!(static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty())))
    {
        auto reply = QMessageBox::question(this, "Quitter", "Voulez vous sauvegarder ?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            onSaveFile();
        }
    }
    qApp->quit();
}

void Notepad::onColorChanged() // Get the color and set the color in the textEdit.
{
    if(isEmpty()) return; // Do not try anything if the editor is empty
    QColor chosenColor = QColorDialog::getColor("Choisir une couleur");
    QString colorToSet = QString::number(chosenColor.red())+","+QString::number(chosenColor.green())+","+QString::number(chosenColor.blue());
    static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->setStyleSheet("color:rgb("+colorToSet+")");
}

void Notepad::onAutoSave()
{
    if((autoSaveCheckBox->isChecked()) && (!fileName().isEmpty()))
    {
        QFile fich{fileName()};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            QTextStream out{&fich};
            out << static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText() << "\n";
            isSaved = true;
        }
    }
}

void Notepad::onTextModified()
{
    if(!autoSaveCheckBox->isChecked())
    {
        isSaved = false;
    }
}

void Notepad::updateTitle()
{
    setWindowTitle(tabView->tabText(tabView->currentIndex()));
}

QString Notepad::fileName()
{
    if(tabView->count() == 0)
        return " ";
    return tabView->tabText(tabView->currentIndex());
}

int Notepad::getIndex(const QString &tabName)
{
    for(auto i =0;i<= tabView->count();i++)
    {
        if(tabView->tabText(i) == tabName)
            return i;
    }
    return 1;
}

bool Notepad::isEmpty()
{
    return (tabView->count() == 0);
}

Notepad::~Notepad()
{}
