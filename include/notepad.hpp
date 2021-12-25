#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include "keywords.hpp"
#include "settings.hpp"
#include "recentfilesmanager.hpp"
#include "about.hpp"
#include "popup.hpp"
#include "filemanager.hpp"
#include "thememanager.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QStyle>
#include <QIcon>
#include <QDir>
#include <QRegularExpression>
#include <QMapIterator>
#include <QDirIterator>
#include <QProcess>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>
#include <QFontMetrics>
#include <QFontDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QTreeView>
#include <QHeaderView>
#include <QVariant>
#include <QColor>
#include <QTextDocument>
#include <QTextBlock>
#include <QTabWidget>
#include <QMenuBar>
#include <QMimeData>
#include <QSplitter>
#include <QHash>

// TODO : implement QSynthaxHighlighting
class Notepad : public QMainWindow
{
Q_OBJECT

public:
    explicit Notepad(QWidget *parent = nullptr);
    ~Notepad() = default;

protected:
    void keyReleaseEvent(QKeyEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    ThemeManager themeManager{};
    constexpr static uint8_t DEFAULT_TAB_SPACE = 4;
    const QString THEME_DIR{QDir::homePath() + "/.qnotepad_themes"};
    bool isSaved{false};
    uint8_t tabSpace{4};
    QLabel *position;
    QLabel *fileType;
    QLabel *tabSpaceIndicator;
    QMenu *file;
    QMenu *color;
    QMenu *edit;
    QMenu *view;
    QAction *newFile;
    QAction *openFile;
    QAction *openFolder;
    QAction *saveFile;
    QAction *saveFileAs;
    QAction *closeAll;
    QAction *quit;
    QAction *autoSave;
    QAction *colorText;
    QAction *colorBackground;
    QAction *highlightSyntax;
    QAction *fontChange;
    QAction *settings;
    QAction *zoomIn;
    QAction *zoomOut;
    QAction *terminal;
    QAction *about;
    QMenu *recentlyOpened;
    QStatusBar *statusBar;
    QPushButton *lock;
    QTabWidget *tabView;
    QFileSystemModel *fileModel;
    QTreeView *fileView;
    QSplitter *editorSplitter;
    QSettings notepadSettings;
    // Methods.
    [[nodiscard]] QString fileName() const;
    [[nodiscard]] bool isEmpty() const;
    static QString colorDialog();
    [[nodiscard]] int getIndex(const QString &tabName) const;
    void buildComponentsAndMenu();
    void buildStatusBar();
    void buildFileView();
    void buildRecentlyOpenedFileList();
    void applyLayout();
    void applyStyle();
    void makeConnections();
    void applyShortcuts();
    void createFile(const QString &fileToCreate);
    [[nodiscard]] QPlainTextEdit *getCurrent() const;
    [[nodiscard]] bool isComment(const QTextBlock &textBlock) const;
    QStringList words;
    QString currentWord{};
    RecentFilesManager recentFilesManager;

private slots:
    void onNewFile();
    void openFileDialog();
    void openFolderDialog();
    void onSaveFile();
    void onNewFileSave();
    void onExistingFileSave();
    void onCloseFile(const int &index);
    void closeAllTabs();
    void onQuit();
    void onColorChanged();
    void onBackgroundColorChanged();
    void onApplyOtherTheme(const QString &theme);
    void onApplyLocalTheme(const QString &themeFileName);
    void onAutoSave();
    void onTextModified();
    void onApplyLock();
    void onCheckLock();
    void onTerminal();
    void onSettings();
    void onFont();
    void updateTitle();
    void updateCursorPosition();
    void updateConnect();
    void syntaxicHighlighting();
    void applyColoration(const QTextBlock &block);
    void checkFileLanguage();
    void updateFileView();
    void changeTerminal(const QString &terminalName);
    void fileRenamed(const QString &path, const QString &oldName, const QString &newName);
    void fileViewItemClicked(const QModelIndex &index);
    void setTabSpace(uint8_t space = 4);
    void zoomPlus();
    void zoomMinus();
    void onAbout();

public slots:
    void onOpenFile(const QString &filename);
    void onOpenFolder(const QString &folder);
};

#endif // NOTEPAD_HPP