#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
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
    explicit Settings(QWidget *parent = nullptr,const QList<QString> &themes = {},const QString terminalName = "konsole",uint8_t tabspace = 4);
private:
    QLabel *tabSpaceIndication;
    QSpinBox *spinTab;
    QLabel *themeIndication;
    QComboBox *themeChange;
    QLabel *terminalIndication;
    QLineEdit *terminalText;
    QPushButton *loadUserTheme;
    QPushButton *ok;
    QPushButton *cancel;
    void buildElements(const QList<QString> &themes,const QString terminalName,uint8_t tabspace);
    void applyLayout();
private slots:
    void localThemeLoader();
signals:
    void themeChanged(QString themeName);
    void changeTabWidth(uint8_t width);
    void localThemeSelected(QString fleThemeName);
    void terminalChanged(QString terminalName);
};

#endif // SETTINGS_HPP
