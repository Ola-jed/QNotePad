#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include <QMainWindow>
#include <QString>
#include <QCheckBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QFont>
#include <QLabel>
#include <QTextStream>
#include <QStringList>
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>
#include <QHBoxLayout>
#include <QFont>
#include <QVBoxLayout>
#include <QTabWidget>
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
    QPushButton *newFile;
    QTabWidget *tabView;
    QPushButton *openFile;
    QPushButton *saveFile;
    QPushButton *quit;
    QPushButton *colorText;
    int getIndex(const QString &tabName);
    QCheckBox *autoSaveCheckBox;
private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onCloseFile(const int &index);
    void onQuit();
    void onColorChanged();
    void onAutoSave();
    void onTextModified();
};
#endif // NOTEPAD_HPP
