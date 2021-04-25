#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include "style.hpp"
#include "keywords.hpp"
#include "settings.hpp"
#include "about.hpp"
#include "popup.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QSettings>
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

class Notepad : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Notepad(QWidget *parent = nullptr);
        ~Notepad();

    protected:
        void keyReleaseEvent(QKeyEvent *e) override;
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dropEvent(QDropEvent *event) override;

    private:
        // Constants.
        const QString THEME_DIR{QDir::homePath()+"/.qnotepad_themes"};
        const QString Adaptic      = loadStyleFromFile(":style/Adaptic.qss");
        const QString Amoled       = loadStyleFromFile(":style/Amoled.qss");
        const QString Aqua         = loadStyleFromFile(":style/Aqua.qss");
        const QString Console      = loadStyleFromFile(":style/Console.qss");
        const QString DEFAULT      = "";
        const QString Diffness     = loadStyleFromFile(":style/Diffness.qss");
        const QString Dtor         = loadStyleFromFile(":style/Dtor.qss");
        const QString ElegantDark  = loadStyleFromFile(":style/ElegantDark.qss");
        const QString Fibrary      = loadStyleFromFile(":style/Fibrary.qss");
        const QString Genetive     = loadStyleFromFile(":style/Genetive.qss");
        const QString Irrorater    = loadStyleFromFile(":style/Irrorater.qss");
        const QString Mac          = loadStyleFromFile(":style/Mac.qss");
        const QString Manjaro      = loadStyleFromFile(":style/Manjaro.qss");
        const QString Material     = loadStyleFromFile(":style/Material.qss");
        const QString Obit         = loadStyleFromFile(":style/Obit.qss");
        const QString Visual       = loadStyleFromFile(":style/Visual.qss");
        const QString Ubuntu       = loadStyleFromFile(":style/Ubuntu.qss");
        const QString World        = loadStyleFromFile(":style/World.qss");
        const QHash<QString,QSet<QString>> FILE_EXTENSIONS{
                {"Asp",{"asp","asax","ascx","ashx","asmx","aspx","axd"}},
                {"Ada",{"adb","ada","ads"}},
                {"ApacheConf",{"apacheconf","vhost"}},
                {"Applescript",{"applescript","scpt"}},
                {"Arduino",{"ino"}},
                {"Assembly",{"asm","a51","inc","nasm"}},
                {"Awk",{"awk","auk","gawk","mawk","nawk"}},
                {"Batchfile",{"bat","cmd"}},
                {"Brainfuck",{"b","bf"}},
                {"C",{"c","h","w","cats","idc"}},
                {"C#",{"cs","cake","cshtml","csx"}},
                {"C++",{"cc","cp","cpp","c++","cxx","h","h++","hxx","hh","tpp"}},
                {"Cmake",{"cmake","cmake.in"}},
                {"Cobol",{"cob","cbl","ccp","cobol","cpy"}},
                {"Configuration",{"conf"}},
                {"CSS",{"css"}},
                {"CSV",{"csv"}},
                {"Clojure",{"clj","boot","cl2","cljc","cljs","cljs.hl","cljscm","cljx","hic"}},
                {"CoffeeScript",{"coffee","_coffee","cake","cjsx","cson","iced"}},
                {"Coq",{"coq","v"}},
                {"Cuda",{"cu"}},
                {"D",{"d","di"}},
                {"Dart",{"dart"}},
                {"Diff",{"diff","patch"}},
                {"DockerFile",{"dockerfile"}},
                {"Eiffel",{"e"}},
                {"Erlang",{"erl","es","escript","hrl","xrl","yrl"}},
                {"F#",{"fs","fsi","fsx"}},
                {"Fortran",{"f90","f","f03","f08","f77","f95","for","fpp"}},
                {"Go",{"go"}},
                {"Gradle",{"gradle"}},
                {"GraphQL",{"graphql"}},
                {"Groovy",{"groovy","grt","gtpl","gvy"}},
                {"Html",{"html","htm","html.ml","xht","xhtml","shtml"}},
                {"Haskell",{"hs","hsc"}},
                {"Ini",{"ini"}},
                {"Json",{"json","geojson","topojson"}},
                {"Java",{"java"}},
                {"Javascript",{"js","_js","es","es6","gs"}},
                {"Julia",{"jl"}},
                {"Kotlin",{"kt","ktm","kts"}},
                {"Less",{"less"}},
                {"Lua",{"lua","nse"}},
                {"Makefile",{"mak","mk","mkfile"}},
                {"Markdown",{"md","markdown","mkd","mkdown","mkdn"}},
                {"Matlab",{"m","matlab"}},
                {"OCaml",{"ml","eliom","eliomi","m14","mli"}},
                {"Objective-C",{"m"}},
                {"Objective-C++",{"mm"}},
                {"Php",{"php","aw","ctp","fcgi","php3","php4","php5","phps","phpt"}},
                {"Pascal",{"pas","lpr"}},
                {"Perl",{"pl"}},
                {"Powershell",{"ps1","psd1","psm1"}},
                {"Prolog",{"yap"}},
                {"QMake",{"pro","pri"}},
                {"QML",{"qml","qbs"}},
                {"R",{"r"}},
                {"Resource",{"qrc"}},
                {"Ruby",{"rb","ruby","ru"}},
                {"Rust",{"rs","rs.in"}},
                {"Sass",{"sass"}},
                {"SCSS",{"scss"}},
                {"SQL",{"sql","pls"}},
                {"Shell",{"bash","sh","zsh","ksh"}},
                {"Swift",{"swift"}},
                {"Toml",{"Toml"}},
                {"TeX",{"tex"}},
                {"Text",{"txt","","fr"}},
                {"Typescript",{"ts","tsx"}},
                {"Visual Basic",{"vb","vba","vbs","vbhtml"}},
                {"Vue",{"vue"}},
                {"XML",{"xml","axml","ccxml","fxml"}},
                {"Yaml",{"yaml","yml","yaml-tmlanguage"}},
        };
        QHash<QString,QString> THEME_NAMES{
                {"Adaptic",Adaptic},
                {"Amoled",Amoled},
                {"Aqua",Aqua},
                {"Console",Console},
                {"Default Theme",DEFAULT},
                {"Diffness",Diffness},
                {"Dtor",Dtor},
                {"Elegant Dark",ElegantDark},
                {"Fibrary",Fibrary},
                {"Genetive",Genetive},
                {"Irrorater",Irrorater},
                {"Mac",Mac},
                {"Manjaro",Manjaro},
                {"Material Dark",Material},
                {"Obit",Obit},
                {"Visual",Visual},
                {"Ubuntu",Ubuntu},
                {"World",World}
        };
        // Member variables.
        bool isSaved {false};
        uint8_t tabSpace {4};
        QLabel *position;
        QLabel *fileType;
        QLabel *tabSpaceIndicator;
        QMenuBar *menuBar;
        QMenu *file;
        QMenu *color;
        QMenu *edit;
        QMenu *view;
        QAction *newFile;
        QAction *openFile;
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
        QStatusBar *statusBar;
        QPushButton *lock;
        QTabWidget *tabView;
        QFileSystemModel *fileModel;
        QTreeView *fileView;
        QSplitter *editorSplitter;
        QSettings notepadSettings;
        // Methods.
        QString fileName() const;
        bool isEmpty() const;
        static QString colorDialog();
        int getIndex(const QString &tabName) const;
        void buildComponents();
        void buildMenu();
        void buildStatusBar();
        void buildFileView();
        void applyLayout();
        void applyStyle();
        void makeConnections();
        void applyShortcuts();
        void createFile(const QString &fileToCreate);
        void loadSavedThemes();
        QPlainTextEdit* getCurrent() const;
        bool isComment(const QTextBlock &textBlock) const;
        QStringList words;
        QString currentWord{};
    private slots:
        void onNewFile();
        void openFileDialog();
        void onSaveFile();
        void onNewFileSave();
        void onExistingFileSave();
        void onCloseFile(const int &index);
        void closeAllTabs();
        void onQuit();
        void onColorChanged();
        void onBackgroundColorChanged();
        void onApplyOtherTheme(const QString& theme);
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
        void applyColoration(const QTextBlock& block);
        void checkFileLanguage();
        void updateFileView();
        void changeTerminal(const QString &terminalName);
        void fileRenamed(const QString &path,const QString &oldName,const QString &newName);
        void fileViewItemClicked(const QModelIndex &index);
        void setTabSpace(uint8_t space = 4);
        void zoomPlus();
        void zoomMinus();
        void onAbout();
    public slots:
        void onOpenFile(const QString &filename);
};
#endif // NOTEPAD_HPP