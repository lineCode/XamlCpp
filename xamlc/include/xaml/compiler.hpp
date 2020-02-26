#ifndef XAMLC_COMPILER_HPP
#define XAMLC_COMPILER_HPP

#include <iosfwd>
#include <vector>
#include <xaml/meta/meta.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/xaml_node.hpp>

namespace xaml
{
    class compiler_module : public module
    {
    public:
        compiler_module() : module() {}
        compiler_module(std::string_view path) : module(path) {}
        ~compiler_module() override {}

        XAMLC_API bool can_compile(std::type_index type) noexcept;
        XAMLC_API std::string compile(std::type_index type, std::string_view code) noexcept;
        XAMLC_API std::vector<std::string_view> include_headers() noexcept;
    };

    XAMLC_API void add_compiler_module(meta_context& ctx, std::string_view path);

    class compiler
    {
    private:
        meta_context* m_ctx;
        size_t indent_count{ 0 };

    public:
        compiler(meta_context& ctx) : m_ctx(&ctx) {}
        ~compiler() {}

    protected:
        XAMLC_API std::ostream& write_indent(std::ostream& stream);

        XAMLC_API std::ostream& write_include(std::ostream& stream, std::string_view header);
        XAMLC_API std::ostream& write_includes(std::ostream& stream, std::vector<std::string_view> const& headers);

        XAMLC_API std::ostream& write_begin_block(std::ostream& stream);
        XAMLC_API std::ostream& write_end_block(std::ostream& stream);

        XAMLC_API std::ostream& write_init_decl(std::ostream& stream, std::string_view ns, std::string_view name);

        XAMLC_API std::ostream& write_type(std::ostream& stream, std::type_index type);
        XAMLC_API std::ostream& write_type(std::ostream& stream, std::string_view ns, std::string_view name);
        XAMLC_API std::ostream& write_args(std::ostream& stream, std::initializer_list<std::string_view> args);
        XAMLC_API std::ostream& write_construct(std::ostream& stream, std::string_view name, std::type_index type);
        XAMLC_API std::ostream& write_call(std::ostream& stream, std::string_view name, std::string_view prefix, std::string_view method, std::initializer_list<std::string_view> args);
        XAMLC_API std::ostream& write_static_call(std::ostream& stream, std::type_index type, std::string_view prefix, std::string_view method, std::initializer_list<std::string_view> args);

        XAMLC_API std::ostream& write_set_property(std::ostream& stream, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_set_property(std::ostream& stream, std::type_index type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_set_property(std::ostream& stream, std::type_index node_type, std::type_index host_type, std::type_index prop_type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, std::type_index type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, std::type_index node_type, std::type_index host_type, std::type_index prop_type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_event(std::ostream& stream, xaml_node& this_node, std::string_view name, xaml_event& ev);

        XAMLC_API std::ostream& write_markup(std::ostream& stream, std::string_view name, std::string_view prop, std::shared_ptr<meta_class> markup);

        XAMLC_API std::ostream& write_deserialize(std::ostream& stream, std::string_view path);

        XAMLC_API std::ostream& compile_impl(std::ostream& stream, xaml_node& node, xaml_node& this_node, bool is_this);
        XAMLC_API std::ostream& compile_extensions(std::ostream& stream, xaml_node& node, bool is_this);

    public:
        XAMLC_API std::ostream& compile(std::ostream& stream, xaml_node& node);
        XAMLC_API std::ostream& compile_fake(std::ostream& stream, xaml_node& node, std::string_view path);
    };
} // namespace xaml

#endif // !XAMLC_COMPILER_HPP
