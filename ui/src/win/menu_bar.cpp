#include <shared/menu_bar.hpp>
#include <wil/result_macros.h>
#include <xaml/result_win32.h>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/window.h>

using namespace std;

xaml_result xaml_menu_bar_internal::draw(xaml_rectangle const& region) noexcept
{
    xaml_control* parent = m_parent;
    xaml_ptr<xaml_win32_control> native_parent;
    if (XAML_SUCCEEDED(parent->query(&native_parent)))
    {
        XAML_RETURN_IF_FAILED(native_parent->get_handle(&m_handle));
    }
    if (!m_menu)
    {
        m_menu.reset(CreateMenu());
        XAML_RETURN_IF_FAILED(draw_visible());
    }
    XAML_RETURN_IF_FAILED(draw_submenu());
    if (m_handle) XAML_RETURN_IF_WIN32_BOOL_FALSE(DrawMenuBar(m_handle));
    return XAML_S_OK;
}

xaml_result xaml_menu_bar_internal::draw_submenu() noexcept
{
    XAML_FOREACH_START(child, m_children);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(child->query(&cc));
        XAML_RETURN_IF_FAILED(cc->draw({}));
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_menu_bar_internal::draw_visible() noexcept
{
    if (m_is_visible)
        XAML_RETURN_IF_WIN32_BOOL_FALSE(SetMenu(m_handle, m_menu.get()));
    else
    {
        if (GetMenu(m_handle) == m_menu.get())
            XAML_RETURN_IF_WIN32_BOOL_FALSE(SetMenu(m_handle, nullptr));
    }
    return XAML_S_OK;
}
