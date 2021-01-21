#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QSpinBox>

class Settings : public QDialog
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
private:
    QLabel *tabSpaceIndication;
    QSpinBox *spinTab;
    QLabel *themeIndication;
    QComboBox *themeChange;
    QPushButton *ok;
    QPushButton *cancel;
    void buildElements();
    void applyLayout();
signals:
    void themeChanged(QString themeName);
    void changeTabWidth(uint8_t width);
};

#endif // SETTINGS_HPP
