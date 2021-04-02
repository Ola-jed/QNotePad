#include "notepad.hpp"

Notepad::Notepad(QWidget *parent)
        : QMainWindow(parent)
{
    loadSavedThemes();
    buildComponents();
    buildMenu();
    applyShortcuts();
    buildStatusBar();
    buildFileView();
    tabView->setTabsClosable(true);
    tabView->setAcceptDrops(true);
    tabView->addTab(new QPlainTextEdit(this),"New File");
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
    connect(saveFileAs,&QAction::triggered,this,&Notepad::onNewFileSave);
    connect(terminal,&QAction::triggered,this,&Notepad::onTerminal);
    connect(fontChange,&QAction::triggered,this,&Notepad::onFont);
    connect(closeAll,&QAction::triggered,this,&Notepad::closeAllTabs);
    connect(colorText,&QAction::triggered,this,&Notepad::onColorChanged);
    connect(about,&QAction::triggered,this,&Notepad::onAbout);
    connect(colorBackground,&QAction::triggered,this,&Notepad::onBackgroundColorChanged);
    connect(zoomIn,&QAction::triggered,this,&Notepad::zoomPlus);
    connect(zoomOut,&QAction::triggered,this,&Notepad::zoomMinus);
    connect(tabView,&QTabWidget::tabCloseRequested,this,&Notepad::onCloseFile);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateConnect);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateTitle);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::updateFileView);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::onCheckLock);
    connect(tabView,&QTabWidget::currentChanged,this,&Notepad::checkFileLanguage);
    connect(lock,&QPushButton::clicked,this,&Notepad::onApplyLock);
    connect(getCurrent(),&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(getCurrent(),&QPlainTextEdit::textChanged,this,&Notepad::onTextModified);
    connect(getCurrent(),&QPlainTextEdit::textChanged,this,&Notepad::synthaxicHighlighting);
    connect(getCurrent(),&QPlainTextEdit::cursorPositionChanged,this,&Notepad::updateCursorPosition);
    connect(fileView,&QTreeView::doubleClicked,this,&Notepad::fileViewItemClicked);
    connect(fileModel,&QFileSystemModel::fileRenamed,this,&Notepad::fileRenamed);
}

// Creationg the items.
void Notepad::buildComponents()
{
    file             = new QMenu("File",this);
    color            = new QMenu("Color",this);
    edit             = new QMenu("Edit",this);
    view             = new QMenu("Zoom",this);
    newFile          = new QAction(QIcon(":assets/new.ico"),"New",this);
    openFile         = new QAction(QIcon(":assets/open.ico"),"Open",this);
    saveFile         = new QAction(QIcon(":assets/save.ico"),"Save",this);
    saveFileAs       = new QAction(QIcon(":assets/saveas.ico"),"Save as",this);
    closeAll         = new QAction(QIcon(":assets/closeall.ico"),"Close all",this);
    quit             = new QAction(QIcon(":assets/quit.ico"),"Quit",this);
    autoSave         = new QAction(QIcon(":assets/autosave.ico"),"AutoSave",this);
    colorText        = new QAction(QIcon(":assets/color.ico"),"Text Color",this);
    colorBackground  = new QAction(QIcon(":assets/background-color"),"Background color",this);
    highlightSynthax = new QAction(QIcon(":assets/highlight.ico"),"Synthax highlighting",this);
    fontChange       = new QAction(QIcon(":assets/font.ico"),"Font",this);
    settings         = new QAction(QIcon(":assets/settings.ico"),"Settings",this);
    zoomIn           = new QAction(QIcon(":assets/zoomin.ico"),"Zoom in",this);
    zoomOut          = new QAction(QIcon(":assets/zoomout.ico"),"Zoom out",this);
    terminal         = new QAction("Terminal",this);
    about            = new QAction("About",this);
    tabView          = new QTabWidget(this);
    menuBar          = new QMenuBar(this);
}

// Build the treeFileView
void Notepad::buildFileView()
{
    fileModel = new QFileSystemModel(this);
    fileView  = new QTreeView(this);
    fileModel->setReadOnly(false);
    fileView->setModel(fileModel);
    fileView->hideColumn(1);
    fileView->hideColumn(2);
    fileView->hideColumn(3);
    fileView->setVisible(false);
    fileView->setHeaderHidden(true);
    editorSplitter = new QSplitter(Qt::Horizontal,this);
}

// Build the status bar
void Notepad::buildStatusBar()
{
    statusBar         = new QStatusBar(this);
    position          = new QLabel("",this);
    fileType          = new QLabel("",this);
    lock              = new QPushButton(QIcon(":assets/unlock.ico"),"",this);
    tabSpaceIndicator = new QLabel("",this);
    statusBar->addWidget(position,1);
    statusBar->addWidget(fileType,4);
    statusBar->addWidget(tabSpaceIndicator,4);
    statusBar->addWidget(lock,1);
    statusBar->setSizeGripEnabled(false);
}

// Update the file view
void Notepad::updateFileView()
{
    fileModel->setRootPath(QFileInfo(fileName()).dir().path());
    const QModelIndex idx {fileModel->index(QFileInfo(fileName()).dir().path())};
    fileView->setRootIndex(idx);
}

// Open the item when the item is double clicked
void Notepad::fileViewItemClicked(const QModelIndex &index)
{
    auto const currentFileName {fileModel->filePath(index)};
    if(QFileInfo(currentFileName).isFile() && (getIndex(currentFileName) == -1))
    {
        onOpenFile(currentFileName);
    }
}

// File renamed.
void Notepad::fileRenamed(const QString &path,const QString &oldName,const QString &newName)
{
    const QString oldNm {path+"/"+oldName};
    const QString newNm {path+"/"+newName};
    QFile::rename(oldNm,newNm);
    tabView->setTabText(getIndex(oldNm),newNm);
}

// Building the menu
void Notepad::buildMenu()
{
    autoSave->setCheckable(true);
    file->addAction(newFile);
    file->addAction(openFile);
    file->addSeparator();
    file->addAction(saveFile);
    file->addAction(saveFileAs);
    file->addSeparator();
    file->addAction(closeAll);
    file->addAction(quit);
    file->addSeparator();
    file->addAction(autoSave);
    color->addAction(colorText);
    color->addAction(colorBackground);
    edit->addMenu(color);
    edit->addAction(settings);
    edit->addAction(highlightSynthax);
    edit->addAction(fontChange);
    view->addAction(zoomIn);
    view->addAction(zoomOut);
    menuBar->addMenu(file);
    menuBar->addMenu(edit);
    menuBar->addMenu(view);
    menuBar->addAction(terminal);
    menuBar->addAction(about);
    highlightSynthax->setCheckable(true);
    highlightSynthax->setChecked(false);
}

// Menu shortcuts
void Notepad::applyShortcuts()
{
    newFile->setShortcut(QKeySequence::New);
    openFile->setShortcut(QKeySequence::Open);
    saveFile->setShortcut(QKeySequence::Save);
    saveFileAs->setShortcut(QKeySequence::SaveAs);
    quit->setShortcut(QKeySequence::Quit);
    settings->setShortcut(QKeySequence::Preferences);
    zoomIn->setShortcut(QKeySequence::ZoomIn);
    zoomOut->setShortcut(QKeySequence::ZoomOut);
}


// Setup the layout
void Notepad::applyLayout()
{
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(menuBar);
    editorSplitter->addWidget(fileView);
    editorSplitter->addWidget(tabView);
    editorSplitter->setStretchFactor(1,4);
    auto *vboxLayout = new QVBoxLayout();
    vboxLayout->setContentsMargins(0,0,0,0);
    vboxLayout->addLayout(topLayout,1);
    vboxLayout->addWidget(editorSplitter,28);
    vboxLayout->addWidget(statusBar,1);
    auto central = new QWidget(this);
    central->setLayout(vboxLayout);
    setCentralWidget(central);
}

// Style.
void Notepad::applyStyle()
{
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,size(),QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon(":assets/notepad.ico"));
    setTabSpace((notepadSettings.value("Tab width").toInt() == 0) ? 4 : notepadSettings.value("Tab width").toInt()); // Loading tab space
    setStyleSheet(THEME_NAMES[notepadSettings.value("Theme").toString()]); // Loading the user's favourite theme
    tabView->setElideMode(Qt::ElideRight);
}

// New File
void Notepad::onNewFile()
{
    const auto filename {QFileDialog::getSaveFileName(this)};
    if(!filename.isEmpty())
    {
        createFile(filename);
    }
}

// Create  new file
void Notepad::createFile(const QString &fileToCreate)
{
    QFile fToCreate{fileToCreate};
    if((!fToCreate.open(QIODevice::ReadWrite)))
    {
        QMessageBox::critical(this,"New File","Cannot create the file");
        return;
    }
    else
    {
        auto fileContent = new QPlainTextEdit(this);
        tabView->addTab(fileContent,fileToCreate);
        tabView->setCurrentIndex(getIndex(fileToCreate));
        tabView->setTabText(getIndex(fileToCreate),fileToCreate);
    }
}

// Dialog for opening a file
void Notepad::openFileDialog()
{
    auto const filename {QFileDialog::getOpenFileName(this)};
    if(filename.isEmpty())
    {
        QMessageBox::warning(this,"New File","Choose a valid file");
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
    if((!fichier.open(QIODevice::ReadOnly)))
    {
        QMessageBox::critical(this,"New File","Cannot open the file");
        return;
    }
    else
    {
        auto tab = new QPlainTextEdit(this);
        tabView->addTab(tab,filename);
        tabView->setCurrentIndex(getIndex(filename));
        // Reading the file line by line and storing in the textEdit.
        QTextStream in{&fichier};
        const QString fileContent{in.readAll()};
        fichier.close();
        getCurrent()->setPlainText(fileContent);
        fileModel->setRootPath(QFileInfo(filename).dir().path());
        const QModelIndex idx {fileModel->index(QFileInfo(filename).dir().path())};
        fileView->setRootIndex(idx);
        fileView->setVisible(true);
    }
}

// Saving the current file.
void Notepad::onSaveFile()
{
    if((fileName() == "New File") && !(getCurrent()->toPlainText().isEmpty()))
    {
        onNewFileSave();
    }
    else if((!fileName().isEmpty()) && !(getCurrent()->toPlainText().isEmpty()))
    {
        onExistingFileSave();
    }
}

// Saving a new file.
void Notepad::onNewFileSave()
{
    auto const filename {QFileDialog::getSaveFileName(this)};
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
            out << getCurrent()->toPlainText();
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
        out << getCurrent()->toPlainText();
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
    if((autoSave->isChecked()) && (!fileName().isEmpty()))
    {
        QFile fich{fileName()};
        if(fich.open(QIODevice::ReadWrite|QFile::Truncate))
        {
            QTextStream out{&fich};
            out << getCurrent()->toPlainText();
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
    if(!autoSave->isChecked())
    {
        setWindowTitle("QNotePad (file modified)");
        isSaved = false;
    }
}

void Notepad::onQuit()
{
    if(isEmpty())
    {
        qApp->quit();
    }
    else if(((!fileName().isEmpty()) && !isSaved && !(getCurrent()->toPlainText().isEmpty())) ||(fileName().isEmpty() &&!(getCurrent()->toPlainText().isEmpty())))
    {
        auto const reply = QMessageBox::question(this, "Quit", "Save ?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            onSaveFile();
        }
    }
    qApp->quit();
}

// Color.
void Notepad::onColorChanged()
{
    if(isEmpty()) return; // Do not try anything if the editor is empty
    getCurrent()->setStyleSheet(getCurrent()->styleSheet()+" color:rgb("+colorDialog()+");");
}

void Notepad::onBackgroundColorChanged()
{
    if(isEmpty()) return; // Do not try anything if the editor is empty
    getCurrent()->setStyleSheet(getCurrent()->styleSheet()+" background:rgb("+colorDialog()+");");
}

QString Notepad::colorDialog()
{
    const QColor chosenColor {QColorDialog::getColor("Choose a color")};
    auto colorToSet {QString::number(chosenColor.red())+","+QString::number(chosenColor.green())+","+QString::number(chosenColor.blue())};
    return colorToSet;
}

// Check the language type
void Notepad::checkFileLanguage()
{
    const auto currentFileExtension {QFileInfo(fileName()).completeSuffix()};
    QString currentFileType {"Plain Text"};
    QHashIterator<QString,QSet<QString>> iteratorHash{FILE_EXTENSIONS};
    while (iteratorHash.hasNext())
    {
        auto elmt = iteratorHash.next();
        const auto& content = elmt.value();
        if(content.contains(currentFileExtension)) currentFileType = iteratorHash.key();
    }
    fileType->setText(currentFileType);
}

// Font customization
void Notepad::onFont()
{
    bool ok;
    const QFont font {QFontDialog::getFont(&ok, QFont("Times", 12), this)};
    if(ok)
    {
        getCurrent()->setFont(font);
    }
}

// Settings
void Notepad::onSettings()
{
    Settings *s = new Settings(this,THEME_NAMES.keys(),notepadSettings.value("Terminal").toString(),notepadSettings.value("Tab width").toUInt());
    connect(s,&Settings::themeChanged,this,&Notepad::onApplyOtherTheme);
    connect(s,&Settings::terminalChanged,this,&Notepad::changeTerminal);
    connect(s,&Settings::localThemeSelected,this,&Notepad::onApplyLocalTheme);
    connect(s,&Settings::changeTabWidth,this,&Notepad::setTabSpace);
    s->show();
}

// Applying the new theme with the name
void Notepad::onApplyOtherTheme(QString theme)
{
    notepadSettings.setValue("Theme",theme);
    setStyleSheet(THEME_NAMES[theme]);
}

// Applying a user's theme
void Notepad::onApplyLocalTheme(const QString &themeFileName)
{
    QDir dir{};
    dir.mkpath(THEME_DIR);
    setStyleSheet(loadStyleFromFile(themeFileName));
    QFile::copy(themeFileName,THEME_DIR+"/"+QFileInfo{themeFileName}.fileName());
    loadSavedThemes();
    notepadSettings.setValue("Theme",THEME_NAMES[QFileInfo{themeFileName}.fileName().split(".").front()]);
}

// Change the default app terminal
void Notepad::changeTerminal(const QString &terminalName)
{
    notepadSettings.setValue("Terminal",terminalName);
}

// Set the tab space of the QPlainTextEdit
void Notepad::setTabSpace(uint8_t space)
{
    notepadSettings.setValue("Tab width",space);
    tabSpace = space;
    tabSpaceIndicator->setText("Tab space : "+QString::number(tabSpace));
    QFontMetrics metrics(getCurrent()->font());
    getCurrent()->setTabStopDistance(tabSpace * metrics.horizontalAdvance(' '));
}

// Synthax Highlighting module
void Notepad::synthaxicHighlighting()
{
    if(!highlightSynthax->isChecked()) return;
    for(auto i = 0; i != getCurrent()->blockCount(); i++)
    {
        QTextBlock block {getCurrent()->document()->findBlockByLineNumber(i)};
        if(block.isValid() && !isComment(block))
        {
            isComment(block);
            applyColoration(block);
        }
    }
}

// Apply coloration
void Notepad::applyColoration(const QTextBlock block)
{
    QList<QTextEdit::ExtraSelection> extraSelections {getCurrent()->extraSelections()};
    const QString text {block.text()};
    foreach(auto highlight,keywordsList)
    {
        int p;
        if(((p = text.indexOf(highlight)) != -1) && (text.mid(p,highlight.length()+1) == highlight+" "))
        {
            int pos {block.position() + p};
            QTextEdit::ExtraSelection selection{};
            selection.cursor = QTextCursor(getCurrent()->document());
            selection.cursor.setPosition( pos );
            selection.cursor.setPosition( pos+highlight.length(), QTextCursor::KeepAnchor );
            selection.format.setForeground(Qt::red);
            extraSelections.append(selection);
            getCurrent()->setExtraSelections(extraSelections);
        }
    }
}

// Check if the textBlock is a comment
bool Notepad::isComment(const QTextBlock &textBlock) const
{
    const bool isOneLine {(textBlock.text().left(2) == "//")};
    const bool isHashtag {(textBlock.text().left(1) == "#")};
    return (isOneLine || isHashtag);
}

// Open a terminal in the current directory, it depends on the os
void Notepad::onTerminal()
{
    QString exec;
#if (defined (_WIN32) || defined (_WIN64))
    exec = QDir::homePath()+"\\AppData\\Local\\Microsoft\\WindowsApps\\wt.exe";
#elif (defined (LINUX) || defined (__linux__))
    exec = notepadSettings.value("Terminal").toString();
#endif
    QString path = ((tabView->count() > 0) && (tabView->currentIndex() >= 0)) ? QFileInfo(fileName()).absoluteDir().absolutePath()
                                                                              : QDir::home().absolutePath();
    QProcess *process = new QProcess(this);
    process->setWorkingDirectory(path);
    process->start(exec,QStringList{});
}

// Update the window title
void Notepad::updateTitle()
{
    setWindowTitle(tabView->tabText(tabView->currentIndex()));
}

// Update and show the cursor position in the status bar
void Notepad::updateCursorPosition()
{
    const int line {getCurrent()->textCursor().blockNumber() + 1} ;
    const int columnn {getCurrent()->textCursor().positionInBlock() + 1};
    position->setText("Line : "+QString::number(line)+" Col : "+QString::number(columnn));
}

// Update the slots connexion when the tab is changed
void Notepad::updateConnect()
{
    connect(getCurrent(),&QPlainTextEdit::textChanged,this,&Notepad::synthaxicHighlighting);
    connect(getCurrent(),&QPlainTextEdit::textChanged,this,&Notepad::onAutoSave);
    connect(getCurrent(),&QPlainTextEdit::textChanged,this,&Notepad::onTextModified);
    connect(getCurrent(),&QPlainTextEdit::cursorPositionChanged,this,&Notepad::updateCursorPosition);
    connect(fileView,&QTreeView::doubleClicked,this,&Notepad::fileViewItemClicked);
    connect(lock,&QPushButton::clicked,this,&Notepad::onCheckLock);
}

// Lock or unlock the current file
void Notepad::onApplyLock()
{
    getCurrent()->setReadOnly(!getCurrent()->isReadOnly());
    onCheckLock();
}

void Notepad::onCheckLock()
{
    lock->setIcon((getCurrent()->isReadOnly()) ? QIcon(":assets/lock.ico") : QIcon(":assets/unlock.ico"));
}

QString Notepad::fileName() const
{
    if(tabView->count() == 0) return " ";
    return tabView->tabText(tabView->currentIndex());
}

// Get the index with the current filename
int Notepad::getIndex(const QString &tabName) const
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

void Notepad::closeAllTabs()
{
    tabView->addTab(new QPlainTextEdit(this),"New File");
    auto const sizeTab{tabView->count()-1};
    for(auto s{0};s != sizeTab;s++)
    {
        tabView->removeTab(0);
    }
}

bool Notepad::isEmpty() const
{
    return (tabView->count() == 0);
}

// Zooming in the current qplaintextedit
void Notepad::zoomPlus()
{
    getCurrent()->zoomIn();
}

void Notepad::zoomMinus()
{
    getCurrent()->zoomOut();
}

// Get the current qplaintextedit widget
QPlainTextEdit* Notepad::getCurrent() const
{
    return (isEmpty()) ? nullptr : qobject_cast<QPlainTextEdit*>(tabView->currentWidget());
}

// Load all the themes located in the local themes directory
void Notepad::loadSavedThemes()
{
    QString tmpThemeName;
    QDirIterator themeDirIt{THEME_DIR};
    while((themeDirIt.hasNext()) && (QFileInfo(tmpThemeName = themeDirIt.next())).isFile())
    {
        THEME_NAMES.insert(QFileInfo{tmpThemeName}.fileName().split(".").front(),loadStyleFromFile(tmpThemeName));
    }
}

// Key event
void Notepad::keyReleaseEvent(QKeyEvent *e)
{
    if(!getCurrent()->hasFocus()) return;
    const auto keyText{e->text()};
    if(keyText.isEmpty() || keyText.isNull()) return;
    const QChar charEntered{keyText.at(0)};
    if(charEntered.isLetterOrNumber())
    {
        currentWord += charEntered;
        const QRegularExpression regexp{currentWord,QRegularExpression::CaseInsensitiveOption};
        QStringList results{};
        foreach(auto const tempWord,words)
        {
            if(regexp.match(tempWord).hasMatch())
            {
                results.append(tempWord);
            }
        }
        if(!results.empty())
        {
            // TODO
            // focus implementation and fix some small bugs
            Popup *popupMenu = new Popup(this,results);
            connect(popupMenu,&Popup::charCancel,this,[&](const QChar c){
                getCurrent()->insertPlainText(c);
            });
            connect(popupMenu,&Popup::textSelected,this,[&](const QString &text){
                getCurrent()->insertPlainText(text.right(text.size()-currentWord.size())+" ");
            });
            const QPoint ref{0,0};
            const QPoint cursorPos{(getCurrent()->mapToGlobal(ref).x())+getCurrent()->cursorRect().x()+10,
                                   getCurrent()->mapToGlobal(ref).y()+getCurrent()->cursorRect().y()+10};
            popupMenu->popup(cursorPos);
            popupMenu->setFocus();
        }
    }
    else
    {
        if(!(currentWord.size() < 2))
        {
            if(!words.contains(currentWord))
            {
                words.append(currentWord);
            }
            currentWord.clear();
        }
    }
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
    if (event->mimeData()->hasUrls())
    {
        const QList<QUrl> urlList {event->mimeData()->urls()};
        // Extract the local path.
        const QString filename {urlList[0].toString().right(urlList[0].toString().length() - 7)};
        onOpenFile(filename);
    }
}

void Notepad::onAbout()
{
    auto aboutDialog = new About(this);
    aboutDialog->show();
}

Notepad::~Notepad()
{}