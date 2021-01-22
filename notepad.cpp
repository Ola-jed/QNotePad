#include "notepad.hpp"

Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
{
    buildComponents();
    buildMenu();
    buildFileView();
    tabView->setTabsClosable(true);
    tabView->setAcceptDrops(true);
    // Creating a blank plaintextedit.
    tabView->addTab(new QPlainTextEdit(this),"New File");
    setTabSpace();
    applyLayout();
    applyStyle();
    checkFileLanguage();
    setAcceptDrops(true);
    makeConnections();
}

// Connections.
void Notepad::makeConnections()
{
    connect(settings,&QAction::triggered,this,&Notepad::onSettings);
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
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateFileView);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::setTabSpace);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::checkFileLanguage);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::onTextModified);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::textChanged,this,&Notepad::synthaxicHighlighting);
    connect(qobject_cast<QPlainTextEdit*>(tabView->currentWidget()),&QPlainTextEdit::cursorPositionChanged,this,&Notepad::updateCursorPosition);
    connect(fileView,&QTreeView::doubleClicked,this,&Notepad::fileViewItemClicked);
    connect(fileModel,&QFileSystemModel::fileRenamed,this,&Notepad::fileRenamed);
}

// Creationg the items.
void Notepad::buildComponents()
{
    fileType         = new QLabel("",this);
    file             = new QMenu("File",this);
    custom           = new QMenu("Customize",this);
    newFile          = new QAction(QIcon(":assets/new.ico"),"New",this);
    openFile         = new QAction(QIcon(":assets/open.ico"),"Open",this);
    saveFile         = new QAction(QIcon(":assets/save.ico"),"Save",this);
    quit             = new QAction(QIcon(":assets/quit.ico"),"Quit",this);
    colorText        = new QAction(QIcon(":assets/color.ico"),"Color",this);
    colorBackground  = new QAction(QIcon(":assets/background-color"),"Background Color",this);
    highlightSynthax = new QAction(QIcon(":assets/highlight.ico"),"Synthax Highlighting",this);
    fontChange       = new QAction(QIcon(":assets/font.ico"),"Font",this);
    settings         = new QAction(QIcon(":assets/settings.ico"),"Settings",this);
    terminal         = new QAction(QIcon(":assets/terminal.ico"),"",this);
    tabView          = new QTabWidget(this);
    menuBar          = new QMenuBar(this);
    autoSaveCheckBox = new QCheckBox("AutoSave",this);
    positionBar      = new QStatusBar(this);
}

// Build the treeFileView
void Notepad::buildFileView()
{
    fileModel        = new QFileSystemModel(this);
    fileView         = new QTreeView(this);
    fileModel->setReadOnly(false);
    fileView->setModel(fileModel);
    fileView->hideColumn(1);
    fileView->hideColumn(2);
    fileView->hideColumn(3);
    fileView->setVisible(false);
}

// Update the file view
void Notepad::updateFileView()
{
    fileModel->setRootPath(QFileInfo(fileName()).dir().path());
    QModelIndex idx = fileModel->index(QFileInfo(fileName()).dir().path());
    fileView->setRootIndex(idx);
}

// Open the item when the item is double clicked
void Notepad::fileViewItemClicked(const QModelIndex &index)
{
    auto name = QFileInfo(fileName()).absoluteDir().path()+"/"+index.data().toString();
    if(QFileInfo(name).isFile() && getIndex(name) == -1)
    {
        onOpenFile(QFileInfo(fileName()).absoluteDir().path()+"/"+index.data().toString());
    }
}

// File renamed.
void Notepad::fileRenamed(const QString &path,const QString &oldName,const QString &newName)
{
    QString oldNm = path+"/"+oldName;
    QString newNm = path+"/"+newName;
    QFile::rename(oldNm,newNm);
    tabView->setTabText(getIndex(oldNm),newNm);
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
    custom->addAction(settings);
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

// Setup the layout
void Notepad::applyLayout()
{
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(menuBar,5);
    topLayout->addWidget(autoSaveCheckBox,2);
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(0,0,0,0);
    bottomLayout->addWidget(positionBar,8);
    bottomLayout->addWidget(fileType,2);
    QHBoxLayout *mediumLayout = new QHBoxLayout();
    mediumLayout->addWidget(fileView,2);
    mediumLayout->addWidget(tabView,15);
    QVBoxLayout *vboxLayout = new QVBoxLayout();
    vboxLayout->setContentsMargins(0,0,0,0);
    vboxLayout->addLayout(topLayout,1);
    vboxLayout->addLayout(mediumLayout,28);
    vboxLayout->addLayout(bottomLayout,1);
    auto central = new QWidget(this);
    central->setLayout(vboxLayout);
    setCentralWidget(central);
}

// Style.
void Notepad::applyStyle()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon(":assets/notepad.ico"));
    setStyleSheet(Aqua);
    tabView->setElideMode(Qt::ElideRight);
}

// New File
void Notepad::onNewFile()
{
    auto filename = QFileDialog::getSaveFileName(this);
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"New File","Enter a valid name");
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
        QMessageBox::critical(this,"New File","Cannot create the file");
        return;
    }
    else
    {
        QMessageBox::information(this,"New File","File created");
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
        QMessageBox::warning(this,"New File","Choose a valid file");
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
        QMessageBox::critical(this,"New File","Cannot open the file");
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
        fileModel->setRootPath(QFileInfo(filename).dir().path());
        QModelIndex idx = fileModel->index(QFileInfo(filename).dir().path());
        fileView->setRootIndex(idx);
        fileView->setVisible(true);
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
        QMessageBox::warning(this,"New File","Enter a valid name");
    }
    else
    {
        QFile fichier{filename};
        if((!fichier.open(QIODevice::ReadWrite)))
        {
            QMessageBox::critical(this,"Save","Could not save");
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
    }
    else
    {
        QMessageBox::critical(this,"Save","Could not save");
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
    if(tabView->count() == 1) qApp->quit();
    else
    {
        QWidget* tabItem = tabView->widget(index);
        if(tabView->tabText(index).isEmpty() && !(qobject_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText().isEmpty())) // If this is a new file
        {
            onNewFileSave();
        }
        else if((!tabView->tabText(index).isEmpty()) && !(qobject_cast<QPlainTextEdit*>(tabView->widget(index))->toPlainText().isEmpty()))
        {
            auto rep = QMessageBox::question(this,"Save","Do you want to save ?");
            if(rep == QMessageBox::Yes) onExistingFileSave();
        }
        tabView->removeTab(index);
        delete(tabItem);
        tabItem = nullptr;
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

void Notepad::onQuit()
{
    if(isEmpty())
        qApp->quit();
    else if(((!fileName().isEmpty()) && !isSaved && !(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty()))
            ||(fileName().isEmpty() &&!(qobject_cast<QPlainTextEdit*>(tabView->widget(tabView->currentIndex()))->toPlainText().isEmpty())))
    {
        auto reply = QMessageBox::question(this, "Quit", "Save ?",QMessageBox::Yes|QMessageBox::No);
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
    QColor chosenColor = QColorDialog::getColor("Choose a color");
    QString colorToSet = QString::number(chosenColor.red())+","+QString::number(chosenColor.green())+","+QString::number(chosenColor.blue());
    return colorToSet;
}

// Check the language type
void Notepad::checkFileLanguage()
{
    auto currentFileExtension  = QFileInfo(fileName()).completeSuffix();
    QString currentFileType    = "Plain Text";
    QMapIterator<QString,QStringList> iteratorMap{FILE_EXTENSIONS};
    while (iteratorMap.hasNext())
    {
        auto elmt    = iteratorMap.next();
        auto content = elmt.value();
        if(content.contains(currentFileExtension)) currentFileType = iteratorMap.key();
    }
    fileType->setText(currentFileType);
}

// Applying the new theme with the name
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

// Settings
void Notepad::onSettings()
{
    Settings *s = new Settings(this);
    connect(s,&Settings::themeChanged,this,&Notepad::onApplyOtherTheme);
    connect(s,&Settings::changeTabWidth,this,&Notepad::setTabSpace);
    s->show();
}

// Synthax Highlighting module
void Notepad::synthaxicHighlighting()
{
    for(auto i = 0; i != qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->blockCount(); i++)
    {
        QTextBlock block = qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->document()->findBlockByLineNumber(i);
        if(block.isValid())
        {
            applyColoration(block);
        }
    }
}

// Apply coloration
void Notepad::applyColoration(const QTextBlock block)
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
void Notepad::setTabSpace(uint8_t space)
{
    tabSpace = space;
    QFontMetrics metrics(qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->font());
    qobject_cast<QPlainTextEdit*>(tabView->currentWidget())->setTabStopDistance(tabSpace * metrics.horizontalAdvance(' '));
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
    connect(fileView,&QTreeView::doubleClicked,this,&Notepad::fileViewItemClicked);
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
    return -1;
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
