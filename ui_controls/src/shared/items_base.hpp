#ifndef XAML_UI_CONTROLS_SHARED_ITEMS_BASE_HPP
#define XAML_UI_CONTROLS_SHARED_ITEMS_BASE_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/items_base.h>

struct xaml_items_base_internal : xaml_control_internal
{
    XAML_EVENT_IMPL(items_changed)
    XAML_PROP_PTR_IMPL_BASE(items, xaml_observable_vector)

    virtual xaml_result XAML_CALL insert_item(std::int32_t index, xaml_ptr<xaml_object> const& value) noexcept = 0;
    virtual xaml_result XAML_CALL remove_item(std::int32_t index) noexcept = 0;
    virtual xaml_result XAML_CALL clear_items() noexcept = 0;
    virtual xaml_result XAML_CALL replace_item(std::int32_t index, xaml_ptr<xaml_object> const& value) = 0;

    std::int32_t m_items_changed_token{ 0 };

    virtual xaml_result XAML_CALL on_items_vector_changed(xaml_ptr<xaml_observable_vector>, xaml_ptr<xaml_vector_changed_args> args) noexcept;

    xaml_result XAML_CALL set_items(xaml_observable_vector* value) noexcept;

    XAML_EVENT_IMPL(sel_id_changed)
    XAML_PROP_EVENT_IMPL(sel_id, std::int32_t, std::int32_t*, std::int32_t)

    xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename... Base>
struct xaml_items_base_implement : xaml_control_implement<T, Base..., xaml_items_base>
{
    XAML_EVENT_INTERNAL_IMPL(items_changed)
    XAML_PROP_PTR_INTERNAL_IMPL(items, xaml_observable_vector)

    XAML_EVENT_INTERNAL_IMPL(sel_id_changed)
    XAML_PROP_INTERNAL_IMPL(sel_id, std::int32_t*, std::int32_t)
};

#endif // !XAML_UI_CONTROLS_SHARED_ITEMS_BASE_HPP
