#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include "style.hpp"
#include "keywords.hpp"
#include "settings.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QSettings>
#include <QStyle>
#include <QIcon>
#include <QMap>
#include <QMapIterator>
#include <QStringList>
#include <QList>
#include <QCheckBox>
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
#include <QFileDialog>
#include <QFileSystemModel>
#include <QTreeView>
#include <QVariant>
#include <QColor>
#include <QTextDocument>
#include <QTextBlock>
#include <QTabWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMimeData>
#include <QAction>

class Notepad : public QMainWindow
{
    Q_OBJECT
public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    // Constants.
    const QString Adaptic      = loadStyleFromFile(":style/Adaptic.qss");
    const QString Amoled       = loadStyleFromFile(":style/Amoled.qss");
    const QString Aqua         = loadStyleFromFile(":style/Aqua.qss");
    const QString Console      = loadStyleFromFile(":style/Console.qss");
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
    const QString Ubuntu       = loadStyleFromFile(":style/Ubuntu.qss");
    const QString World        = loadStyleFromFile(":style/World.qss");
    const QMap<QString,QStringList> FILE_EXTENSIONS{
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
        {"CSS",{"css"}},
        {"CSV",{"csv"}},
        {"Clojure",{"clj","boot","cl2","cljc","cljs","cljs.hl","cljscm","cljx","hic"}},
        {"CofeeScript",{"coffee","_coffee","cake","cjsx","cson","iced"}},
        {"Coq",{"coq","v"}},
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
        {"Ruby",{"rb","ruby","ru"}},
        {"Rust",{"rs","rs.in"}},
        {"Sass",{"sass"}},
        {"SCSS",{"scss"}},
        {"SQL",{"sql"}},
        {"Shell",{"bash","sh","zsh","ksh"}},
        {"Swift",{"swift"}},
        {"TeX",{"tex"}},
        {"Text",{"txt","","fr"}},
        {"Typescript",{"ts","tsx"}},
        {"Visual Basic",{"vb","vba","vbs","vbhtml"}},
        {"Vue",{"vue"}},
        {"XML",{"xml","axml","ccxml","fxml"}},
        {"Yaml",{"yaml","yml","yaml-tmlanguage"}},
    };
    const QMap<QString,QString> THEME_NAMES{
        {"Adaptic",Adaptic},
        {"Amoled",Amoled},
        {"Aqua",Aqua},
        {"Console",Console},
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
        {"Ubuntu",Ubuntu},
        {"World",World}
    };
    // Member variables.
    bool isSaved = false;
    uint8_t tabSpace = 4;
    QLabel *position;
    QLabel *fileType;
    QMenuBar *menuBar;
    QMenu *file;
    QMenu *custom;
    QAction *newFile;
    QAction *openFile;
    QAction *saveFile;
    QAction *quit;
    QAction *colorText;
    QAction *colorBackground;
    QAction *highlightSynthax;
    QAction *fontChange;
    QAction *settings;
    QAction *terminal;
    QStatusBar *statusBar;
    QTabWidget *tabView;
    QCheckBox *autoSaveCheckBox;
    QFileSystemModel *fileModel;
    QTreeView *fileView;
    QString currentFileLanguage;
    QSettings notepadSettings;
    // Methods.
    QString fileName();
    bool isEmpty();
    QString colorDialog();
    int getIndex(const QString &tabName);
    void buildComponents();
    void buildMenu();
    void buildStatusBar();
    void buildFileView();
    void applyLayout();
    void applyStyle();
    void makeConnections();
    void createFile(const QString &fileToCreate);
    bool isComment(const QTextBlock &textBlock);
private slots:
    void onNewFile();
    void openFileDialog();
    void onOpenFile(const QString &filename);
    void onSaveFile();
    void onNewFileSave();
    void onExistingFileSave();
    void onCloseFile(const int &index);
    void onQuit();
    void onColorChanged();
    void onBackgroundColorChanged();
    void onApplyOtherTheme(QString theme);
    void onAutoSave();
    void onTextModified();
    void onTerminal();
    void onSettings();
    void onFont();
    void updateTitle();
    void updateCursorPosition();
    void updateConnect();
    void synthaxicHighlighting();
    void applyColoration(const QTextBlock block);
    void checkFileLanguage();
    void updateFileView();
    void fileRenamed(const QString &path,const QString &oldName,const QString &newName);
    void fileViewItemClicked(const QModelIndex &index);
    void setTabSpace(uint8_t space = 4);
};
#endif // NOTEPAD_HPP
