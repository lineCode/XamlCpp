#ifndef XAML_UI_META_HPP
#define XAML_UI_META_HPP

#include <xaml/ui/application.hpp>
#include <xaml/ui/button.hpp>
#include <xaml/ui/canvas.hpp>
#include <xaml/ui/entry.hpp>
#include <xaml/ui/grid.hpp>
#include <xaml/ui/label.hpp>
#include <xaml/ui/password_entry.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    inline void init_meta(std::shared_ptr<meta_context> const& ctx = nullptr) noexcept
    {
        init_context(ctx);
        add_xml_namespace("https://github.com/Berrysoft/XamlCpp/", "xaml");
        register_class<
            application,
            window,
            grid,
            button,
            canvas,
            label,
            entry,
            password_entry>();
        REGISTER_ENUM(xaml, halignment_t);
        REGISTER_ENUM(xaml, valignment_t);
        REGISTER_ENUM(xaml, grid_layout);
    }
} // namespace xaml

#endif // !XAML_UI_META_HPP
