#include "style.hpp"

QString loadStyleFromFile(const QString &fileName)
{
    QFile styleFile{fileName};
    styleFile.open(QFile::ReadOnly);
    return QLatin1String(styleFile.readAll());
}