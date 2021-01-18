#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include "keywords.hpp"
#include <QScreen>
#include <QStyle>
#include <QIcon>
#include <QDebug>
#include <QMainWindow>
#include <QCheckBox>
#include <QProcess>
#include <QFontDialog>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QStatusBar>
#include <QFont>
#include <QLabel>
#include <QTextStream>
#include <QStringList>
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>
#include <QHBoxLayout>
#include <QFont>
#include <QTextDocument>
#include <QVBoxLayout>
#include <QTextBlock>
#include <QTabWidget>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QApplication>

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();

private:
    QString fileName();
    bool isEmpty();
    bool isSaved = false;
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
    int getIndex(const QString &tabName);
    QCheckBox *autoSaveCheckBox;
    QString colorDialog();

    void buildComponents();
    void buildMenu();
    void applyLayout();
    void applyStyle();
private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onNewFileSave();
    void onExistingFileSave();
    void onCloseFile(const int &index);
    void onQuit();
    void onColorChanged();
    void onBackgroundColorChanged();
    void onAutoSave();
    void onTextModified();
    void onTerminal();
    void onFont();
    void updateTitle();
    void updateCursorPosition();
    void updateConnect();
    void synthaxicHighlighting();
};
#endif // NOTEPAD_HPP
