#ifndef QNOTEPAD_FILEMANAGER_HPP
#define QNOTEPAD_FILEMANAGER_HPP

#include <QString>
#include <QFile>
#include <optional>

class FileManager
{
public:
    FileManager() = default;
    static bool rename(const QString& originalName,const QString& newName);
    static bool copy(const QString& originalFile,const QString& fileCopy);
    static bool createFile(const QString& fileName);
    static std::optional<QString> readAll(const QString &fileName);
    static bool saveIntoFile(const QString& fileName,const QString& content);
};


#endif //QNOTEPAD_FILEMANAGER_HPP
