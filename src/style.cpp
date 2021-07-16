#include "style.hpp"

/// Load a qt stylesheet from a given file
QString loadStyleFromFile(const QString &fileName)
{
    QFile styleFile{fileName};
    styleFile.open(QFile::ReadOnly);
    return QLatin1String(styleFile.readAll());
}