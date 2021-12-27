#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "fileextensionsloader.hpp"
#include "utils.hpp"

/// Constructor
/// Check if a random value exists in the settings and then fetch the settings
/// If the value exists, load the json data
/// Otherwise, it writes the settings
FileExtensionsLoader::FileExtensionsLoader()
{
    if (settings.contains("Extensions/Asp"))
    {
        settings.beginGroup(EXTENSIONS_GROUP);
        for (auto const &supportedLanguage: settings.childKeys())
        {
            const auto languageExtensions = settings.value(supportedLanguage).toStringList();
            fileExtensions.insert(supportedLanguage,
                                  QSet<QString>(languageExtensions.begin(), languageExtensions.end()));
        }
        settings.endGroup();
    }
    else
    {
        saveJsonToSettings();
    }
}

/// Get all the files types supported with their extensions
/// \return A QHash<QString,QSet<QString>> containing all the extensions
const QHash<QString, QSet<QString>> &FileExtensionsLoader::extensions() const
{
    return fileExtensions;
}

/// Save the data in the json file to the settings
void FileExtensionsLoader::saveJsonToSettings()
{
    QFile f{":lang_extensions.json"};
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    auto const jsonDocument = QJsonDocument::fromJson(f.readAll());
    auto const jsonArray = jsonDocument.array();
    settings.beginGroup(EXTENSIONS_GROUP);
    settings.remove("");
    fileExtensions.clear();
    fileExtensions.reserve(jsonArray.size());
    for (auto const &value: jsonArray)
    {
        auto const obj = value.toObject();
        auto const language = obj.keys().first();
        auto const extensions = obj.value(language).toArray().toVariantList();
        settings.setValue(language, extensions);
        fileExtensions.insert(language, utils::variantListToStringSet(extensions));
    }
    settings.endGroup();
}
