// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/WidgetJson.h>

#include <ftk/UI/Bellows.h>
#include <ftk/UI/CheckBox.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/DoubleEdit.h>
#include <ftk/UI/DoubleEditSlider.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/FloatEditSlider.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/GroupBox.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/IntEditSlider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>
#include <ftk/UI/Spacer.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Format.h>

#include <map>

namespace ftk
{
    namespace
    {
        const std::string idKey = "Id";

        std::string getString(
            const nlohmann::json& json,
            const std::string& key,
            const std::string& defaultValue = std::string())
        {
            std::string out = defaultValue;
            if (json.contains(key) && json.at(key).is_string())
            {
                out = json.at(key).get<std::string>();
            }
            return out;
        }

        std::string removeSpaces(const std::string& value)
        {
            std::string out;
            for (const char c : value)
            {
                if (c != ' ')
                {
                    out.push_back(c);
                }
            }
            return out;
        }

        // Parse an enum property, e.g. { "marginRole": "Margin" }; a
        // value that does not name an enumerant is an error, since a
        // silently ignored misspelling reads as the property not
        // working. The enum labels have spaces ("Spacing Small"), and
        // the code style spelling without them ("SpacingSmall") is
        // accepted too, since data written next to code tends to use
        // the code's names.
        template<typename T>
        bool getEnum(
            const nlohmann::json& json,
            const std::string& key,
            T& value,
            WidgetLoadResult& result)
        {
            bool out = false;
            if (json.contains(key) && json.at(key).is_string())
            {
                const std::string s = json.at(key).get<std::string>();
                out = from_string(s, value);
                if (!out)
                {
                    const std::string s2 = removeSpaces(s);
                    for (auto e = T::First;
                        e < T::Count;
                        e = static_cast<T>(static_cast<int>(e) + 1))
                    {
                        if (removeSpaces(to_string(e)) == s2)
                        {
                            value = e;
                            out = true;
                            break;
                        }
                    }
                }
                if (!out)
                {
                    result.errors.push_back(Format("Cannot parse {0}: \"{1}\"").
                        arg(key).
                        arg(s));
                }
            }
            return out;
        }

        template<typename T, typename U, typename V>
        void loadRange(
            const nlohmann::json& json,
            const std::shared_ptr<T>& widget,
            WidgetLoadResult& result)
        {
            if (json.contains("range"))
            {
                const auto& v = json.at("range");
                if (v.is_array() && 2 == v.size() && v[0].is_number() && v[1].is_number())
                {
                    widget->setRange(U(v[0].get<V>(), v[1].get<V>()));
                }
                else
                {
                    result.errors.push_back("Cannot parse range");
                }
            }
        }

        std::map<std::string, WidgetLoadFactory>& getRegistry();

        std::shared_ptr<IWidget> load(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            const std::shared_ptr<IWidget>& parent,
            WidgetLoadResult& result)
        {
            std::shared_ptr<IWidget> out;
            if (!json.is_object() || !json.contains("type"))
            {
                result.errors.push_back("Expected an object with a type");
                return out;
            }

            // The widget dump writes types with the namespace, e.g.
            // "ftk::PushButton"; accept those so a dump loads back.
            std::string type = json.at("type").get<std::string>();
            const size_t colons = type.rfind("::");
            if (colons != std::string::npos)
            {
                type.erase(0, colons + 2);
            }

            auto& registry = getRegistry();
            const auto i = registry.find(type);
            if (i == registry.end())
            {
                result.errors.push_back(Format("Unknown widget type: \"{0}\"").
                    arg(type));
                return out;
            }
            out = i->second(context, json, result);
            if (!out)
                return out;

            // The properties every widget has.
            const std::string id = getString(json, "id");
            if (!id.empty())
            {
                out->setProperty(idKey, id);
            }
            const std::string tooltip = getString(json, "tooltip");
            if (!tooltip.empty())
            {
                out->setTooltip(tooltip);
            }
            if (json.contains("enabled") && json.at("enabled").is_boolean())
            {
                out->setEnabled(json.at("enabled").get<bool>());
            }
            if (json.contains("visible") && json.at("visible").is_boolean())
            {
                out->setVisible(json.at("visible").get<bool>());
            }
            HAlign hAlign = HAlign::Fill;
            if (getEnum(json, "hAlign", hAlign, result))
            {
                out->setHAlign(hAlign);
            }
            VAlign vAlign = VAlign::Fill;
            if (getEnum(json, "vAlign", vAlign, result))
            {
                out->setVAlign(vAlign);
            }
            Stretch stretch = Stretch::Fixed;
            if (getEnum(json, "hStretch", stretch, result))
            {
                out->setHStretch(stretch);
            }
            if (getEnum(json, "vStretch", stretch, result))
            {
                out->setVStretch(stretch);
            }

            if (parent)
            {
                out->setParent(parent);
            }
            return out;
        }

        template<typename T>
        std::shared_ptr<IWidget> rowLayoutFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = T::create(context);
            SizeRole sizeRole = SizeRole::None;
            if (getEnum(json, "marginRole", sizeRole, result))
            {
                out->setMarginRole(sizeRole);
            }
            if (getEnum(json, "spacingRole", sizeRole, result))
            {
                out->setSpacingRole(sizeRole);
            }
            widgetLoadChildren(context, json, out, result);
            return out;
        }

        std::shared_ptr<IWidget> formLayoutFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = FormLayout::create(context);
            SizeRole sizeRole = SizeRole::None;
            if (getEnum(json, "marginRole", sizeRole, result))
            {
                out->setMarginRole(sizeRole);
            }
            if (getEnum(json, "spacingRole", sizeRole, result))
            {
                out->setSpacingRole(sizeRole);
            }
            // A form is rows of label and widget rather than plain
            // children, e.g.
            // { "rows": [ { "label": "Name:", "widget": {...} } ] }.
            if (json.contains("rows") && json.at("rows").is_array())
            {
                for (const auto& row : json.at("rows"))
                {
                    if (row.contains("widget"))
                    {
                        if (auto widget = load(context, row.at("widget"), nullptr, result))
                        {
                            out->addRow(getString(row, "label"), widget);
                        }
                    }
                }
            }
            return out;
        }

        template<typename T>
        std::shared_ptr<IWidget> textFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = T::create(context, getString(json, "text"));
            return out;
        }

        template<typename T>
        std::shared_ptr<IWidget> buttonFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = T::create(context, getString(json, "text"));
            const std::string icon = getString(json, "icon");
            if (!icon.empty())
            {
                out->setIcon(icon);
            }
            if (json.contains("checkable") && json.at("checkable").is_boolean())
            {
                out->setCheckable(json.at("checkable").get<bool>());
            }
            if (json.contains("checked") && json.at("checked").is_boolean())
            {
                out->setChecked(json.at("checked").get<bool>());
            }
            return out;
        }

        template<typename T, typename U, typename V>
        std::shared_ptr<IWidget> numberFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = T::create(context);
            loadRange<T, U, V>(json, out, result);
            if (json.contains("rangeSoft") && json.at("rangeSoft").is_boolean())
            {
                out->getModel()->setRangeSoft(json.at("rangeSoft").get<bool>());
            }
            if (json.contains("step") && json.at("step").is_number())
            {
                out->setStep(json.at("step").get<V>());
            }
            if (json.contains("largeStep") && json.at("largeStep").is_number())
            {
                out->setLargeStep(json.at("largeStep").get<V>());
            }
            if (json.contains("default") && json.at("default").is_number())
            {
                out->setDefault(json.at("default").get<V>());
            }
            if (json.contains("value") && json.at("value").is_number())
            {
                out->setValue(json.at("value").get<V>());
            }
            return out;
        }

        std::shared_ptr<IWidget> comboBoxFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = ComboBox::create(context);
            if (json.contains("items") && json.at("items").is_array())
            {
                std::vector<std::string> items;
                for (const auto& item : json.at("items"))
                {
                    if (item.is_string())
                    {
                        items.push_back(item.get<std::string>());
                    }
                }
                out->setItems(items);
            }
            if (json.contains("currentIndex") && json.at("currentIndex").is_number_integer())
            {
                out->setCurrentIndex(json.at("currentIndex").get<int>());
            }
            return out;
        }

        std::shared_ptr<IWidget> lineEditFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = LineEdit::create(context);
            const std::string text = getString(json, "text");
            if (!text.empty())
            {
                out->setText(text);
            }
            const std::string format = getString(json, "format");
            if (!format.empty())
            {
                out->setFormat(format);
            }
            return out;
        }

        std::shared_ptr<IWidget> groupBoxFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = GroupBox::create(context, getString(json, "text"));
            widgetLoadChildren(context, json, out, result);
            return out;
        }

        std::shared_ptr<IWidget> bellowsFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            auto out = Bellows::create(context, getString(json, "text"));
            if (json.contains("widget"))
            {
                if (auto widget = load(context, json.at("widget"), nullptr, result))
                {
                    out->setWidget(widget);
                }
            }
            if (json.contains("open") && json.at("open").is_boolean())
            {
                out->setOpen(json.at("open").get<bool>());
            }
            return out;
        }

        std::shared_ptr<IWidget> scrollWidgetFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            ScrollType scrollType = ScrollType::Both;
            getEnum(json, "scrollType", scrollType, result);
            auto out = ScrollWidget::create(context, scrollType);
            if (json.contains("border") && json.at("border").is_boolean())
            {
                out->setBorder(json.at("border").get<bool>());
            }
            if (json.contains("widget"))
            {
                if (auto widget = load(context, json.at("widget"), nullptr, result))
                {
                    out->setWidget(widget);
                }
            }
            return out;
        }

        std::shared_ptr<IWidget> dividerFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            Orientation orientation = Orientation::Horizontal;
            getEnum(json, "orientation", orientation, result);
            return Divider::create(context, orientation);
        }

        std::shared_ptr<IWidget> spacerFactory(
            const std::shared_ptr<Context>& context,
            const nlohmann::json& json,
            WidgetLoadResult& result)
        {
            Orientation orientation = Orientation::Horizontal;
            getEnum(json, "orientation", orientation, result);
            auto out = Spacer::create(context, orientation);
            SizeRole sizeRole = SizeRole::None;
            if (getEnum(json, "spacingRole", sizeRole, result))
            {
                out->setSpacingRole(sizeRole);
            }
            return out;
        }

        std::map<std::string, WidgetLoadFactory>& getRegistry()
        {
            static std::map<std::string, WidgetLoadFactory> registry;
            if (registry.empty())
            {
                registry["Bellows"] = &bellowsFactory;
                registry["CheckBox"] = &buttonFactory<CheckBox>;
                registry["ComboBox"] = &comboBoxFactory;
                registry["Divider"] = &dividerFactory;
                registry["DoubleEdit"] = &numberFactory<DoubleEdit, RangeD, double>;
                registry["DoubleEditSlider"] = &numberFactory<DoubleEditSlider, RangeD, double>;
                registry["FloatEdit"] = &numberFactory<FloatEdit, RangeF, float>;
                registry["FloatEditSlider"] = &numberFactory<FloatEditSlider, RangeF, float>;
                registry["FormLayout"] = &formLayoutFactory;
                registry["GroupBox"] = &groupBoxFactory;
                registry["HorizontalLayout"] = &rowLayoutFactory<HorizontalLayout>;
                registry["IntEdit"] = &numberFactory<IntEdit, RangeI, int>;
                registry["IntEditSlider"] = &numberFactory<IntEditSlider, RangeI, int>;
                registry["Label"] = &textFactory<Label>;
                registry["LineEdit"] = &lineEditFactory;
                registry["PushButton"] = &buttonFactory<PushButton>;
                registry["ScrollWidget"] = &scrollWidgetFactory;
                registry["Spacer"] = &spacerFactory;
                registry["ToolButton"] = &buttonFactory<ToolButton>;
                registry["VerticalLayout"] = &rowLayoutFactory<VerticalLayout>;
            }
            return registry;
        }
    }

    WidgetLoadResult widgetLoad(
        const std::shared_ptr<Context>& context,
        const nlohmann::json& json,
        const std::shared_ptr<IWidget>& parent)
    {
        WidgetLoadResult out;
        out.widget = load(context, json, parent, out);
        return out;
    }

    std::shared_ptr<IWidget> findWidget(
        const std::shared_ptr<IWidget>& widget,
        const std::string& id)
    {
        std::shared_ptr<IWidget> out;
        if (widget)
        {
            if (widget->hasProperty(idKey) && widget->getProperty(idKey) == id)
            {
                out = widget;
            }
            else
            {
                for (const auto& child : widget->getChildren())
                {
                    out = findWidget(child, id);
                    if (out)
                        break;
                }
            }
        }
        return out;
    }

    void widgetLoadRegister(
        const std::string& type,
        const WidgetLoadFactory& factory)
    {
        getRegistry()[type] = factory;
    }

    void widgetLoadChildren(
        const std::shared_ptr<Context>& context,
        const nlohmann::json& json,
        const std::shared_ptr<IWidget>& parent,
        WidgetLoadResult& result)
    {
        if (json.contains("children") && json.at("children").is_array())
        {
            for (const auto& child : json.at("children"))
            {
                load(context, child, parent, result);
            }
        }
    }
}
