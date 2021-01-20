#include "notepad.hpp"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
{
    buildComponents();
    buildThemeList();
    buildMenu();
    // Creating a blank plaintextedit.
    tabView->setTabsClosable(true);
    tabView->setAcceptDrops(true);
    tabView->addTab(new QPlainTextEdit(this),"New File");
    setTabSpace();
    applyLayout();
    applyStyle();
    setAcceptDrops(true);
    // Connecting signals - slots .
    connect(themeChoice,&QComboBox::currentTextChanged,this,&Notepad::onApplyOtherTheme);
    connect(quit,&QAction::triggered,this,&Notepad::onQuit);
    connect(newFile,&QAction::triggered,this,&Notepad::onNewFile);
    connect(openFile,&QAction::triggered,this,&Notepad::openFileDialog);
    connect(saveFile,&QAction::triggered,this,&Notepad::onSaveFile);
    connect(terminal,&QAction::triggered,this,&Notepad::onTerminal);
    connect(fontChange,&QAction::triggered,this,&Notepad::onFont);
    connect(colorText,&QAction::triggered,this,&Notepad::onColorChanged);
    connect(colorBackground,&QAction::triggered,this,&Notepad::onBackgroundColorChanged);
    connect(tabView,&QTabWidget::tabCloseRequested,this,&Notepad::onCloseFile);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateConnect);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateTitle);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::setTabSpace);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::onTextModified);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::synthaxicHighlighting);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::cursorPositionChanged,this,&Notepad::updateCursorPosition);
}

// Creationg the items.
void Notepad::buildComponents()
{
    file             = new QMenu("File",this);
    custom           = new QMenu("Customize",this);
    newFile          = new QAction(QIcon("assets/new.ico"),"New",this);
    openFile         = new QAction(QIcon("assets/open.ico"),"Open",this);
    saveFile         = new QAction(QIcon("assets/save.ico"),"Save",this);
    quit             = new QAction(QIcon("assets/quit.ico"),"Quit",this);
    colorText        = new QAction(QIcon("assets/color.ico"),"Color",this);
    colorBackground  = new QAction(QIcon("assets/background-color"),"Background Color",this);
    highlightSynthax = new QAction(QIcon("assets/highlight.ico"),"Synthax Highlighting",this);
    fontChange       = new QAction(QIcon("assets/font.ico"),"Font",this);
    terminal         = new QAction(QIcon("assets/terminal.ico"),"",this);
    tabView          = new QTabWidget(this);
    menuBar          = new QMenuBar(this);
    autoSaveCheckBox = new QCheckBox("AutoSave",this);
    themeChoice      = new QComboBox(this);
    positionBar      = new QStatusBar(this);
}

// Building the menu
void Notepad::buildMenu()
{
    // Shortcuts.
    newFile->setShortcut(QKeySequence::New);
    openFile->setShortcut(QKeySequence::Open);
    saveFile->setShortcut(QKeySequence::Save);
    quit->setShortcut(QKeySequence::Quit);
    file->addAction(newFile);
    file->addAction(openFile);
    file->addAction(saveFile);
    file->addAction(quit);
    custom->addAction(colorText);
    custom->addAction(colorBackground);
    custom->addAction(highlightSynthax);
    custom->addAction(fontChange);
    menuBar->addMenu(file);
    menuBar->addSeparator();
    menuBar->addMenu(custom);
    menuBar->addSeparator();
    menuBar->addAction(terminal);
    highlightSynthax->setCheckable(true);
    highlightSynthax->setChecked(true);
}

// Building the qcombobox theme list.
void Notepad::buildThemeList()
{
    themeChoice->addItem("Aqua");
    themeChoice->addItem("Amoled");
    themeChoice->addItem("Console");
    themeChoice->addItem("Diffness");
    themeChoice->addItem("Elegant Dark");
    themeChoice->addItem("Mac");
    themeChoice->addItem("Manjaro");
    themeChoice->addItem("Material Dark");
    themeChoice->addItem("Obit");
    themeChoice->addItem("Ubuntu");
    themeChoice->setCurrentIndex(0);
}

// Setup the layout
void Notepad::applyLayout()
{
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(menuBar,5);
    topLayout->addWidget(themeChoice,1);
    topLayout->addWidget(autoSaveCheckBox,2);
    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->setContentsMargins(10,0,10,0);
    vboxLayout->addLayout(topLayout,1);
    vboxLayout->addWidget(tabView,28);
    vboxLayout->addWidget(positionBar,1);
    auto central = new QWidget(this);
    central->setLayout(vboxLayout);
    setCentralWidget(central);
}

// Style.
void Notepad::applyStyle()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon("assets/notepad.ico"));
    setStyleSheet(Aqua);
    tabView->setElideMode(Qt::ElideRight);
}

// New File
void Notepad::onNewFile()
{
    auto filename = QFileDialog::getSaveFileName(this);
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"Nouveau Fichier","Entrer un nom valide");
    }
    else
    {
        createFile(filename);
    }
}

// Create  new file
void Notepad::createFile(const QString &fileToCreate)
{
    QFile fichier{fileToCreate};
    if((!fichier.open(QIODevice::ReadWrite)))
    {
        QMessageBox::critical(this,"Nouveau Fichier","Impossible de créer le fichier");
        return;
    }
    else
    {
        QMessageBox::information(this,"Nouveau Fichier","Le fichier a bien été créé");
        auto fileContent = new QPlainTextEdit(this);
        tabView->addTab(fileContent,fileToCreate);
        tabView->setTabText(getIndex(fileToCreate),fileToCreate);
    }
}

// Dialog for opening a file
void Notepad::openFileDialog()
{
    auto filename = QFileDialog::getOpenFileName(this);
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"Nouveau Fichier","Choisir fichier valide");
        return;
    }
    else
    {
        onOpenFile(filename);
    }
}

// Opening a file.
void Notepad::onOpenFile(const QString &filename)
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
        // Reading the file line by line and storing in the textEdit.
        QTextStream in{&fichier};
        QString fileContent{in.readAll()};
        fichier.close();
        qobject_cast<QPlainTextEdit*>(tabView->widget(getIndex(filename)))->setPlainText(fileContent);
    }
}

// Saving the current file.
void Notepad::onSaveFile()
{
    if((fileName() == "New File") && !(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty())) // If this is a new file
    {
        onNewFileSave();
    }
    else if((!fileName().isEmpty()) && !(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty()))
    {
        onExistingFileSave();
    }
}

// Saving a new file.
void Notepad::onNewFileSave()
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
            QTextStream out{&fichier};
            out << qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText();
            setWindowTitle("QNotePad");
            tabView->setTabText(tabView->currentIndex(),filename);
        }
    }
}

// Saving an existing file.
void Notepad::onExistingFileSave()
{
    QFile fich{fileName()};
    if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
    {
        QTextStream out{&fich};
        out << qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText();
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

// AutoSave of the file
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

// Close the current file.
void Notepad::onCloseFile(const int &index)
{
    if(index < 0) return;
    QWidget* tabItem = tabView->widget(index);
    if(tabView->tabText(index).isEmpty() && !(qobject_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText().isEmpty())) // If this is a new file
    {
        onNewFileSave();
    }
    else if((!tabView->tabText(index).isEmpty()) && !(qobject_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText().isEmpty()))
    {
        onExistingFileSave();
    }
    tabView->removeTab(index);
    delete(tabItem);
    tabItem = nullptr;
}

void Notepad::onTextModified()
{
    if(!autoSaveCheckBox->isChecked())
    {
        setWindowTitle("QNotePad (file modified)");
        isSaved = false;
    }
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
    qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->setStyleSheet(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->styleSheet()+" color:rgb("+colorDialog()+");");
}

void Notepad::onBackgroundColorChanged()
{
    if(isEmpty()) return; // Do not try anything if the editor is empty
    qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->setStyleSheet(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->styleSheet()+" background:rgb("+colorDialog()+");");
}

QString Notepad::colorDialog()
{
    QColor chosenColor = QColorDialog::getColor("Choisir une couleur");
    QString colorToSet = QString::number(chosenColor.red())+","+QString::number(chosenColor.green())+","+QString::number(chosenColor.blue());
    return colorToSet;
}

// Applying the new theme withn the name
void Notepad::onApplyOtherTheme(QString theme)
{
    setStyleSheet(THEME_NAMES[theme]);
}

// Font customization
void Notepad::onFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Times", 12), this);
    if(ok)
    {
        qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->setFont(font);
    }
}

// Synthax Highlighting module
void Notepad::synthaxicHighlighting()
{
    for(auto i = 0; i != qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->blockCount(); i++)
    {
        QTextBlock block = qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->document()->findBlockByLineNumber(i);
        if(block.isValid())
        {
            QList<QTextEdit::ExtraSelection> extraSelections {qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->extraSelections()};
            QString text = block.text();
            foreach(auto highlight,keywordsList)
            {
                int p;
                if(((p = text.indexOf(highlight)) != -1) && (text.mid(p,highlight.length()+1) == highlight+" "))
                {
                    int pos = block.position() + p;
                    QTextEdit::ExtraSelection selection{};
                    selection.cursor = QTextCursor(qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->document());
                    selection.cursor.setPosition( pos );
                    selection.cursor.setPosition( pos+highlight.length(), QTextCursor::KeepAnchor );
                    selection.format.setForeground(Qt::red);
                    extraSelections.append(selection);
                    qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->setExtraSelections(extraSelections);
                }
            }
        }
    }
}

// Open a terminal in the current directory, it depends on the os
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

// Set the tab space of the QPlainTextEdit
void Notepad::setTabSpace()
{
    QFontMetrics metrics(qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->font());
    qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->setTabStopDistance(TAB_SPACE * metrics.horizontalAdvance(' '));
}

// Update the window title
void Notepad::updateTitle()
{
    setWindowTitle(tabView->tabText(tabView->currentIndex()));
}

// Update and show the cursor position in the status bar
void Notepad::updateCursorPosition()
{
    int line    = qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->textCursor().blockNumber() + 1 ;
    int columnn = qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->textCursor().positionInBlock() + 1;
    positionBar->showMessage("Line : "+QString::number(line)+" col : "+QString::number(columnn));
}

// Update the slots connexion when the tab is changed
void Notepad::updateConnect()
{
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::synthaxicHighlighting);
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

// Get the index with the current filename
int Notepad::getIndex(const QString &tabName)
{
    for(auto i = 0;i <= tabView->count();i++)
    {
        if(tabView->tabText(i) == tabName)
        {
            return i;
        }
    }
    return 1;
}

bool Notepad::isEmpty()
{
    return (tabView->count() == 0);
}

// Drag event to open files.
void Notepad::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

// Drop event to open files.
void Notepad::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    // Check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        QString filename;
        QList<QUrl> urlList = mimeData->urls();
        // Extract the local paths of the files.
        // This code is only valid in linux because of the path . Needs to be adapted on windows
        filename = urlList[0].toString().right(urlList[0].toString().length() - 7);
        onOpenFile(filename);
    }
}

Notepad::~Notepad()
{}
