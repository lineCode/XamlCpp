#include <win/cotaskmem_string.hpp>

using namespace std;

struct xaml_string_cotaskmem_impl : xaml_implement<xaml_string_cotaskmem_impl, xaml_string, xaml_object>
{
    wil::unique_cotaskmem_string m_str;

    xaml_string_cotaskmem_impl(wil::unique_cotaskmem_string&& str) noexcept : m_str(std::move(str)) {}

    xaml_result XAML_CALL get_length(int32_t* psize) noexcept override
    {
        *psize = lstrlen(m_str.get());
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_data(xaml_char_t const** ptr) noexcept override
    {
        if (!m_str.get()[0])
            *ptr = nullptr;
        else
            *ptr = m_str.get();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL equals(xaml_string* str, bool* pres) noexcept override
    {
        int32_t rhs_length;
        XAML_RETURN_IF_FAILED(str->get_length(&rhs_length));
        xaml_char_t const* rhs_data;
        XAML_RETURN_IF_FAILED(str->get_data(&rhs_data));
        int32_t lhs_length = lstrlen(m_str.get());
        if (lhs_length != rhs_length)
        {
            *pres = false;
        }
        else
        {
            *pres = char_traits<xaml_char_t>::compare(m_str.get(), rhs_data, (size_t)rhs_length) == 0;
        }
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_string_new_cotaskmem(wil::unique_cotaskmem_string&& str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_cotaskmem_impl>(ptr, std::move(str));
}