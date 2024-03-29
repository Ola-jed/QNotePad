#include "notepad.hpp"

/// Constructor
Notepad::Notepad(QWidget *parent) : QMainWindow(parent)
{
    themeManager.loadSavedThemes();
    buildComponentsAndMenu();
    buildRecentlyOpenedFileList();
    buildStatusBar();
    buildFileView();
    applyShortcuts();
    tabView->setTabsClosable(true);
    tabView->setAcceptDrops(true);
    tabView->addTab(new Editor(this), "New File");
    applyLayout();
    applyStyle();
    checkFileLanguage();
    setAcceptDrops(true);
    makeConnections();
    fileView->setEditTriggers(QAbstractItemView::EditKeyPressed);
}

/// Connections.
void Notepad::makeConnections()
{
    connect(settings, &QAction::triggered, this, &Notepad::onSettings);
    connect(quit, &QAction::triggered, this, &Notepad::onQuit);
    connect(newFile, &QAction::triggered, this, &Notepad::onNewFile);
    connect(openFile, &QAction::triggered, this, &Notepad::openFileDialog);
    connect(openFolder, &QAction::triggered, this, &Notepad::openFolderDialog);
    connect(saveFile, &QAction::triggered, this, &Notepad::onSaveFile);
    connect(saveFileAs, &QAction::triggered, this, &Notepad::onNewFileSave);
    connect(terminal, &QAction::triggered, this, &Notepad::onTerminal);
    connect(fontChange, &QAction::triggered, this, &Notepad::onFont);
    connect(closeAll, &QAction::triggered, this, &Notepad::closeAllTabs);
    connect(colorText, &QAction::triggered, this, &Notepad::onColorChanged);
    connect(about, &QAction::triggered, this, &Notepad::onAbout);
    connect(syncConfiguration, &QAction::triggered, this, &Notepad::onConfigurationSyncing);
    connect(colorBackground, &QAction::triggered, this, &Notepad::onBackgroundColorChanged);
    connect(zoomIn, &QAction::triggered, this, &Notepad::zoomPlus);
    connect(zoomOut, &QAction::triggered, this, &Notepad::zoomMinus);
    connect(tabView, &QTabWidget::tabCloseRequested, this, &Notepad::onCloseFile);
    connect(tabView, &QTabWidget::currentChanged, this, &Notepad::updateConnect);
    connect(tabView, &QTabWidget::currentChanged, this, &Notepad::updateTitle);
    connect(tabView, &QTabWidget::currentChanged, this, &Notepad::updateFileView);
    connect(tabView, &QTabWidget::currentChanged, this, &Notepad::onCheckLock);
    connect(tabView, &QTabWidget::currentChanged, this, &Notepad::checkFileLanguage);
    connect(lock, &QPushButton::clicked, this, &Notepad::onApplyLock);
    connect(getCurrent(), &Editor::textChanged, this, &Notepad::onAutoSave);
    connect(getCurrent(), &Editor::textChanged, this, &Notepad::onTextModified);
    connect(getCurrent(), &Editor::textChanged, this, &Notepad::syntaxicHighlighting);
    connect(getCurrent(), &Editor::cursorPositionChanged, this, &Notepad::updateCursorPosition);
    connect(fileView, &QTreeView::doubleClicked, this, &Notepad::fileViewItemClicked);
    connect(fileModel, &QFileSystemModel::fileRenamed, this, &Notepad::fileRenamed);
}

/// Creating the items.
void Notepad::buildComponentsAndMenu()
{
    // File menu
    file = menuBar()->addMenu("File");
    newFile = file->addAction(QIcon(":assets/new.ico"), "New");
    openFile = file->addAction(QIcon(":assets/open.ico"), "Open file");
    openFolder = file->addAction(QIcon(":assets/openfolder.ico"), "Open folder");
    file->addSeparator();
    saveFile = file->addAction(QIcon(":assets/save.ico"), "Save");
    saveFileAs = file->addAction(QIcon(":assets/saveas.ico"), "Save as");
    file->addSeparator();
    closeAll = file->addAction(QIcon(":assets/closeall.ico"), "Close all");
    quit = file->addAction(QIcon(":assets/quit.ico"), "Quit");
    file->addSeparator();
    autoSave = file->addAction(QIcon(":assets/autosave.ico"), "AutoSave");
    file->addSeparator();
    recentlyOpened = file->addMenu(QIcon(":assets/recent.ico"), "Recently opened");
    // Preferences menu
    edit = menuBar()->addMenu("Preferences");
    color = edit->addMenu(QIcon(":assets/colormenu.ico"), "Color");
    colorText = color->addAction(QIcon(":assets/color.ico"), "Text Color");
    colorBackground = color->addAction(QIcon(":assets/background-color"), "Background color");
    settings = edit->addAction(QIcon(":assets/settings.ico"), "Settings");
    highlightSyntax = edit->addAction(QIcon(":assets/highlight.ico"), "Synthax highlighting");
    fontChange = edit->addAction(QIcon(":assets/font.ico"), "Font");
    syncConfiguration = edit->addAction(QIcon(":assets/sync.ico"),"Sync configuration");
    highlightSyntax->setCheckable(true);
    highlightSyntax->setChecked(false);
    // Zoom menu
    view = menuBar()->addMenu("Zoom");
    zoomIn = view->addAction(QIcon(":assets/zoomin.ico"), "Zoom in");
    zoomOut = view->addAction(QIcon(":assets/zoomout.ico"), "Zoom out");
    // Other menu items
    terminal = new QAction(QIcon(":assets/terminal.ico"), "Terminal", this);
    about = new QAction(QIcon(":assets/about.ico"), "About", this);
    menuBar()->addAction(terminal);
    menuBar()->addAction(about);
    tabView = new QTabWidget(this);
}

/// Build the list of recently opened files
/// Iterate over the recently opened list to add actions to the menu
void Notepad::buildRecentlyOpenedFileList()
{
    const auto recentFiles{recentFilesManager.recentFiles()};
    for (const QVariant &aFile: recentFiles.first().toList())
    {
        recentlyOpened->addAction(aFile.toString(), [this, aFile] {
            onOpenFile(aFile.toString());
        });
    }
}

/// Build the treeFileView
void Notepad::buildFileView()
{
    fileModel = new QFileSystemModel(this);
    fileView = new QTreeView(this);
    fileModel->setReadOnly(false);
    fileView->setModel(fileModel);
    fileView->hideColumn(1);
    fileView->hideColumn(2);
    fileView->hideColumn(3);
    fileView->setVisible(false);
    fileView->setHeaderHidden(true);
    editorSplitter = new QSplitter(Qt::Horizontal, this);
}

/// Build the status bar
void Notepad::buildStatusBar()
{
    statusBar = new QStatusBar(this);
    position = new QLabel("", this);
    fileType = new QLabel("", this);
    lock = new QPushButton(QIcon(":assets/unlock.ico"), "", this);
    tabSpaceIndicator = new QLabel("", this);
    statusBar->addWidget(position, 1);
    statusBar->addWidget(fileType, 4);
    statusBar->addWidget(tabSpaceIndicator, 4);
    statusBar->addWidget(lock, 1);
    statusBar->setSizeGripEnabled(false);
}

/// Update the file view
void Notepad::updateFileView()
{
    const auto filePath{QFileInfo(fileName()).dir().path()};
    fileModel->setRootPath(filePath);
    const auto idx{fileModel->index(filePath)};
    fileView->setRootIndex(idx);
}

/// Setup the layout
void Notepad::applyLayout()
{
    editorSplitter->addWidget(fileView);
    editorSplitter->addWidget(tabView);
    editorSplitter->setStretchFactor(1, 4);
    setCentralWidget(editorSplitter);
    setStatusBar(statusBar);
}

/// Style.
/// If existing, we load tab space preferences and/or user stylesheet
void Notepad::applyStyle()
{
    resize(850, 550);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                    Qt::AlignCenter, size(),
                                    QGuiApplication::primaryScreen()->availableGeometry()));
    setWindowIcon(QIcon(":assets/notepad.ico"));
    setTabSpace((notepadSettings.value("Tab width").toInt() == 0)
                ? DEFAULT_TAB_SPACE
                : notepadSettings.value("Tab width").toInt());
    setStyleSheet(themeManager[notepadSettings.value("Theme", "").toString()]);
    tabView->setElideMode(Qt::ElideRight);
}

/// Open the item when the item is double clicked
/// \param index
void Notepad::fileViewItemClicked(const QModelIndex &index)
{
    auto const currentFileName{fileModel->filePath(index)};
    if (QFileInfo(currentFileName).isFile() && (getIndex(currentFileName) == -1))
    {
        onOpenFile(currentFileName);
    }
}

/// File renamed.
/// \param path
/// \param oldName
/// \param newName
void Notepad::fileRenamed(const QString &path, const QString &oldName, const QString &newName)
{
    const QString oldFileName{QString{path}.append("/").append(oldName)};
    const QString newFileName{QString{path}.append("/").append(newName)};
    if (!FileManager::rename(oldFileName, newFileName))
    {
        QMessageBox::warning(this, "File rename", "File renaming failed");
        return;
    }
    tabView->setTabText(getIndex(oldFileName), newFileName);
}

/// Menu shortcuts
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

/// New File
void Notepad::onNewFile()
{
    const auto filename{QFileDialog::getSaveFileName(this)};
    if (!filename.isEmpty())
    {
        createFile(filename);
    }
}

/// Create  new file
/// \param fileToCreate
void Notepad::createFile(const QString &fileToCreate)
{
    if ((!FileManager::createFile(fileToCreate)))
    {
        QMessageBox::critical(this, "New File", "Cannot create the file");
        return;
    }
    auto fileContent = new Editor(this);
    tabView->addTab(fileContent, fileToCreate);
    tabView->setCurrentIndex(getIndex(fileToCreate));
    tabView->setTabText(getIndex(fileToCreate), fileToCreate);
}

/// Dialog for opening a file
void Notepad::openFileDialog()
{
    auto const filename{QFileDialog::getOpenFileName(this)};
    if (filename.isEmpty())
    {
        return;
    }
    onOpenFile(filename);
}

/// Dialog for opening a folder
void Notepad::openFolderDialog()
{
    auto const folder{QFileDialog::getExistingDirectory(this)};
    if (folder.isEmpty())
    {
        return;
    }
    onOpenFolder(folder);
}

/// Opening a file.
/// \param filename
void Notepad::onOpenFile(const QString &filename)
{
    const auto fileContent{FileManager::readAll(filename)};
    if (!fileContent.has_value())
    {
        QMessageBox::warning(this, "New File", "Cannot open the file");
        return;
    }
    recentFilesManager.addRecentFile(filename);
    auto tab = new Editor(this);
    tabView->addTab(tab, filename);
    tabView->setCurrentIndex(getIndex(filename));
    getCurrent()->setPlainText(fileContent.value());
}

/// Opening a folder
/// \param filename
void Notepad::onOpenFolder(const QString &folder)
{
    fileModel->setRootPath(folder);
    const auto idx{fileModel->index(QDir(folder).absolutePath())};
    fileView->setRootIndex(idx);
    fileView->setVisible(true);
}

/// Saving the current file.
void Notepad::onSaveFile()
{
    if ((fileName() == "New File") && !(getCurrent()->toPlainText().isEmpty()))
    {
        onNewFileSave();
    }
    else if ((!fileName().isEmpty()) && !(getCurrent()->toPlainText().isEmpty()))
    {
        onExistingFileSave();
    }
}

/// Saving a new file.
void Notepad::onNewFileSave()
{
    auto const filename{QFileDialog::getSaveFileName(this)};
    if (filename.isEmpty())
    {
        QMessageBox::warning(this, "New File", "Enter a valid name");
        return;
    }
    if (!FileManager::saveIntoFile(filename, getCurrent()->toPlainText()))
    {
        QMessageBox::critical(this, "Save", "Could not save");
        return;
    }
    setWindowTitle("QNotePad");
    tabView->setTabText(tabView->currentIndex(), filename);
}

// Saving an existing file.
void Notepad::onExistingFileSave()
{
    if (!FileManager::saveIntoFile(fileName(), getCurrent()->toPlainText()))
    {
        QMessageBox::critical(this, "Save", "Could not save");
        return;
    }
    setWindowTitle("QNotePad");
    isSaved = true;
}

// AutoSave of the file
void Notepad::onAutoSave()
{
    if (!(autoSave->isChecked()) || (fileName().isEmpty()))
    {
        return;
    }
    if (FileManager::saveIntoFile(fileName(), getCurrent()->toPlainText()))
    {
        isSaved = true;
        setWindowTitle("QNotePad");
    }
}

// Close the current file.
void Notepad::onCloseFile(const int &index)
{
    if (index < 0)
    {
        return;
    }
    if (tabView->count() == 1)
    {
        qApp->quit();
    }
    else
    {
        auto tabItem{tabView->widget(index)};
        // If this is a new file
        if (tabView->tabText(index).isEmpty() &&
            !(qobject_cast<Editor *>(tabItem)->toPlainText().trimmed().isEmpty()))
        {
            onNewFileSave();
        }
        else if ((!tabView->tabText(index).isEmpty()) &&
                 !(qobject_cast<Editor *>(tabItem)->toPlainText().trimmed().isEmpty()) &&
                 !isSaved)
        {
            const auto rep{QMessageBox::question(this, "Save", "Do you want to save ?")};
            if (rep == QMessageBox::Yes)
            {
                onExistingFileSave();
            }
        }
        tabView->removeTab(index);
        delete (tabItem);
    }
}

/// Update the window title
void Notepad::onTextModified()
{
    if (!autoSave->isChecked())
    {
        setWindowTitle("QNotePad (file modified)");
        isSaved = false;
    }
}

/// Quitting the app
void Notepad::onQuit()
{
    const auto currentTextEditIsEmpty{getCurrent()->toPlainText().trimmed().isEmpty()};
    const auto currentFilenameIsEmpty{fileName().trimmed().isEmpty()};
    if (((!currentFilenameIsEmpty) && !isSaved && !currentTextEditIsEmpty) ||
        (currentFilenameIsEmpty && !currentTextEditIsEmpty))
    {
        auto const reply{QMessageBox::question(this, "Quit", "Save ?", QMessageBox::Yes | QMessageBox::No)};
        if (reply == QMessageBox::Yes)
        {
            onSaveFile();
        }
    }
    qApp->quit();
}

/// Text Color
/// Do not try anything if the editor is empty
void Notepad::onColorChanged()
{
    if (isEmpty())
    {
        return;
    }
    getCurrent()->setStyleSheet(getCurrent()->styleSheet() + " color:" + colorDialog() + ";");
}

/// Background color
/// Do not try anything if the editor is empty
void Notepad::onBackgroundColorChanged()
{
    if (isEmpty())
    {
        return;
    }
    getCurrent()->setStyleSheet(getCurrent()->styleSheet() + " background:" + colorDialog() + ";");
}

/// Dialog to let the user choose a color
QString Notepad::colorDialog()
{
    return QColorDialog::getColor("Choose a color").name(QColor::HexRgb);
}

/// Check the language type
void Notepad::checkFileLanguage()
{
    const auto currentFileExtension{QFileInfo(fileName()).completeSuffix()};
    QString currentFileType{"Plain Text"};
    QHashIterator<QString, QSet<QString>> iteratorHash{fileExtensionsLoader.extensions()};
    while (iteratorHash.hasNext())
    {
        auto nextElement{iteratorHash.next()};
        const auto &content{nextElement.value()};
        if (content.contains(currentFileExtension))
        {
            currentFileType = iteratorHash.key();
        }
    }
    fileType->setText(currentFileType);
}

/// Font customization
void Notepad::onFont()
{
    bool ok;
    const auto font{QFontDialog::getFont(&ok, QFont("Times", 12), this)};
    if (ok)
    {
        getCurrent()->setFont(font);
    }
}

/// Settings
void Notepad::onSettings()
{
    auto s = new Settings(this, themeManager.themeNames(),
                          notepadSettings.value("Terminal").toString(),
                          notepadSettings.value("Tab width").toUInt());
    connect(s, &Settings::themeChanged, this, &Notepad::onApplyOtherTheme);
    connect(s, &Settings::terminalChanged, this, &Notepad::changeTerminal);
    connect(s, &Settings::localThemeSelected, this, &Notepad::onApplyLocalTheme);
    connect(s, &Settings::changeTabWidth, this, &Notepad::setTabSpace);
    s->show();
}

/// Applying the new theme with the name
/// \param theme
void Notepad::onApplyOtherTheme(const QString &theme)
{
    notepadSettings.setValue("Theme", theme);
    setStyleSheet(themeManager[theme]);
}

/// Applying a user's theme
/// \param themeFileName
void Notepad::onApplyLocalTheme(const QString &themeFileName)
{
    QDir dir{};
    dir.mkpath(THEME_DIR);
    const auto fileThemeName{QFileInfo{themeFileName}.fileName()};
    const auto theme{ThemeManager::loadStyleFromFile(themeFileName)};
    themeManager.addTheme(themeFileName, theme);
    setStyleSheet(theme);
    if (!FileManager::copy(themeFileName, THEME_DIR + "/" + fileThemeName))
    {
        QMessageBox::warning(this, "Theme loading", "Theme copy failed");
    }
    themeManager.loadSavedThemes();
    notepadSettings.setValue("Theme", themeManager[fileThemeName.split(".").front()]);
}

/// Change the default app terminal
/// \param terminalName
void Notepad::changeTerminal(const QString &terminalName)
{
    notepadSettings.setValue("Terminal", terminalName);
}

/// Set the tab space of the Editor
/// \param space
void Notepad::setTabSpace(uint8_t space)
{
    notepadSettings.setValue("Tab width", space);
    tabSpace = space;
    tabSpaceIndicator->setText("Tab space : " + QString::number(tabSpace));
    QFontMetrics metrics(getCurrent()->font());
    getCurrent()->setTabStopDistance(tabSpace * metrics.horizontalAdvance(' '));
}

/// Syntax Highlighting module
void Notepad::syntaxicHighlighting()
{
    if (!highlightSyntax->isChecked())
    {
        return;
    }
    for (auto i = 0; i != getCurrent()->blockCount(); i++)
    {
        auto block{getCurrent()->document()->findBlockByLineNumber(i)};
        if (block.isValid() && !isComment(block))
        {
            applyColoration(block);
        }
    }
}

/// Apply coloration
/// TODO : Fix the coloration
/// \param block
void Notepad::applyColoration(const QTextBlock &block)
{
    auto extraSelections{getCurrent()->extraSelections()};
    const auto text{block.text()};
    for (const auto &highlight: keywordsList)
    {
        int p;
        if (((p = text.indexOf(highlight)) != -1) && (text.mid(p, highlight.length() + 1) == highlight + " "))
        {
            const int pos{block.position() + p};
            QTextEdit::ExtraSelection selection{};
            selection.cursor = QTextCursor(getCurrent()->document());
            selection.cursor.setPosition(pos);
            selection.cursor.setPosition(pos + highlight.length(), QTextCursor::KeepAnchor);
            selection.format.setForeground(Qt::red);
            extraSelections.append(selection);
            getCurrent()->setExtraSelections(extraSelections);
        }
    }
}

/// Check if the textBlock is a comment
/// \param textBlock
/// TODO : this is not well implemented
bool Notepad::isComment(const QTextBlock &textBlock)
{
    const auto isOneLine{(textBlock.text().left(2) == "//")};
    const auto isHashtag{(textBlock.text().left(1) == "#")};
    return (isOneLine || isHashtag);
}

/// Open a terminal in the current directory, it depends on the os
void Notepad::onTerminal()
{
    QString exec;
#if (defined (_WIN32) || defined (_WIN64))
    exec = QDir::homePath()+"\\AppData\\Local\\Microsoft\\WindowsApps\\wt.exe";
#elif (defined (LINUX) || defined (__linux__))
    const auto terminalSetting = notepadSettings.value("Terminal").toString();
    exec = terminalSetting.trimmed().isEmpty() ? "konsole" : terminalSetting;
#endif
    const auto path = ((tabView->count() > 0) && (tabView->currentIndex() >= 0))
                      ? QFileInfo(fileName()).absoluteDir().absolutePath()
                      : QDir::home().absolutePath();
    auto process = new QProcess(this);
    process->setWorkingDirectory(path);
    process->start(exec, QStringList{});
}

/// Update the window title
void Notepad::updateTitle()
{
    setWindowTitle(tabView->tabText(tabView->currentIndex()));
}

/// Update and show the cursor position in the status bar
void Notepad::updateCursorPosition()
{
    const auto cursor{getCurrent()->textCursor()};
    const int line{cursor.blockNumber() + 1};
    const int column{cursor.positionInBlock() + 1};
    position->setText(QString{"Line : %1 Column : %2"}.arg(line).arg(column));
}

/// Update the slots connexion when the tab is changed
void Notepad::updateConnect()
{
    connect(getCurrent(), &Editor::textChanged, this, &Notepad::syntaxicHighlighting);
    connect(getCurrent(), &Editor::textChanged, this, &Notepad::onAutoSave);
    connect(getCurrent(), &Editor::textChanged, this, &Notepad::onTextModified);
    connect(getCurrent(), &Editor::cursorPositionChanged, this, &Notepad::updateCursorPosition);
    connect(fileView, &QTreeView::doubleClicked, this, &Notepad::fileViewItemClicked);
    connect(lock, &QPushButton::clicked, this, &Notepad::onCheckLock);
}

/// Lock or unlock the current file
void Notepad::onApplyLock()
{
    getCurrent()->setReadOnly(!getCurrent()->isReadOnly());
    onCheckLock();
}

/// Update the lock icon when the document is locked/unlocked
void Notepad::onCheckLock()
{
    lock->setIcon((getCurrent()->isReadOnly())
                  ? QIcon(":assets/lock.ico")
                  : QIcon(":assets/unlock.ico")
    );
}

/// Return the name of the file opened in the current tab
QString Notepad::fileName() const
{
    if (tabView->count() == 0)
    {
        return "";
    }
    return tabView->tabText(tabView->currentIndex());
}

/// Get the index with the current filename
int Notepad::getIndex(const QString &tabName) const
{
    for (auto i = 0; i <= tabView->count(); i++)
    {
        if (tabView->tabText(i) == tabName)
        {
            return i;
        }
    }
    return -1;
}

/// To close all the tabs, we loop on the size and we close the tab at pos 1
void Notepad::closeAllTabs()
{
    tabView->addTab(new Editor(this), "New File");
    auto const sizeTab{tabView->count() - 1};
    for (auto s = 0; s != sizeTab; s++)
    {
        tabView->removeTab(0);
    }
}

/// Is the app empty ?
bool Notepad::isEmpty() const
{
    return tabView->count() == 0;
}

/// Zooming in the current qplaintextedit
void Notepad::zoomPlus()
{
    getCurrent()->zoomIn();
}

/// Zooming out the current qplaintextedit
void Notepad::zoomMinus()
{
    getCurrent()->zoomOut();
}

/// Get the current qplaintextedit widget
Editor *Notepad::getCurrent() const
{
    return isEmpty() ? nullptr : qobject_cast<Editor *>(tabView->currentWidget());
}

/// Sync the configuration with the settings in the json files
void Notepad::onConfigurationSyncing()
{
    fileExtensionsLoader.saveJsonToSettings();
    QMessageBox::information(this, "Configuration synced",
                             "Configuration synced.Please restart the application");
}

/// Key event
/// \param e
void Notepad::keyReleaseEvent(QKeyEvent *e)
{
    if (!getCurrent()->hasFocus())
    {
        return;
    }
    const auto keyText{e->text()};
    if (keyText.isEmpty() || keyText.isNull())
    {
        return;
    }
    const QChar charEntered{keyText.at(0)};
    if (charEntered.isLetterOrNumber())
    {
        currentWord += charEntered;
        const QRegularExpression regexp{currentWord, QRegularExpression::CaseInsensitiveOption};
        QStringList results{};
        for (auto const &tempWord: words)
        {
            if (regexp.match(tempWord).hasMatch())
            {
                results.append(tempWord);
            }
        }
        if (!results.empty())
        {
            auto popupMenu = new Popup(this, results);
            connect(popupMenu, &Popup::charCancel, this, [&](const QChar c) {
                getCurrent()->insertPlainText(c);
            });
            connect(popupMenu, &Popup::textSelected, this, [&](const QString &text) {
                getCurrent()->insertPlainText(text.right(text.size() - currentWord.size()) + " ");
            });
            const QPoint ref{0, 0};
            const QPoint cursorPos{(getCurrent()->mapToGlobal(ref).x()) + getCurrent()->cursorRect().x() + 10,
                                   getCurrent()->mapToGlobal(ref).y() + getCurrent()->cursorRect().y() + 10};
            popupMenu->popup(cursorPos);
            popupMenu->setFocus();
        }
    }
    else
    {
        if (currentWord.size() >= 2)
        {
            if (!words.contains(currentWord))
            {
                words.append(currentWord);
            }
            currentWord.clear();
        }
    }
}

/// Drag event to open files.
/// \param e
void Notepad::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

/// Drop event to open files
/// \param event
void Notepad::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        const auto urlList{event->mimeData()->urls()};
        const auto filename{urlList[0].toLocalFile()};
        onOpenFile(filename);
    }
}

/// Show the about dialog
void Notepad::onAbout()
{
    auto aboutDialog = new About(this);
    aboutDialog->show();
}