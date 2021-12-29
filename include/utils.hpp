#ifndef QNOTEPAD_UTILS_HPP
#define QNOTEPAD_UTILS_HPP

#include <QSet>
using QStringSet = QSet<QString>;
namespace utils
{
    QStringSet variantListToStringSet(const QVariantList &variantList)
    {
        QStringSet stringSet{};
        stringSet.reserve(variantList.size());
        for (auto const &strVariant: variantList)
        {
            stringSet.insert(strVariant.toString());
        }
        return stringSet;
    }
}

#endif //QNOTEPAD_UTILS_HPP
