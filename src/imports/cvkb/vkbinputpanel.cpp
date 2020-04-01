/*
 * The MIT License (MIT)
 *
 * Copyright (C) 2020 CELLINK AB <info@cellink.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "vkbinputpanel.h"
#include "vkbinputdelegate.h"
#include "vkbinputlayoutattached.h"
#include "vkbinputlayoutitem.h"
#include "vkbinputpopup.h"

#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlengine.h>
#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbInputPanel::VkbInputPanel(QObject *parent)
    : QQuickPopup(parent),
      m_layoutItem(new VkbInputLayoutItem(popupItem()))
{
    setClosePolicy(CloseOnEscape);
    setContentItem(m_layoutItem);

    connect(this, &QQuickPopup::visibleChanged, this, &VkbInputPanel::visibleChanged);
    connect(this, &QQuickPopup::openedChanged, this, &VkbInputPanel::animatingChanged);
    connect(this, &QQuickPopup::visibleChanged, this, &VkbInputPanel::animatingChanged);
    connect(this, &QQuickPopup::localeChanged, this, &VkbInputPanel::localeChanged);

    // ### TODO: fix QQuickPopup::spacingChange()
    if (QQuickControl *control = qobject_cast<QQuickControl *>(popupItem()))
        connect(control, &QQuickControl::spacingChanged, this, &VkbInputPanel::updateSpacing);
}

bool VkbInputPanel::isVisible() const
{
    return QQuickPopup::isVisible();
}

void VkbInputPanel::setVisible(bool visible)
{
    QQuickPopup::setVisible(visible);
}

bool VkbInputPanel::isAnimating() const
{
    return isVisible() && !isOpened();
}

QRectF VkbInputPanel::rect() const
{
    return m_rect;
}

QLocale VkbInputPanel::locale() const
{
    return QQuickPopup::locale();
}

Qt::LayoutDirection VkbInputPanel::inputDirection() const
{
    return Qt::LeftToRight;
}

VkbInputLayout VkbInputPanel::layout() const
{
    return m_layoutItem->layout();
}

void VkbInputPanel::setLayout(const VkbInputLayout &layout)
{
    if (m_layoutItem->layout() == layout)
        return;

    m_layoutItem->setLayout(layout);
    updateButtons();
    emit layoutChanged();
}

QQmlListProperty<VkbInputDelegate> VkbInputPanel::delegates()
{
    return QQmlListProperty<VkbInputDelegate>(this, nullptr, delegates_append, delegates_count, delegates_at, delegates_clear);
}

void VkbInputPanel::handleKeyClick()
{
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(sender());
    if (!attached)
        return;

    emit keyClicked(attached->inputKey());
}

void VkbInputPanel::handleKeyPressAndHold()
{
    QQuickAbstractButton *button = qobject_cast<QQuickAbstractButton *>(sender());
    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(button);
    if (!attached)
        return;

    if (!attached->alt().isEmpty()) {
        VkbInputPopup *popup = createPopup(attached->inputKey(), button);
        if (!popup)
            return;

        popup->open();
        connect(popup, &QQuickPopup::closed, popup, &QObject::deleteLater);
        connect(popup, &VkbInputPopup::keySelected, this, &VkbInputPanel::keyClicked);
        connect(button, &QQuickAbstractButton::released, popup, &QQuickPopup::close);
    }

    emit keyPressAndHold(attached->inputKey());
}

void VkbInputPanel::componentComplete()
{
    QQuickPopup::componentComplete();
    updateButtons();
}

void VkbInputPanel::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickPopup::geometryChanged(newGeometry, oldGeometry);
    updateRect(newGeometry);
}

void VkbInputPanel::updateRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    m_rect = rect;
    emit rectChanged();
}

void VkbInputPanel::updateSpacing()
{
    m_layoutItem->setSpacing(spacing());
}

void VkbInputPanel::updateButtons()
{
    if (!isComponentComplete())
        return;

    QHash<VkbInputKey, QQuickAbstractButton *> newButtons;
    QHash<VkbInputKey, QQuickAbstractButton *> oldButtons = m_layoutItem->buttons();

    VkbInputLayout layout = m_layoutItem->layout();
    for (int r = 0; r < layout.rowCount(); ++r) {
        const QVector<VkbInputKey> row = layout.rowAt(r);
        for (int c = 0; c < row.count(); ++c) {
            const VkbInputKey &key = row.at(c);
            QQuickAbstractButton *button = oldButtons.take(key);
            if (!button) {
                button = createButton(key, m_layoutItem);
                connect(button, &QQuickAbstractButton::clicked, this, &VkbInputPanel::handleKeyClick);
                connect(button, &QQuickAbstractButton::pressAndHold, this, &VkbInputPanel::handleKeyPressAndHold);
            }
            newButtons.insert(key, button);
        }
    }

    for (QQuickAbstractButton *button : qAsConst(oldButtons))
        button->deleteLater();

    m_layoutItem->setButtons(newButtons);
}

VkbInputDelegate *VkbInputPanel::findDelegate(Qt::Key key) const
{
    auto it = std::find_if(m_delegates.cbegin(), m_delegates.cend(), [&key](VkbInputDelegate *delegate) { return delegate->key() == key; });
    if (it != m_delegates.cend())
        return *it;
    if (key != Qt::Key_unknown)
        return findDelegate(Qt::Key_unknown);
    return nullptr;
}

template <typename T>
static T *beginCreate(const VkbInputKey &key, QQmlComponent *component, QObject *parent)
{
    if (!component)
        return nullptr;

    QQmlContext *creationContext = component->creationContext();
    if (!creationContext)
        creationContext = qmlContext(parent);
    QQmlContext *context = new QQmlContext(creationContext, parent);

    QObject *instance = component->beginCreate(context);
    T *object = qobject_cast<T *>(instance);
    if (!object) {
        delete instance;
        return nullptr;
    }

    VkbInputLayoutAttached *attached = VkbInputLayoutAttached::qmlAttachedPropertiesObject(instance);
    if (attached)
        attached->setInputKey(key);

    return object;
}

QQuickAbstractButton *VkbInputPanel::createButton(const VkbInputKey &key, QQuickItem *parent) const
{
    VkbInputDelegate *delegate = findDelegate(key.key);
    if (!delegate)
        return nullptr;

    QQmlComponent *component = delegate->button();
    QQuickAbstractButton *button = beginCreate<QQuickAbstractButton>(key, component, parent);
    if (!button) {
        qWarning() << "VkbInputLayoutItem::createButton: button delegate for" << key.key << "is not a Button";
        return nullptr;
    }

    button->setParentItem(parent);
    button->setFocusPolicy(Qt::NoFocus);
    button->setAutoRepeat(key.autoRepeat);
    button->setCheckable(key.checkable);
    if (key.checked)
        button->setChecked(true);
    component->completeCreate();
    return button;
}

VkbInputPopup *VkbInputPanel::createPopup(const VkbInputKey &key, QQuickAbstractButton *button) const
{
    VkbInputDelegate *delegate = findDelegate(key.key);
    if (!delegate)
        return nullptr;

    QQmlComponent *component = delegate->popup();
    VkbInputPopup *popup = beginCreate<VkbInputPopup>(key, component, button);
    if (!popup) {
        qWarning() << "VkbInputLayout::createPopup: popup delegate for" << key.key << "is not an InputPopup";
        return nullptr;
    }

    popup->setParentItem(button);
    component->completeCreate();
    return popup;
}

void VkbInputPanel::delegates_append(QQmlListProperty<VkbInputDelegate> *property, VkbInputDelegate *delegate)
{
    VkbInputPanel *that = static_cast<VkbInputPanel *>(property->object);
    that->m_delegates.append(delegate);
    that->updateButtons();
}

int VkbInputPanel::delegates_count(QQmlListProperty<VkbInputDelegate> *property)
{
    VkbInputPanel *that = static_cast<VkbInputPanel *>(property->object);
    return that->m_delegates.count();
}

VkbInputDelegate *VkbInputPanel::delegates_at(QQmlListProperty<VkbInputDelegate> *property, int index)
{
    VkbInputPanel *that = static_cast<VkbInputPanel *>(property->object);
    return that->m_delegates.value(index);
}

void VkbInputPanel::delegates_clear(QQmlListProperty<VkbInputDelegate> *property)
{
    VkbInputPanel *that = static_cast<VkbInputPanel *>(property->object);
    that->m_delegates.clear();
    that->updateButtons();
}
