#include <iomanip>
#include <iostream>
#include <options.h>
#include <sf/format.hpp>
#include <unordered_map>
#include <xaml/version.h>

#ifdef UNICODE
#define _tmain wmain
#define _tcout ::std::wcout
#else
#define _tmain main
#define _tcout ::std::cout
#endif // UNICODE

using namespace std;

xaml_std_string_view_t get_type_name(xaml_ptr<xaml_meta_context> const& ctx, xaml_guid const& type)
{
    xaml_ptr<xaml_reflection_info> info;
    if (XAML_SUCCEEDED(ctx->get_type(type, &info)))
    {
        xaml_ptr<xaml_string> name;
        XAML_THROW_IF_FAILED(info->get_name(&name));
        return to_string_view_t(name);
    }
    else
    {
        return U("Unknown-type");
    }
}

int _tmain(int argc, xaml_char_t** argv)
{
    xaml_ptr<xaml_meta_context> cmdline_ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&cmdline_ctx));
    XAML_THROW_IF_FAILED(xaml_detector_options_register(cmdline_ctx));
    xaml_ptr<xaml_detector_options> options;
    XAML_THROW_IF_FAILED(xaml_cmdline_parse_and_print(cmdline_ctx, argc, argv, &options));

    xaml_ptr<xaml_string> path_str;
    XAML_THROW_IF_FAILED(options->get_path(&path_str));

    xaml_ptr<xaml_module> module;
    XAML_THROW_IF_FAILED(xaml_module_new(&module));
    XAML_THROW_IF_FAILED(module->open(path_str));

    xaml_ptr<xaml_meta_context> ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&ctx));

    bool recursive;
    XAML_THROW_IF_FAILED(options->get_recursive(&recursive));
    if (recursive)
        XAML_THROW_IF_FAILED(ctx->add_module_recursive(module));
    else
        XAML_THROW_IF_FAILED(ctx->add_module(module));

    xaml_result (*pget_version)(xaml_version*);
    XAML_THROW_IF_FAILED(module->get_method("xaml_module_version", (void**)&pget_version));

    xaml_version ver;
    XAML_THROW_IF_FAILED(pget_version(&ver));

    sf::println(_tcout, U("Module {} ({})"), quoted(to_string_view_t(path_str)), ver);

    xaml_ptr<xaml_map_view> types;
    XAML_THROW_IF_FAILED(ctx->get_types(&types));

    for (auto item : types)
    {
        xaml_ptr<xaml_key_value_pair> pair = item.query<xaml_key_value_pair>();
        xaml_ptr<xaml_object> second;
        XAML_THROW_IF_FAILED(pair->get_value(&second));
        xaml_ptr<xaml_reflection_info> info = second.query<xaml_reflection_info>();
        xaml_ptr<xaml_string> name;
        XAML_THROW_IF_FAILED(info->get_name(&name));
        xaml_ptr<xaml_string> include_file;
        XAML_THROW_IF_FAILED(info->get_include_file(&include_file));
        if (auto t = info.query<xaml_type_info>())
        {
            sf::println(_tcout, U("class {}"), name);
            sf::println(_tcout, U("  included in <{}>"), include_file);
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_properties(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair->get_key(&key));
                    xaml_ptr<xaml_object> value;
                    XAML_THROW_IF_FAILED(pair->get_value(&value));
                    xaml_ptr<xaml_property_info> info = value.query<xaml_property_info>();
                    xaml_guid type;
                    XAML_THROW_IF_FAILED(info->get_type(&type));
                    sf::println(_tcout, U("    P: {}\t{}"), key.query<xaml_string>(), get_type_name(ctx, type));
                }
            }
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_collection_properties(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair->get_key(&key));
                    xaml_ptr<xaml_object> value;
                    XAML_THROW_IF_FAILED(pair->get_value(&value));
                    xaml_ptr<xaml_collection_property_info> info = value.query<xaml_collection_property_info>();
                    xaml_guid type;
                    XAML_THROW_IF_FAILED(info->get_type(&type));
                    sf::println(_tcout, U("    C: {}\t{}"), key.query<xaml_string>(), get_type_name(ctx, type));
                }
            }
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_events(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair->get_key(&key));
                    sf::println(_tcout, U("    E: {}"), key.query<xaml_string>());
                }
            }
        }
        else if (auto t = info.query<xaml_enum_info>())
        {
            sf::println(_tcout, U("enum {}"), name);
            sf::println(_tcout, U("  included in <{}>"), include_file);
            xaml_ptr<xaml_map_view> values;
            XAML_THROW_IF_FAILED(t->get_values(&values));
            for (auto item2 : values)
            {
                xaml_ptr<xaml_key_value_pair> pair = item2.query<xaml_key_value_pair>();
                xaml_ptr<xaml_object> key;
                XAML_THROW_IF_FAILED(pair->get_key(&key));
                xaml_ptr<xaml_object> value;
                XAML_THROW_IF_FAILED(pair->get_value(&value));
                xaml_ptr<xaml_box> box = value.query<xaml_box>();
                int const* pvalue;
                XAML_THROW_IF_FAILED(box->get_data((void const**)&pvalue));
                sf::println(_tcout, U("    {} = {}"), key.query<xaml_string>(), *pvalue);
            }
        }
    }

    return 0;
}
