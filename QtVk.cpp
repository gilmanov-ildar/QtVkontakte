/**
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 *
 * @file QtVk.cpp
 * @brief
 * @details
 * @date Created on 08.12.2015
 * @copyright 2015 DDwarf LLC, <dev@ddwarf.org>
 * @author Gilmanov Ildar <dev@ddwarf.org>
 */

#include <QtQml>
#include <QDebug>
#include <QPixmap>
#include <QtQuick/QQuickItemGrabResult>

#include "QtVk.h"

namespace DDwarf {
namespace Social {

QtVk *QtVk::m_instance = nullptr;

void QtVk::initStatic()
{
    if(m_instance)
    {
        qWarning() << QString("QtVk is already initialized");
        return;
    }

    m_instance = new QtVk();

    qmlRegisterSingletonType<QtVk>("org.ddwarf.social", 1, 0, "QtVk", &QtVk::qtVkProvider);
}

QtVk *QtVk::instance()
{
    if(!m_instance)
    {
        qWarning() << "QtVk had not initialized. You must call QtVk::initStatic() method after Application created";
    }

    return m_instance;
}

QtVk::QtVk(QObject *parent) :
    QObject(parent)
{
}

QObject *QtVk::qtVkProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return m_instance;
}

void QtVk::openShareDialog(const QString &textToPost,
                           const QVariantList &photoLinks,
                           const QVariantList &photos,
                           const QString &linkTitle,
                           const QString &linkUrl)
{
    QStringList photoStringLinks;

    for(QVariantList::const_iterator it = photoLinks.cbegin(); it != photoLinks.cend(); ++it)
    {
        const QVariant &var = *it;

        if(var.canConvert<QString>())
        {
            photoStringLinks.push_back(var.toString());
        }
        else
        {
            qWarning() << "Incorrect value for photos";
            return;
        }
    }

    QList<QPixmap> pixmapList;

    for(QVariantList::const_iterator it = photos.cbegin(); it != photos.cend(); ++it)
    {
        const QVariant &var = *it;

        if(var.canConvert<QPixmap>())
        {
            pixmapList.push_back(var.value<QPixmap>());
        }
        else if(var.canConvert<QQuickItemGrabResult*>())
        {
            QQuickItemGrabResult *grabResult = var.value<QQuickItemGrabResult*>();
            pixmapList.push_back(QPixmap::fromImage(grabResult->image()));
        }
        else
        {
            qWarning() << "Incorrect value for photos";
            return;
        }
    }

    openShareDialog(textToPost,
                    photoStringLinks,
                    pixmapList,
                    linkTitle,
                    linkUrl);
}

} // namespace Social
} // namespace DDwarf

