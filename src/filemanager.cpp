#include <QTextStream>
#include "filemanager.hpp"

/// Rename a file
/// \param originalName
/// \param newName
/// \return If the renaming suceeded or not
bool FileManager::rename(const QString &originalName, const QString &newName)
{
    return QFile::rename(originalName, newName);
}

/// Copy a file content into another
/// \param originalFile
/// \param fileCopy
/// \return If the copy suceeded or not
bool FileManager::copy(const QString &originalFile, const QString &fileCopy)
{
    return QFile::copy(originalFile, fileCopy);
}

/// Create a new file
/// \param fileName
/// \return If the file creation suceeded or not
bool FileManager::createFile(const QString &fileName)
{
    QFile file{fileName};
    if (!file.open(QIODevice::ReadWrite))
    {
        return false;
    }
    file.close();
    return true;
}

/// Read all the content of the file in a QString
/// \param fileName
/// \return The content of the file (or nothing if the file could not be opened)
std::optional<QString> FileManager::readAll(const QString &fileName)
{
    QFile fileToRead{fileName};
    if (!fileToRead.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return std::nullopt;
    }
    QTextStream in{&fileToRead};
    const auto content = in.readAll();
    fileToRead.close();
    return std::optional<QString>{content};
}

/// Save the content into a file
/// \param fileName
/// \param content
/// \return If the save succeeded or not
bool FileManager::saveIntoFile(const QString &fileName, const QString &content)
{
    QFile saveFile{fileName};
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }
    QTextStream out{&saveFile};
    out << content;
    saveFile.close();
    return true;
}
