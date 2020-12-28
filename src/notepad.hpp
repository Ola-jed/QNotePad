#ifndef NOTEPAD_HPP
#define NOTEPAD_HPP

#include <QMainWindow>
#include <QString>
#include <QPushButton>
#include <QTextEdit>
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


QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();

private:
    Ui::Notepad *ui;
    QString fileName = "";
    bool isSaved = false;

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
