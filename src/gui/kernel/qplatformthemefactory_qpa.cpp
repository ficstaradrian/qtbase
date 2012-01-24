/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qplatformthemefactory_qpa_p.h"
#include <QPlatformThemePlugin>
#include "private/qfactoryloader_p.h"
#include "qmutex.h"

#include "qguiapplication.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

#if !defined(QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader,
    (QPlatformThemeFactoryInterface_iid, QLatin1String("/platformthemes"), Qt::CaseInsensitive))
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, directLoader,
                          (QPlatformThemeFactoryInterface_iid, QLatin1String(""), Qt::CaseInsensitive))
#endif

QPlatformTheme *QPlatformThemeFactory::create(const QString& key, const QString &platformPluginPath)
{
    QPlatformTheme *ret = 0;
    QStringList paramList = key.split(QLatin1Char(':'));
    QString platform = paramList.takeFirst().toLower();

#if !defined(QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
    // Try loading the plugin from platformPluginPath first:
    if (!platformPluginPath.isEmpty()) {
        QCoreApplication::addLibraryPath(platformPluginPath);
        if (QPlatformThemeFactoryInterface *factory =
            qobject_cast<QPlatformThemeFactoryInterface*>(directLoader()->instance(platform)))
            ret = factory->create(key, paramList);

        if (ret)
            return ret;
    }
    if (QPlatformThemeFactoryInterface *factory = qobject_cast<QPlatformThemeFactoryInterface*>(loader()->instance(platform)))
        ret = factory->create(platform, paramList);
#endif

    return ret;
}

/*!
    Returns the list of valid keys, i.e. the keys this factory can
    create styles for.

    \sa create()
*/
QStringList QPlatformThemeFactory::keys(const QString &platformPluginPath)
{
#if !defined(QT_NO_LIBRARY) && !defined(QT_NO_SETTINGS)
    QStringList list;

    if (!platformPluginPath.isEmpty()) {
        QCoreApplication::addLibraryPath(platformPluginPath);
        foreach (const QString &key, directLoader()->keys()) {
            list += key + QString(QLatin1String(" (from %1)")).arg(platformPluginPath);
        }
    }

    list += loader()->keys();
#else
    QStringList list;
#endif
    return list;
}

QT_END_NAMESPACE
