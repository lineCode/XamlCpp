#include <xaml/ui/controls/webview.hpp>

namespace xaml
{
    webview::webview() : control()
    {
        add_uri_changed([this](webview&, string_view_t) { if (get_handle() && !m_navigating) draw_uri(); });
    }

    webview::~webview() {}
} // namespace xaml
