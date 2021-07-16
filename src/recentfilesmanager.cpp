#include "recentfilesmanager.hpp"

RecentFilesManager::RecentFilesManager()
{
    recentFilesList = (settings.contains(RECENT_FILES))
            ? settings.value(RECENT_FILES).toList()
            : QList<QVariant>{};
}

/// Get all the recent files
QList <QVariant> RecentFilesManager::recentFiles() const
{
    return recentFilesList;
}

/// Add a file to the recent files and save
/// This container should not contain more than 10 elements
/// \param filePath
void RecentFilesManager::addRecentFile(const QString &filePath)
{
    if(recentFilesList.size() >= 10)
    {
        recentFilesList.pop_front();
    }
    if(!recentFilesList.contains(filePath))
    {
        recentFilesList.push_back(filePath);
        settings.setValue(RECENT_FILES,recentFilesList);
    }
}