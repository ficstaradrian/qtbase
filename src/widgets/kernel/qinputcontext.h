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

/****************************************************************************
**
** Definition of QInputContext class
**
** Copyright (C) 2003-2004 immodule for Qt Project.  All rights reserved.
**
** This file is written to contribute to Nokia Corporation and/or its subsidiary(-ies) under their own
** license. You may use this file under your Qt license. Following
** description is copied from their original file headers. Contact
** immodule-qt@freedesktop.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifndef QINPUTCONTEXT_H
#define QINPUTCONTEXT_H

#include <QtCore/qobject.h>
#include <QtCore/qglobal.h>
#include <QtGui/qevent.h>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include <QtWidgets/qaction.h>

#ifndef QT_NO_IM

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QWidget;
class QFont;
class QPopupMenu;
class QInputContextPrivate;

class Q_WIDGETS_EXPORT QInputContext : public QObject
{
    Q_OBJECT
public:
    explicit QInputContext(QObject* parent = 0);
    virtual ~QInputContext();

    virtual QString identifierName();
    virtual QString language();

    virtual void reset();
    virtual void update();

    virtual void mouseHandler( int x, QMouseEvent *event);
    virtual QFont font() const;

    QWidget *focusWidget() const;
    virtual void setFocusWidget( QWidget *w );

    virtual void widgetDestroyed(QWidget *w);

    virtual QList<QAction *> actions();

    void sendEvent(const QInputMethodEvent &event);

    virtual bool isComposing() const { return false; }

    enum StandardFormat {
        PreeditFormat,
        SelectionFormat
    };
    QTextFormat standardFormat(StandardFormat s) const;
private:
    friend class QWidget;
    friend class QWidgetPrivate;
    friend class QInputContextFactory;
    friend class QApplication;
private:   // Disabled copy constructor and operator=
    QInputContext( const QInputContext & );
    QInputContext &operator=( const QInputContext & );

};

QT_END_NAMESPACE

QT_END_HEADER

#endif //Q_NO_IM

#endif // QINPUTCONTEXT_H
