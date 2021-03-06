#ifndef XAMLD_OPTIONS_H
#define XAMLD_OPTIONS_H

#include <xaml/cmdline/options_base.h>
#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_detector_options, { 0xd1e430b3, 0xb91f, 0x4cba, { 0x87, 0x81, 0xda, 0x1d, 0x48, 0x59, 0xef, 0x7e } })

#define XAML_DETECTOR_OPTIONS_VTBL(type)                     \
    XAML_VTBL_INHERIT(XAML_CMDLINE_OPTIONS_BASE_VTBL(type)); \
    XAML_PROP(recursive, type, bool*, bool);                 \
    XAML_PROP(path, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_detector_options, xaml_cmdline_options_base)
{
    XAML_DECL_VTBL(xaml_detector_options, XAML_DETECTOR_OPTIONS_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_detector_options_new(xaml_detector_options**) XAML_NOEXCEPT;
EXTERN_C xaml_result XAML_CALL xaml_detector_options_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAMLD_OPTIONS_H
