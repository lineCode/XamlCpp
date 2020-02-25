#include <xaml/ui/application.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/meta.hpp>
#include <xaml/ui/window.hpp>

using namespace std;
using namespace xaml;

extern "C"
{
    XAML_UI_META_API void init_meta(void* ctx) noexcept
    {
        shared_ptr<meta_context>* pctx = (shared_ptr<meta_context>*)ctx;
        ui_init_traits::init_meta(pctx ? *pctx : nullptr);
    }

    const char* const s_headers[] = {
        "xaml/ui/application.hpp",
        "xaml/ui/container.hpp",
        "xaml/ui/window.hpp",
        nullptr
    };
    XAML_UI_META_API const char* const* include_headers() noexcept { return s_headers; }
}

namespace xaml
{
    void ui_init_traits::init_meta(shared_ptr<meta_context> const& ctx) noexcept
    {
        init_context(ctx);
        add_xml_namespace("https://github.com/Berrysoft/XamlCpp/", "xaml");
        register_class<
            application,
            control,
            container,
            multicontainer,
            window>();
        REGISTER_ENUM(xaml, halignment_t);
        REGISTER_ENUM(xaml, valignment_t);
    }
} // namespace xaml
