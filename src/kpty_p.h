/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2003, 2007 Oswald Buddenhagen <ossi@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef kpty_p_h
#define kpty_p_h

#include "kpty.h"

#include <config-pty.h>

#include <QByteArray>
#include <QString>

class KPtyPrivate
{
public:
    Q_DECLARE_PUBLIC(KPty)

    KPtyPrivate(KPty *parent);
    virtual ~KPtyPrivate();
#if !HAVE_OPENPTY
    bool chownpty(bool grant);
#endif

    int masterFd;
    int slaveFd;
    bool ownMaster : 1;

    QByteArray ttyName;
    QString utempterPath;

    KPty *q_ptr;
};

#endif
