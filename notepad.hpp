#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include "style.hpp"
#include "keywords.hpp"
#include <QScreen>
#include <QFontMetrics>
#include <QStyle>
#include <QIcon>
#include <QDebug>
#include <QMap>
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
#include <QComboBox>
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
#include <QMimeData>
#include <QList>
#include <QAction>
#include <QApplication>

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
    const QMap<QString,QString> THEME_NAMES{
        {"Amoled",Amoled},
        {"Aqua",Aqua},
        {"Console",Console},
        {"Difness",Diffness},
        {"Elegant Dark",ElegantDark},
        {"Mac",Mac},
        {"Manjaro",Manjaro},
        {"Material Dark",Material},
        {"Obit",Obit},
        {"Ubuntu",Ubuntu}
    };
    const int TAB_SPACE = 4;
    // Member variables.
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
    QCheckBox *autoSaveCheckBox;
    QComboBox *themeChoice;
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
    void setTabSpace();
};
#endif // NOTEPAD_HPP
