#ifndef XAML_DELEGATE_H
#define XAML_DELEGATE_H

#ifdef __cplusplus
#include <functional>
#include <xaml/box.h>
#include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/vector.h>

XAML_CLASS(xaml_delegate, { 0x3a1a793a, 0x8a83, 0x4d40, { 0x89, 0x62, 0x03, 0x9e, 0x00, 0x0c, 0x0f, 0xc3 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_delegate : xaml_object
{
    virtual xaml_result XAML_CALL invoke(xaml_vector_view*, xaml_object**) noexcept = 0;
};
#else
#define XAML_DELEGATE_VTBL(type) \
    XAML_OBJECT_VTBL(type)       \
    xaml_result(XAML_CALL* invoke)(type* const, xaml_vector_view*, xaml_object**);

struct xaml_delegate
{
    struct
    {
        XAML_DELEGATE_VTBL(xaml_delegate)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_delegate_new(xaml_result (*)(xaml_vector_view*, xaml_object**), xaml_delegate**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result xaml_delegate_bind(xaml_delegate*, xaml_vector_view*, xaml_delegate**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result xaml_delegate_new(std::function<xaml_result(xaml_vector_view*, xaml_object**)>&&, xaml_delegate**) noexcept;

inline xaml_ptr<xaml_object> __xaml_delegate_impl_invoke_impl_get_at(xaml_vector_view* args, size_t index)
{
    xaml_ptr<xaml_object> item;
    XAML_THROW_IF_FAILED(args->get_at(index, &item));
    return item;
}

template <typename Return, typename... Args, size_t... Indicies>
Return __xaml_delegate_impl_invoke_impl(std::function<Return(Args...)> const& func, xaml_vector_view* args, std::index_sequence<Indicies...>)
{
    return func(unbox_value<Args>(__xaml_delegate_impl_invoke_impl_get_at(args, Indicies))...);
}

template <typename Return, typename... Args>
Return __xaml_delegate_impl_invoke(std::function<Return(Args...)> const& func, xaml_vector_view* args)
{
    return __xaml_delegate_impl_invoke_impl<Return, Args...>(func, args, std::make_index_sequence<sizeof...(Args)>{});
}

template <typename Return, typename... Args, typename F>
inline xaml_result xaml_delegate_new(F&& func, xaml_delegate** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_delegate_new(
        std::function<xaml_result(xaml_vector_view*, xaml_object**)>{
            [func = std::function<Return(Args...)>(std::forward<F>(func))](xaml_vector_view* args, xaml_object** ptr) -> xaml_result {
                std::size_t size;
                XAML_RETURN_IF_FAILED(args->get_size(&size));
                if (size < sizeof...(Args)) return XAML_E_INVALIDARG;
                try
                {
                    if constexpr (std::is_same_v<Return, void>)
                    {
                        __xaml_delegate_impl_invoke<Return, Args...>(func, args);
                        *ptr = nullptr;
                        return XAML_S_OK;
                    }
                    else
                    {
                        Return res = __xaml_delegate_impl_invoke<Return, Args...>(func, args);
                        return box_value(res, ptr);
                    }
                }
                XAML_CATCH_RETURN() } },
        ptr);
}
#endif // __cplusplus

#endif // !XAML_DELEGATE_H
