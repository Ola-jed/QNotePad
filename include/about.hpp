#ifndef ABOUT_HPP
#define ABOUT_HPP

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>

class About : public QDialog
{
Q_OBJECT
public:
    explicit About(QWidget *parent = nullptr);
private:
    QLabel      *iconLabel;
    QLabel      *label;
    QPushButton *okBtn;

    void buildComponents();
    void applyLayout();
    void buildWindow();

    const QString INFO{"QNotePad is a free and open source text"
                       " editor written with Qt5.\n"
                       "It has word prediction and a basic syntaxic coloration.\n"
                       "Github repository : "
                       "<a href=\"https://github.com/Ola-jed/QNotePad\">https://github.com/Ola-jed/QNotePad</a>"};
};

#endif // ABOUT_HPP