#include <shared/entry.hpp>
#include <xaml/ui/controls/entry.h>

using namespace std;

xaml_result XAML_CALL xaml_entry_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_text_changed->add_noexcept<xaml_ptr<xaml_entry>, xaml_ptr<xaml_string>>(
        [this](xaml_ptr<xaml_entry>, xaml_ptr<xaml_string>) -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(draw_text());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_entry_new(xaml_entry** ptr) noexcept
{
    return xaml_object_init<xaml_entry_impl>(ptr);
}

xaml_result XAML_CALL xaml_entry_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_entry;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_entry_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text, xaml_string);
    XAML_TYPE_INFO_ADD_PROP(text_halignment, xaml_halignment);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_entry_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_entry, "xaml/ui/controls/entry.h");
    XAML_RETURN_IF_FAILED(xaml_entry_members(__info));
    return ctx->add_type(__info);
}
