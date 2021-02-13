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
#include <QList>
#include <QFileDialog>

class Settings : public QDialog
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr,const QList<QString> &themes = {},uint8_t tabspace = 4);
private:
    QLabel *tabSpaceIndication;
    QSpinBox *spinTab;
    QLabel *themeIndication;
    QComboBox *themeChange;
    QPushButton *loadUserTheme;
    QPushButton *ok;
    QPushButton *cancel;
    void buildElements(const QList<QString> &themes,uint8_t tabspace);
    void applyLayout();
private slots:
    void localThemeLoader();
signals:
    void themeChanged(QString themeName);
    void changeTabWidth(uint8_t width);
    void localThemeSelected(QString fleThemeName);
};

#endif // SETTINGS_HPP
