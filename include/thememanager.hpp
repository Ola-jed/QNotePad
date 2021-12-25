#ifndef QNOTEPAD_THEMEMANAGER_HPP
#define QNOTEPAD_THEMEMANAGER_HPP

#include "lazy.hpp"
#include <QString>
#include <QDir>
#include <QSet>

class ThemeManager
{
private:
    const QString THEME_DIR{QDir::homePath() + "/.qnotepad_themes"};
    const QSet<QString> DEFAULT_THEMES_NAMES{
            {"Adaptic"},
            {"Amoled"},
            {"Aqua"},
            {"Console"},
            {"Diffness"},
            {"Dtor"},
            {"Elegant Dark"},
            {"Fibrary"},
            {"Genetive"},
            {"Irrorater"},
            {"Mac"},
            {"Manjaro"},
            {"Material Dark"},
            {"Obit"},
            {"Visual"},
            {"Ubuntu"},
            {"World"}
    };
    QHash<QString, Lazy<QString>> themes{};

public:
    ThemeManager();
    [[nodiscard]] QList<QString> themeNames() const;
    void addTheme(const QString &themeName, const QString &theme);
    void loadSavedThemes();
    QString operator[](const QString &index);
    static QString loadStyleFromFile(const QString &fileName);
};


#endif //QNOTEPAD_THEMEMANAGER_HPP
