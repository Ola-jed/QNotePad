#ifndef QNOTEPAD_FILEEXTENSIONSLOADER_HPP
#define QNOTEPAD_FILEEXTENSIONSLOADER_HPP

#include <QString>
#include <QHash>
#include <QSettings>

class FileExtensionsLoader
{
private:
    const QString EXTENSIONS_GROUP = "Extensions";
    QSettings settings{".settings/setting.ini", QSettings::IniFormat};
    QHash<QString, QSet<QString>> fileExtensions;
    void saveJsonToSettings();
public:
    FileExtensionsLoader();
    [[nodiscard]] const QHash<QString, QSet<QString>>& extensions() const;
};

#endif //QNOTEPAD_FILEEXTENSIONSLOADER_HPP
