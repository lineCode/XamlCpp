#include <shared/radio_box.hpp>
#include <xaml/ui/container.h>
#include <xaml/ui/controls/radio_box.h>

using namespace std;

xaml_result xaml_radio_box_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_button_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_checked_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_is_checked_changed->add_noexcept<xaml_ptr<xaml_radio_box>, bool>(
        [this](xaml_ptr<xaml_radio_box>, bool) -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(draw_checked());
                XAML_RETURN_IF_FAILED(draw_group());
            }
            return XAML_S_OK;
        },
        &token)));

#ifdef XAML_UI_COCOA
    XAML_RETURN_IF_FAILED((m_click->add_noexcept<xaml_ptr<xaml_radio_box>>(
        [this](xaml_ptr<xaml_radio_box>) -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(on_state_changed());
            }
            return XAML_S_OK;
        },
        &token)));
#endif // XAML_UI_COCOA
    return XAML_S_OK;
}

#ifndef XAML_UI_GTK3
xaml_result xaml_radio_box_internal::draw_group() noexcept
{
    if (m_parent && m_is_checked)
    {
        xaml_ptr<xaml_multicontainer> multic;
        if (XAML_SUCCEEDED(m_parent->query(&multic)))
        {
            xaml_ptr<xaml_vector_view> children;
            XAML_RETURN_IF_FAILED(multic->get_children(&children));
            XAML_FOREACH_START(c, children);
            {
                if (auto rc = c.query<xaml_radio_box>())
                {
                    if (rc.get() != static_cast<xaml_radio_box*>(m_outer_this))
                    {
                        xaml_ptr<xaml_string> group;
                        XAML_RETURN_IF_FAILED(rc->get_group(&group));
                        bool equals;
                        XAML_RETURN_IF_FAILED(xaml_string_equals(group, m_group, &equals));
                        if (equals)
                        {
                            XAML_RETURN_IF_FAILED(rc->set_is_checked(false));
                        }
                    }
                }
            }
            XAML_FOREACH_END();
        }
    }
    return XAML_S_OK;
}
#endif // !XAML_UI_GTK3

xaml_result XAML_CALL xaml_radio_box_new(xaml_radio_box** ptr) noexcept
{
    return xaml_object_init<xaml_radio_box_impl>(ptr);
}

xaml_result XAML_CALL xaml_radio_box_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_radio_box;
    XAML_RETURN_IF_FAILED(xaml_button_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_radio_box_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_checked, bool);
    XAML_TYPE_INFO_ADD_PROP(group, xaml_string);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_radio_box_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_radio_box, "xaml/ui/controls/check_box.h");
    XAML_RETURN_IF_FAILED(xaml_radio_box_members(__info));
    return ctx->add_type(__info);
}
