#include <QDirIterator>
#include "thememanager.hpp"

/// Constructor
ThemeManager::ThemeManager()
{
    for (auto const &themeName: DEFAULT_THEMES_NAMES)
    {
        themes.insert(themeName, Lazy<QString>{[themeName]() -> QString {
            auto themeNameCopy{themeName};
            themeNameCopy.remove(" ");
            const auto stylesheetFile{":style/" + themeNameCopy + ".qss"};
            return loadStyleFromFile(stylesheetFile);
        }});
    }
}

/// Get the names of the available themes
/// \return A lis containing the name of the themes
QList<QString> ThemeManager::themeNames() const
{
    return themes.keys();
}

/// Add a theme to the list of themes
/// \param themeName
/// \param theme
void ThemeManager::addTheme(const QString &themeName, const QString &theme)
{
    themes.insert(themeName, Lazy<QString>{[theme]() -> QString {
        return theme;
    }});
}

/// Get a theme content by its name
/// \param index
/// \return The stylesheet of the theme
QString ThemeManager::operator[](const QString &index)
{
    return themes.contains(index) ? themes[index].get() : "";
}

/// Load the content of a stylesheet by the fileName
/// Util function
/// \param fileName
/// \return The qss stylesheet contained in the file
QString ThemeManager::loadStyleFromFile(const QString &fileName)
{
    QFile styleFile{fileName};
    styleFile.open(QFile::ReadOnly);
    return QLatin1String(styleFile.readAll());
}

/// Load the themes saved in the user directory
void ThemeManager::loadSavedThemes()
{
    QString tmpThemeName;
    QDirIterator themeDirIt{THEME_DIR};
    while ((themeDirIt.hasNext()) && (QFileInfo(tmpThemeName = themeDirIt.next())).isFile())
    {
        themes.insert(QFileInfo{tmpThemeName}.fileName().split(".").front(),
                      Lazy<QString>([tmpThemeName]() -> QString {
                          return loadStyleFromFile(tmpThemeName);
                      }));
    }
}
