#include "notepad.hpp"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
{
    file             = new QMenu("File",this);
    custom           = new QMenu("Customize",this);
    newFile          = new QAction(QIcon("assets/new.ico"),"New",this);
    openFile         = new QAction(QIcon("assets/open.ico"),"Open",this);
    saveFile         = new QAction(QIcon("assets/save.ico"),"Save",this);
    quit             = new QAction(QIcon("assets/quit.ico"),"Quit",this);
    colorText        = new QAction(QIcon("assets/new.ico"),"Color",this);
    fontChange       = new QAction(QIcon("assets/font.ico"),"Font",this);
    terminal         = new QAction(QIcon("assets/terminal.ico"),"",this);
    tabView          = new QTabWidget(this);
    menuBar          = new QMenuBar(this);
    autoSaveCheckBox = new QCheckBox("AutoSave",this);

    // Shortcuts.
    newFile->setShortcut(QKeySequence::New);
    openFile->setShortcut(QKeySequence::Open);
    saveFile->setShortcut(QKeySequence::Save);
    quit->setShortcut(QKeySequence::Quit);

    tabView->setTabsClosable(true);

    file->addAction(newFile);
    file->addAction(openFile);
    file->addAction(saveFile);
    file->addAction(quit);
    custom->addAction(colorText);
    custom->addAction(fontChange);
    menuBar->addMenu(file);
    menuBar->addSeparator();
    menuBar->addMenu(custom);
    menuBar->addSeparator();
    menuBar->addAction(terminal);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(menuBar,5);
    topLayout->addWidget(autoSaveCheckBox,2);

    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->addLayout(topLayout,1);
    vboxLayout->addWidget(tabView,9);
    auto central = new QWidget(this);
    central->setLayout(vboxLayout);
    setCentralWidget(central);

    setWindowIcon(QIcon("assets/notepad.ico"));
    setStyleSheet("QMenu{background-color: rgb(28, 49, 80);}QLabel{color:#27fff8;} QMenuBar {spacing: 3px;} QMenuBar::item {padding: 1px 4px;background: transparent;border-radius: 4px;} QMenuBar::item:selected {background: #a8a8a8;} QMenuBar::item:pressed {background: #888888;}");
    tabView->setStyleSheet("QTabBar::tab{width:100px;background:#0E3146;height:25px;border-style:solid;border-color:#898989;border-width:2px} QTabBar::tab:selected{background:#343435;}");
    tabView->setElideMode(Qt::ElideRight);

    connect(quit,&QAction::triggered,this,&Notepad::onQuit);
    connect(newFile,&QAction::triggered,this,&Notepad::onNewFile);
    connect(openFile,&QAction::triggered,this,&Notepad::onOpenFile);
    connect(saveFile,&QAction::triggered,this,&Notepad::onSaveFile);
    connect(terminal,&QAction::triggered,this,&Notepad::onTerminal);
    connect(fontChange,&QAction::triggered,this,&Notepad::onFont);
    connect(static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex())),&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateTitle);
    connect(colorText,&QAction::triggered,this,&Notepad::onColorChanged);
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

void Notepad::onFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Times", 12), this);
    if(ok)
    {
        static_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->setFont(font);
    }
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

void Notepad::onTerminal()
{
//    QProcess *term = new QProcess(this);
    if((tabView->count() > 0) && (tabView->currentIndex() >= 0))
    {
        QDir fileDir{QFileInfo(fileName()).absoluteDir()};
        QString path      = fileDir.absolutePath();
        QProcess *process = new QProcess(this);
        QString exec      = "gnome-terminal";
        process->setWorkingDirectory(path);
        process->start(exec,QStringList{});
    }
    else
    {
        QProcess *process = new QProcess(this);
        QString exec = "gnome-terminal";
        process->setWorkingDirectory(QDir::home().absolutePath());
        process->start(exec,QStringList{});
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
{

}
