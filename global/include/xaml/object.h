#ifndef XAML_OBJECT_HPP
#define XAML_OBJECT_HPP

#ifdef __cplusplus
#include <atomic>
#include <cstddef>
#else
#include <stddef.h>
#endif // __cplusplus

#include <xaml/guid.h>
#include <xaml/result.h>
#include <xaml/utility.h>

XAML_CLASS(xaml_object, { 0xaf86e2e0, 0xb12d, 0x4c6a, { 0x9c, 0x5a, 0xd7, 0xaa, 0x65, 0x10, 0x1e, 0x90 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_object
{
    virtual std::size_t XAML_CALL add_ref() noexcept = 0;
    virtual std::size_t XAML_CALL release() noexcept = 0;
    virtual xaml_result XAML_CALL get_type(xaml_guid*) noexcept = 0;
    virtual xaml_result XAML_CALL query(xaml_guid const&, xaml_object**) noexcept = 0;

    template <typename T>
    xaml_result query(T** ptr) noexcept
    {
        return query(xaml_type_guid_v<T>, (xaml_object**)ptr);
    }
};
#else
#define XAML_OBJECT_VTBL(type)                                 \
    size_t(XAML_CALL* add_ref)(type* const);                   \
    size_t(XAML_CALL* release)(type* const);                   \
    xaml_result(XAML_CALL* get_type)(type* const, xaml_guid*); \
    xaml_result(XAML_CALL* query)(type* const, xaml_guid const*, xaml_object**);

struct xaml_object
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_object)
    } const* vtbl;
};
#endif // __cplusplus

#ifdef __cplusplus
template <typename T, typename D, typename... Base>
struct xaml_implement : D
{
protected:
    std::atomic<std::size_t> m_ref_count{ 1 };

public:
    std::size_t XAML_CALL add_ref() noexcept override { return ++m_ref_count; }

    std::size_t XAML_CALL release() noexcept override
    {
        std::size_t res = --m_ref_count;
        if (!res)
        {
            delete static_cast<T*>(this);
            return 0;
        }
        else
        {
            return res;
        }
    }

    xaml_result XAML_CALL get_type(xaml_guid* ptype) noexcept override
    {
        *ptype = xaml_type_guid_v<D>;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL query(xaml_guid const& type, xaml_object** ptr) noexcept override;
};

template <typename... B>
struct __query_impl;

template <typename B1, typename... B>
struct __query_impl<B1, B...>
{
    template <typename T, typename D, typename... Base>
    xaml_result operator()(xaml_implement<T, D, Base...>* self, xaml_guid const& type, xaml_object** ptr) const noexcept
    {
        if (type == xaml_type_guid_v<B1>)
        {
            self->add_ref();
            *ptr = static_cast<B1*>(self);
            return 0;
        }
        else
        {
            return __query_impl<B...>{}(self, type, ptr);
        }
    }
};

template <>
struct __query_impl<>
{
    template <typename T, typename D, typename... Base>
    xaml_result operator()(xaml_implement<T, D, Base...>*, xaml_guid const&, xaml_object**) const noexcept
    {
        return 1;
    }
};

template <typename T, typename D, typename... Base>
inline xaml_result xaml_implement<T, D, Base...>::query(xaml_guid const& type, xaml_object** ptr) noexcept
{
    return __query_impl<D, Base...>{}(this, type, ptr);
}

template <typename D, typename T, typename... Args>
inline xaml_result xaml_object_new(T** ptr, Args&&... args) noexcept
{
    try
    {
        T* res = new D(std::forward<Args>(args)...);
        *ptr = res;
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()
}
#endif // __cplusplus

#endif // !XAML_OBJECT_HPP
