/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qquickabstracttextfield_p.h"

#include <QtQuick/private/qquickitem_p.h>
#include <QtQuick/private/qquicktext_p.h>
#include <QtQuick/private/qquickclipnode_p.h>

QT_BEGIN_NAMESPACE

class QQuickAbstractTextFieldPrivate
{
    Q_DECLARE_PUBLIC(QQuickAbstractTextField)

public:
    QQuickAbstractTextFieldPrivate() : background(Q_NULLPTR), placeholder(Q_NULLPTR) { }

    QQuickItem *background;
    QQuickText *placeholder;
    QQuickAbstractTextField *q_ptr;
};

QQuickAbstractTextField::QQuickAbstractTextField(QQuickItem *parent) :
    QQuickTextInput(parent), d_ptr(new QQuickAbstractTextFieldPrivate)
{
    Q_D(QQuickAbstractTextField);
    d->q_ptr = this;
}

QQuickAbstractTextField::~QQuickAbstractTextField()
{
}

QQuickItem *QQuickAbstractTextField::background() const
{
    Q_D(const QQuickAbstractTextField);
    return d->background;
}

void QQuickAbstractTextField::setBackground(QQuickItem *background)
{
    Q_D(QQuickAbstractTextField);
    if (d->background != background) {
        delete d->background;
        d->background = background;
        if (background) {
            background->setParentItem(this);
            if (qFuzzyIsNull(background->z()))
                background->setZ(-1);
        }
        emit backgroundChanged();
    }
}

QQuickText *QQuickAbstractTextField::placeholder() const
{
    Q_D(const QQuickAbstractTextField);
    return d->placeholder;
}

void QQuickAbstractTextField::setPlaceholder(QQuickText *placeholder)
{
    Q_D(QQuickAbstractTextField);
    if (d->placeholder != placeholder) {
        delete d->placeholder;
        d->placeholder = placeholder;
        if (placeholder && !placeholder->parentItem())
            placeholder->setParentItem(this);
        emit placeholderChanged();
    }
}

void QQuickAbstractTextField::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_D(QQuickAbstractTextField);
    QQuickTextInput::geometryChanged(newGeometry, oldGeometry);
    if (d->background) {
        QQuickItemPrivate *p = QQuickItemPrivate::get(d->background);
        if (!p->widthValid || qFuzzyCompare(d->background->width(), oldGeometry.width()))
            d->background->setWidth(newGeometry.width());
        if (!p->heightValid || qFuzzyCompare(d->background->height(), oldGeometry.height()))
            d->background->setHeight(newGeometry.height());
    }
}

QSGNode *QQuickAbstractTextField::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    QQuickDefaultClipNode *clipNode = static_cast<QQuickDefaultClipNode *>(oldNode);
    if (!clipNode)
        clipNode = new QQuickDefaultClipNode(QRectF());

    clipNode->setRect(clipRect().adjusted(leftPadding(), topPadding(), -rightPadding(), -bottomPadding()));
    clipNode->update();

    QSGNode *textNode = QQuickTextInput::updatePaintNode(clipNode->firstChild(), data);
    if (!textNode->parent())
        clipNode->appendChildNode(textNode);

    return clipNode;
}

QT_END_NAMESPACE
