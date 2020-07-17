/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzCore/base.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <QtWidgets/QWidget>
#include "PropertyEditorAPI.h"

class QCheckBox;

namespace AzToolsFramework
{
    class PropertyCheckBoxCtrl
        : public QWidget
    {
        Q_OBJECT
    public:
        AZ_CLASS_ALLOCATOR(PropertyCheckBoxCtrl, AZ::SystemAllocator, 0);

        PropertyCheckBoxCtrl(QWidget* parent = nullptr);
        virtual ~PropertyCheckBoxCtrl() = default;

        bool value() const;

        QWidget* GetFirstInTabOrder();
        QWidget* GetLastInTabOrder();
        void UpdateTabOrder();

        void SetCheckBoxToolTip(const char* text);

    signals:
        void valueChanged(bool newValue);

    public slots:
        void setValue(bool val);

    protected slots:
        void onStateChanged(int value);

    private:
        QCheckBox* m_checkBox;
    };

    class CheckBoxHandlerCommon
        : public QObject
    {
        Q_OBJECT
    public:
        AZ_CLASS_ALLOCATOR(CheckBoxHandlerCommon, AZ::SystemAllocator, 0);
        QWidget* CreateGUICommon(QWidget* parent);
        void ConsumeAttributeCommon(PropertyCheckBoxCtrl* widget, AZ::u32 attrib, PropertyAttributeReader* attrValue, const char* debugName);
    };

    template <typename ValueType>
    class PropertyCheckBoxHandlerCommon
        : public CheckBoxHandlerCommon
        , public PropertyHandler<ValueType, PropertyCheckBoxCtrl>
    {
    public:
        AZ::u32 GetHandlerName(void) const override { return AZ::Edit::UIHandlers::CheckBox; }
        bool IsDefaultHandler() const override { return true; }
        QWidget* GetFirstInTabOrder(PropertyCheckBoxCtrl* widget) override { return widget->GetFirstInTabOrder(); }
        QWidget* GetLastInTabOrder(PropertyCheckBoxCtrl* widget) override { return widget->GetLastInTabOrder(); }
        void UpdateWidgetInternalTabbing(PropertyCheckBoxCtrl* widget) override { widget->UpdateTabOrder(); }

        void ConsumeAttribute(PropertyCheckBoxCtrl* widget, AZ::u32 attrib, PropertyAttributeReader* attrValue, const char* debugName) override;
    };

    class BoolPropertyCheckBoxHandler
        : public PropertyCheckBoxHandlerCommon<bool>
    {
        // this is a Qt Object purely so it can connect to slots with context.  This is the only reason its in this header.
        Q_OBJECT
    public:
        AZ_CLASS_ALLOCATOR(BoolPropertyCheckBoxHandler, AZ::SystemAllocator, 0);

        QWidget* CreateGUI(QWidget *parent) override;
 
        void WriteGUIValuesIntoProperty(size_t index, PropertyCheckBoxCtrl* widget, property_t& instance, InstanceDataNode* node) override;
        bool ReadValuesIntoGUI(size_t index, PropertyCheckBoxCtrl* widget, const property_t& instance, InstanceDataNode* node)  override;
    };

    // A CheckBoxGenericHandler is used to register a checkbox widget that doesn't depend on the underlying type
    // This is useful if we want UI checkbox element that doesn't have any specific underlying storage
    class CheckBoxGenericHandler
        : public CheckBoxHandlerCommon
        , public GenericPropertyHandler<PropertyCheckBoxCtrl>
    {
        Q_OBJECT
    public:
        AZ_CLASS_ALLOCATOR(CheckBoxGenericHandler, AZ::SystemAllocator, 0);

        QWidget* CreateGUI(QWidget* parent) override;
        AZ::u32 GetHandlerName() const override { return AZ::Edit::UIHandlers::CheckBox; }
        void WriteGUIValuesIntoProperty(size_t index, PropertyCheckBoxCtrl* widget, void* value, const AZ::Uuid& propertyType) override;
        bool ReadValueIntoGUI(size_t index, PropertyCheckBoxCtrl* widget, void* value, const AZ::Uuid& propertyType) override;
        void ConsumeAttribute(PropertyCheckBoxCtrl* widget, AZ::u32 attrib, PropertyAttributeReader* attrValue, const char* debugName) override;
    };

    void RegisterCheckBoxHandlers();
};
