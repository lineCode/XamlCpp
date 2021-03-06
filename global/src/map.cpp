#include <xaml/map.h>

using namespace std;

using inner_map_type = unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>, __std_xaml_hasher, __std_xaml_eq>;

struct xaml_key_value_pair_impl : xaml_implement<xaml_key_value_pair_impl, xaml_key_value_pair, xaml_object>
{
private:
    xaml_ptr<xaml_object> m_key;
    xaml_ptr<xaml_object> m_value;

public:
    xaml_key_value_pair_impl(xaml_ptr<xaml_object>&& key, xaml_ptr<xaml_object>&& value) noexcept : m_key(move(key)), m_value(move(value)) {}

    xaml_result XAML_CALL get_key(xaml_object** ptr) noexcept
    {
        return m_key->query(ptr);
    }

    xaml_result XAML_CALL get_value(xaml_object** ptr) noexcept
    {
        return m_value->query(ptr);
    }
};

xaml_result XAML_CALL xaml_key_value_pair_new(xaml_object* key, xaml_object* value, xaml_key_value_pair** ptr) noexcept
{
    return xaml_object_new<xaml_key_value_pair_impl>(ptr, key, value);
}

struct xaml_map_enumerator_impl : xaml_implement<xaml_map_enumerator_impl, xaml_enumerator, xaml_object>
{
private:
    inner_map_type::const_iterator m_begin, m_end;
    bool m_init;

public:
    xaml_map_enumerator_impl(inner_map_type::const_iterator begin, inner_map_type::const_iterator end) noexcept
        : m_begin(begin), m_end(end), m_init(false) {}

    xaml_result XAML_CALL move_next(bool* pb) noexcept override
    {
        if (!m_init)
        {
            m_init = true;
        }
        else
        {
            ++m_begin;
        }
        *pb = m_begin != m_end;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_current(xaml_object** ptr) noexcept override
    {
        if (m_begin == m_end) return XAML_E_OUTOFBOUNDS;
        xaml_ptr<xaml_key_value_pair> pair;
        XAML_RETURN_IF_FAILED(xaml_key_value_pair_new(m_begin->first, m_begin->second, &pair));
        return pair->query(ptr);
    }
};

struct xaml_hasher_impl : xaml_implement<xaml_hasher_impl, xaml_hasher, xaml_object>
{
private:
    function<xaml_result(xaml_object*, int32_t*)> m_func;
    function<xaml_result(xaml_object*, xaml_object*, bool*)> m_eq_func;

public:
    xaml_hasher_impl(function<xaml_result(xaml_object*, int32_t*)>&& func, function<xaml_result(xaml_object*, xaml_object*, bool*)>&& eq_func) noexcept
        : m_func(move(func)), m_eq_func(move(eq_func)) {}

    xaml_result XAML_CALL hash(xaml_object* obj, int32_t* phash) noexcept override
    {
        return m_func(obj, phash);
    }

    xaml_result XAML_CALL equal(xaml_object* lhs, xaml_object* rhs, bool* pb) noexcept override
    {
        return m_eq_func(lhs, rhs, pb);
    }
};

xaml_result XAML_CALL xaml_hasher_new(xaml_result (*func)(xaml_object*, int32_t*), xaml_result (*eq_func)(xaml_object*, xaml_object*, bool*), xaml_hasher** ptr) noexcept
{
    return xaml_object_new_catch<xaml_hasher_impl>(ptr, func, eq_func);
}

xaml_result XAML_CALL xaml_hasher_new(function<xaml_result(xaml_object*, int32_t*)>&& func, function<xaml_result(xaml_object*, xaml_object*, bool*)>&& eq_func, xaml_hasher** ptr) noexcept
{
    return xaml_object_new<xaml_hasher_impl>(ptr, move(func), move(eq_func));
}

xaml_result XAML_CALL xaml_hasher_default(xaml_hasher** ptr) noexcept
{
    return xaml_hasher_new<xaml_ptr<xaml_object>>(ptr);
}

xaml_result XAML_CALL xaml_hasher_string_default(xaml_hasher** ptr) noexcept
{
    return xaml_hasher_new(
        function<xaml_result(xaml_object*, std::int32_t*)>{
            [](xaml_object* obj, int32_t* phash) -> xaml_result {
                static hash<xaml_std_string_view_t> hasher{};
                xaml_ptr<xaml_string> value;
                XAML_RETURN_IF_FAILED(xaml_unbox_value(obj, &value));
                xaml_char_t const* data;
                XAML_RETURN_IF_FAILED(value->get_data(&data));
                int32_t length;
                XAML_RETURN_IF_FAILED(value->get_length(&length));
                std::size_t std_hash = hasher(xaml_std_string_view_t(data, (std::size_t)length));
                std::int32_t* ptr = (std::int32_t*)&std_hash;
#if SIZE_MAX == UINT64_MAX
                static_assert(sizeof(XAML_STD size_t) == sizeof(XAML_STD uint64_t), "Unknown 64-bit platform.");
                *phash = ptr[0] ^ ptr[1];
#elif SIZE_MAX == UINT32_MAX
                static_assert(sizeof(XAML_STD size_t) == sizeof(XAML_STD uint32_t), "Unknown 32-bit platform.");
                *phash = ptr[0];
#else
#error Cannot determine platform architecture
#endif
                return XAML_S_OK;
            } },
        function<xaml_result(xaml_object*, xaml_object*, bool*)>{
            [](xaml_object* lhs, xaml_object* rhs, bool* pb) -> xaml_result {
                xaml_ptr<xaml_string> lvalue, rvalue;
                XAML_RETURN_IF_FAILED(xaml_unbox_value(lhs, &lvalue));
                XAML_RETURN_IF_FAILED(xaml_unbox_value(rhs, &rvalue));
                return xaml_string_equals(lvalue, rvalue, pb);
            } },
        ptr);
}

struct xaml_map_impl : xaml_implement<xaml_map_impl, xaml_map, xaml_map_view, xaml_enumerable, xaml_object>
{
private:
    inner_map_type m_map{};

public:
    xaml_map_impl(xaml_ptr<xaml_hasher> const& hasher) noexcept : m_map(1, __std_xaml_hasher{ hasher }, __std_xaml_eq{ hasher }) {}
    xaml_map_impl(inner_map_type&& map) noexcept : m_map(move(map)) {}

    xaml_result XAML_CALL get_size(int32_t* psize) noexcept override
    {
        *psize = (int32_t)m_map.size();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL lookup(xaml_object* key, xaml_object** ptr) noexcept override
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) return XAML_E_KEYNOTFOUND;
        return it->second->query(ptr);
    }

    xaml_result XAML_CALL has_key(xaml_object* key, bool* pb) noexcept override
    {
#ifdef XAML_APPLE
        auto it = m_map.find(key);
        *pb = it != m_map.end();
#else
        *pb = m_map.contains(key);
#endif // XAML_APPLE
        return XAML_S_OK;
    }

    xaml_result XAML_CALL insert(xaml_object* key, xaml_object* value, bool* pb) noexcept override
    {
        auto [it, inserted] = m_map.emplace(key, value);
        if (!inserted)
        {
            it->second = nullptr;
            it->second = value;
        }
        *pb = !inserted;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL remove(xaml_object* key) noexcept override
    {
        m_map.erase(key);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL clear() noexcept override
    {
        m_map.clear();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_hasher(xaml_hasher** ptr) noexcept override
    {
        auto hasher = m_map.hash_function().get_hasher();
        return hasher->query(ptr);
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator** ptr) noexcept override
    {
        return xaml_object_new<xaml_map_enumerator_impl>(ptr, m_map.begin(), m_map.end());
    }
};

xaml_result XAML_CALL xaml_map_new(inner_map_type&& map, xaml_map** ptr) noexcept
{
    return xaml_object_new<xaml_map_impl>(ptr, move(map));
}

xaml_result XAML_CALL xaml_map_new(xaml_map** ptr) noexcept
{
    return xaml_map_new({}, ptr);
}

xaml_result XAML_CALL xaml_map_new_with_hasher(xaml_hasher* hasher, xaml_map** ptr) noexcept
{
    return xaml_object_new<xaml_map_impl>(ptr, hasher);
}
