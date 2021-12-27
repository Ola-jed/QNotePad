#ifndef QNOTEPAD_RECENTFILESMANAGER_HPP
#define QNOTEPAD_RECENTFILESMANAGER_HPP

#include <QSettings>

class RecentFilesManager
{
public:
    RecentFilesManager();
    [[nodiscard]] QList<QVariant> recentFiles() const;
    void addRecentFile(const QString &filePath);
    const QString RECENT_FILES = "recent files";

private:
    QList<QVariant> recentFilesList;
    QSettings settings{".settings/setting.ini", QSettings::IniFormat};
};

#endif //QNOTEPAD_RECENTFILESMANAGER_HPP
