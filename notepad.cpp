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
    colorText        = new QAction(QIcon("assets/color.ico"),"Color",this);
    colorBackground  = new QAction(QIcon("assets/background-color"),"Background Color",this);
    fontChange       = new QAction(QIcon("assets/font.ico"),"Font",this);
    terminal         = new QAction(QIcon("assets/terminal.ico"),"",this);
    tabView          = new QTabWidget(this);
    menuBar          = new QMenuBar(this);
    autoSaveCheckBox = new QCheckBox("AutoSave",this);
    positionBar      = new QStatusBar(this);

    // Shortcuts.
    newFile->setShortcut(QKeySequence::New);
    openFile->setShortcut(QKeySequence::Open);
    saveFile->setShortcut(QKeySequence::Save);
    quit->setShortcut(QKeySequence::Quit);

    // Creating a blank plaintextedit.
    tabView->setTabsClosable(true);
    tabView->addTab(new QPlainTextEdit(this),"New File");

    file->addAction(newFile);
    file->addAction(openFile);
    file->addAction(saveFile);
    file->addAction(quit);
    custom->addAction(colorText);
    custom->addAction(colorBackground);
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
    vboxLayout->addWidget(tabView,18);
    vboxLayout->addWidget(positionBar,1);
    auto central = new QWidget(this);
    central->setLayout(vboxLayout);
    setCentralWidget(central);

    // Style.
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon("assets/notepad.ico"));
    setStyleSheet("QMenu{background-color: rgb(28, 49, 80);}QLabel{color:#27fff8;} QMenuBar{spacing: 3px;}"
        "QMenuBar::item {padding: 1px 4px;background: transparent;border-radius: 4px;}"
        "QMenuBar::item:selected {background: #a8a8a8;} QMenuBar::item:pressed {background: #888;}");
    tabView->setStyleSheet("QTabWidget::pane{border-top: 2px solid #C2C7CB;}"
        "QTabWidget::tab-bar {left: 5px;}"
        "QTabBar::tab {background:#0E3146;border:2px solid #C4C4C3;border-bottom-color:#C2C7CB;border-top-left-radius:4px;border-top-right-radius:4px;min-width: 8ex;padding: 2px;}"
        "QTabBar::tab:selected{background: #343435;} QTabBar::tab:hover{background: #626263;}"
        "QTabBar::tab:selected{border-color: #9B9B9B;border-bottom-color: #C2C7CB;}"
        "QTabBar::tab:!selected {margin-top: 2px;}"
        "QTabBar::tab:selected {margin-left: -4px;margin-right: -4px;}"
        "QTabBar::tab:first:selected{margin-left: 0;}"
        "QTabBar::tab:last:selected{margin-right: 0;}"
        "QTabBar::tab:only-one{margin: 0;}");
    tabView->setElideMode(Qt::ElideRight);

    connect(quit,&QAction::triggered,this,&Notepad::onQuit);
    connect(newFile,&QAction::triggered,this,&Notepad::onNewFile);
    connect(openFile,&QAction::triggered,this,&Notepad::onOpenFile);
    connect(saveFile,&QAction::triggered,this,&Notepad::onSaveFile);
    connect(terminal,&QAction::triggered,this,&Notepad::onTerminal);
    connect(fontChange,&QAction::triggered,this,&Notepad::onFont);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateTitle);
    connect(colorText,&QAction::triggered,this,&Notepad::onColorChanged);
    connect(colorBackground,&QAction::triggered,this,&Notepad::onBackgroundColorChanged);
    connect(tabView,&QTabWidget::tabCloseRequested,this,&Notepad::onCloseFile);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateConnect);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::onTextModified);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::cursorPositionChanged,this,&Notepad::updateCursorPosition);
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
            qobject_cast<QPlainTextEdit*>(tabView->widget(getIndex(filename)))->setPlainText(" ");
            // Reading the file line by line and storing int the textEdit.
            QTextStream in{&fichier};
            QString tempLine;
            while(!in.atEnd())
            {
                tempLine = in.readLine();
                qobject_cast<QPlainTextEdit*>(tabView->widget(getIndex(filename)))->appendPlainText(tempLine);
            }
        }
    }
}

void Notepad::onSaveFile()
{
    if(fileName().isEmpty() && !(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty())) // If this is a new file
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
                setWindowTitle("QNotePad");
                tabView->setTabText(tabView->currentIndex(),filename);
            }
        }
    }
    else if((!fileName().isEmpty()) && !(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty()))
    {
        QFile fich{fileName()};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            QTextStream out{&fich};
            out << qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText() << "\n";
            setWindowTitle("QNotePad");
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
    if(tabView->tabText(index).isEmpty() && !(qobject_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText().isEmpty())) // If this is a new file
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
    else if((!tabView->tabText(index).isEmpty()) && !(qobject_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText().isEmpty()))
    {
        QFile fich{tabView->tabText(index)};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            QTextStream out{&fich};
            out << qobject_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText() << "\n";
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
    else if(((!fileName().isEmpty()) && !isSaved && !(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty()))
            ||(fileName().isEmpty() &&!(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty())))
    {
        auto reply = QMessageBox::question(this, "Quitter", "Voulez vous sauvegarder ?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            onSaveFile();
        }
    }
    qApp->quit();
}

// Color.
void Notepad::onColorChanged() // Get the color and set the color in the textEdit.
{
    if(isEmpty()) return; // Do not try anything if the editor is empty
    QColor chosenColor = QColorDialog::getColor("Choisir une couleur");
    QString colorToSet = QString::number(chosenColor.red())+","+QString::number(chosenColor.green())+","+QString::number(chosenColor.blue());
    qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->setStyleSheet(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->styleSheet()+" color:rgb("+colorToSet+");");
}

void Notepad::onBackgroundColorChanged()
{
    if(isEmpty()) return; // Do not try anything if the editor is empty
    QColor chosenColor = QColorDialog::getColor("Choisir une couleur");
    QString colorToSet = QString::number(chosenColor.red())+","+QString::number(chosenColor.green())+","+QString::number(chosenColor.blue());
    qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->setStyleSheet(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->styleSheet()+" background:rgb("+colorToSet+");");
}

void Notepad::onFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Times", 12), this);
    if(ok)
    {
        qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->setFont(font);
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
            out << qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText() << "\n";
            isSaved = true;
            setWindowTitle("QNotePad");
        }
    }
}

void Notepad::onTextModified()
{
    if(!autoSaveCheckBox->isChecked())
    {
        setWindowTitle("QNotePad (file modified)");
        isSaved = false;
    }
}

void Notepad::onTerminal()
{
    QString exec;
    #if (defined (_WIN32) || defined (_WIN64))
        exec = "cmd.exe";
    #elif (defined (LINUX) || defined (__linux__))
        exec = "gnome-terminal";
    #endif
    if((tabView->count() > 0) && (tabView->currentIndex() >= 0))
    {
        QDir fileDir{QFileInfo(fileName()).absoluteDir()};
        QString path      = fileDir.absolutePath();
        QProcess *process = new QProcess(this);
        process->setWorkingDirectory(path);
        process->start(exec,QStringList{});
    }
    else
    {
        QProcess *process = new QProcess(this);
        process->setWorkingDirectory(QDir::home().absolutePath());
        process->start(exec,QStringList{});
    }
}

void Notepad::updateTitle()
{
    setWindowTitle(tabView->tabText(tabView->currentIndex()));
}

void Notepad::updateCursorPosition()
{
    int line    = qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->textCursor().blockNumber() + 1 ;
    int columnn = qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->textCursor().positionInBlock();
    positionBar->showMessage("Line : "+QString::number(line)+" col : "+QString::number(columnn));
}

void Notepad::updateConnect()
{
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::onTextModified);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::cursorPositionChanged,this,&Notepad::updateCursorPosition);
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
