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

#include "vkbinputmodel.h"
#include "vkbinputdelegate.h"
#include "vkbinputlayoutattached.h"
#include "vkbinputpopup.h"

#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlengine.h>
#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbInputModel::VkbInputModel(QObject *parent) : QObject(parent)
{
}

QQmlListProperty<VkbInputDelegate> VkbInputModel::delegates()
{
    return QQmlListProperty<VkbInputDelegate>(this, nullptr, delegates_append, delegates_count, delegates_at, delegates_clear);
}

VkbInputDelegate *VkbInputModel::findDelegate(Qt::Key key) const
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

QQuickAbstractButton *VkbInputModel::createButton(const VkbInputKey &key, QQuickItem *parent) const
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

VkbInputPopup *VkbInputModel::createPopup(const VkbInputKey &key, QQuickAbstractButton *button) const
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
    popup->setAlt(key.alt);
    component->completeCreate();
    return popup;
}

void VkbInputModel::delegates_append(QQmlListProperty<VkbInputDelegate> *property, VkbInputDelegate *delegate)
{
    VkbInputModel *that = static_cast<VkbInputModel *>(property->object);
    that->m_delegates.append(delegate);
    emit that->delegatesChanged();
}

int VkbInputModel::delegates_count(QQmlListProperty<VkbInputDelegate> *property)
{
    VkbInputModel *that = static_cast<VkbInputModel *>(property->object);
    return that->m_delegates.count();
}

VkbInputDelegate *VkbInputModel::delegates_at(QQmlListProperty<VkbInputDelegate> *property, int index)
{
    VkbInputModel *that = static_cast<VkbInputModel *>(property->object);
    return that->m_delegates.value(index);
}

void VkbInputModel::delegates_clear(QQmlListProperty<VkbInputDelegate> *property)
{
    VkbInputModel *that = static_cast<VkbInputModel *>(property->object);
    that->m_delegates.clear();
    emit that->delegatesChanged();
}
