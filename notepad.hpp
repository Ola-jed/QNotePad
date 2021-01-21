#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include "style.hpp"
#include "keywords.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QStyle>
#include <QIcon>
#include <QMap>
#include <QMapIterator>
#include <QStringList>
#include <QList>
#include <QCheckBox>
#include <QProcess>
#include <QPlainTextEdit>
#include <QFile>
#include <QFileInfo>
#include <QStatusBar>
#include <QComboBox>
#include <QLabel>
#include <QTextStream>
#include <QMessageBox>
#include <QFont>
#include <QFontMetrics>
#include <QFontDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextDocument>
#include <QTextBlock>
#include <QTabWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMimeData>
#include <QAction>
#include <QDebug>

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
        {"Difness",Diffness},
        {"Elegant Dark",ElegantDark},
        {"Mac",Mac},
        {"Manjaro",Manjaro},
        {"Material Dark",Material},
        {"Obit",Obit},
        {"Ubuntu",Ubuntu},
        {"World",World}
    };
    const int TAB_SPACE = 4;
    // Member variables.
    bool isSaved = false;
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
    QAction *terminal;
    QStatusBar *positionBar;
    QTabWidget *tabView;
    QCheckBox *autoSaveCheckBox;
    QComboBox *themeChoice;
    QString currentFileLanguage;
    // Methods.
    QString fileName();
    bool isEmpty();
    QString colorDialog();
    int getIndex(const QString &tabName);
    void buildComponents();
    void buildMenu();
    void buildThemeList();
    void applyLayout();
    void applyStyle();
    void makeConnections();
    void createFile(const QString &fileToCreate);
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
    void onFont();
    void updateTitle();
    void updateCursorPosition();
    void updateConnect();
    void synthaxicHighlighting();
    void checkFileLanguage();
    void setTabSpace();
};
#endif // NOTEPAD_HPP
