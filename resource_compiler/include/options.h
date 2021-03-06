#ifndef XAML_RC_OPTIONS_H
#define XAML_RC_OPTIONS_H

#include <xaml/cmdline/options_base.h>
#include <xaml/meta/meta_context.h>
#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_rc_options, { 0x50dd3e88, 0x5d41, 0x4418, { 0x90, 0x96, 0x7f, 0xd8, 0xb7, 0x76, 0xf9, 0xca } })

#define XAML_RC_OPTIONS_VTBL(type)                           \
    XAML_VTBL_INHERIT(XAML_CMDLINE_OPTIONS_BASE_VTBL(type)); \
    XAML_METHOD(get_inputs, type, xaml_vector_view**);       \
    XAML_CPROP(input, type, xaml_string*, xaml_string*);     \
    XAML_PROP(output, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_rc_options, xaml_cmdline_options_base)
{
    XAML_DECL_VTBL(xaml_rc_options, XAML_RC_OPTIONS_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_rc_options_new(xaml_rc_options**) XAML_NOEXCEPT;
EXTERN_C xaml_result XAML_CALL xaml_rc_options_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_RC_OPTIONS_H
