#include <wil/resource.h>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/label.hpp>

using namespace std;

namespace xaml
{
    static wil::unique_hbrush white_brush{ CreateSolidBrush(RGB(255, 255, 255)) };

    optional<LRESULT> label::__wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        case WM_CTLCOLORSTATIC:
        {
            return (LRESULT)white_brush.get();
        }
        }
        return nullopt;
    }

    void label::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            window_create_params params = {};
            params.class_name = U("STATIC");
            params.style = WS_CHILD | WS_VISIBLE | SS_LEFT;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = get_parent().get();
            this->__create(params);
        }
        rectangle real = region - get_margin();
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, (int)real.x, (int)real.y, (int)real.width, (int)real.height, SWP_NOZORDER));
        __set_size_noevent({ real.width, real.height });
        draw_text();
        draw_alignment();
        SetParent(get_handle(), get_parent()->get_handle());
        ShowWindow(get_handle(), SW_SHOW);
    }

    void label::draw_size()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, 0, 0, (int)get_width(), (int)get_height(), SWP_NOZORDER | SWP_NOMOVE));
    }

    void label::draw_text()
    {
        THROW_IF_WIN32_BOOL_FALSE(Static_SetText(get_handle(), m_text.c_str()));
    }

    void label::draw_alignment()
    {
        LONG_PTR style = WS_CHILD | WS_VISIBLE;
        switch (m_text_halignment)
        {
        case halignment_t::center:
            style |= SS_CENTER;
            break;
        case halignment_t::right:
            style |= SS_RIGHT;
            break;
        default:
            style |= SS_LEFT;
            break;
        }
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
    }

    void label::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(m_text));
        draw_size();
    }
} // namespace xaml
