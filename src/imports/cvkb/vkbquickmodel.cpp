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

#include "vkbquickmodel.h"
#include "vkbquickdelegate.h"
#include "vkbquicklayout.h"
#include "vkbquickpopup.h"

#include <QtQml/qqmlcomponent.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlengine.h>
#include <QtQuickTemplates2/private/qquickabstractbutton_p.h>

VkbQuickModel::VkbQuickModel(QObject *parent) : QObject(parent)
{
}

QQmlListProperty<VkbQuickDelegate> VkbQuickModel::delegates()
{
    return QQmlListProperty<VkbQuickDelegate>(this, nullptr, delegates_append, delegates_count, delegates_at, delegates_clear);
}

VkbQuickDelegate *VkbQuickModel::findDelegate(Qt::Key key) const
{
    auto it = std::find_if(m_delegates.cbegin(), m_delegates.cend(), [&key](VkbQuickDelegate *delegate) { return delegate->key() == key; });
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

    VkbQuickLayoutAttached *attached = VkbQuickLayout::qmlAttachedPropertiesObject(instance);
    if (attached)
        attached->setInputKey(key);

    return object;
}

QQuickAbstractButton *VkbQuickModel::createButton(const VkbInputKey &key, QQuickItem *parent) const
{
    VkbQuickDelegate *delegate = findDelegate(key.key);
    if (!delegate)
        return nullptr;

    QQmlComponent *component = delegate->button();
    QQuickAbstractButton *button = beginCreate<QQuickAbstractButton>(key, component, parent);
    if (!button) {
        qWarning() << "VkbQuickModel::createButton: button delegate for" << key.key << "is not a Button";
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

VkbQuickPopup *VkbQuickModel::createPopup(const VkbInputKey &key, QQuickAbstractButton *button) const
{
    VkbQuickDelegate *delegate = findDelegate(key.key);
    if (!delegate)
        return nullptr;

    QQmlComponent *component = delegate->popup();
    VkbQuickPopup *popup = beginCreate<VkbQuickPopup>(key, component, button);
    if (!popup) {
        qWarning() << "VkbQuickModel::createPopup: popup delegate for" << key.key << "is not an InputPopup";
        return nullptr;
    }

    popup->setParentItem(button);
    popup->setAlt(key.alt);
    component->completeCreate();
    return popup;
}

void VkbQuickModel::delegates_append(QQmlListProperty<VkbQuickDelegate> *property, VkbQuickDelegate *delegate)
{
    VkbQuickModel *that = static_cast<VkbQuickModel *>(property->object);
    that->m_delegates.append(delegate);
    emit that->delegatesChanged();
}

int VkbQuickModel::delegates_count(QQmlListProperty<VkbQuickDelegate> *property)
{
    VkbQuickModel *that = static_cast<VkbQuickModel *>(property->object);
    return that->m_delegates.count();
}

VkbQuickDelegate *VkbQuickModel::delegates_at(QQmlListProperty<VkbQuickDelegate> *property, int index)
{
    VkbQuickModel *that = static_cast<VkbQuickModel *>(property->object);
    return that->m_delegates.value(index);
}

void VkbQuickModel::delegates_clear(QQmlListProperty<VkbQuickDelegate> *property)
{
    VkbQuickModel *that = static_cast<VkbQuickModel *>(property->object);
    that->m_delegates.clear();
    emit that->delegatesChanged();
}
