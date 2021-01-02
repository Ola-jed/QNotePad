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
#include <QVBoxLayout>
#include <QApplication>

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();

private:
    QString fileName = "";
    bool isSaved = false;
    QPushButton *newFile;
    QPushButton *openFile;
    QPushButton *saveFile;
    QPushButton *quit;
    QPushButton *colorText;
    QPlainTextEdit *textEdit;
    QLabel *label;
    QCheckBox *autoSaveCheckBox;
private slots:
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onQuit();
    void onTextChanged();
    void onColorChanged();
    void onAutoSave();
};
#endif // NOTEPAD_HPP
