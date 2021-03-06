#include <xaml/object.h>
#include <xaml/vector.h>

using namespace std;

using inner_vector_type = vector<xaml_ptr<xaml_object>>;

struct xaml_vector_enumerator_impl : xaml_implement<xaml_vector_enumerator_impl, xaml_enumerator, xaml_object>
{
private:
    inner_vector_type::const_iterator m_begin, m_end;
    bool m_init;

public:
    xaml_vector_enumerator_impl(inner_vector_type::const_iterator begin, inner_vector_type::const_iterator end) noexcept
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
        return (*m_begin)->query(ptr);
    }
};

struct xaml_vector_impl : xaml_implement<xaml_vector_impl, xaml_vector, xaml_vector_view, xaml_enumerable, xaml_object>
{
private:
    inner_vector_type m_vec{};

public:
    xaml_vector_impl(inner_vector_type&& vec) noexcept : m_vec(move(vec)) {}

    xaml_result XAML_CALL get_size(int32_t* psize) noexcept override
    {
        *psize = (int32_t)m_vec.size();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL index_of(xaml_object* value, int32_t* pindex) noexcept override
    {
        for (size_t i = 0; i < m_vec.size(); i++)
        {
            if (m_vec[i].get() == value)
            {
                *pindex = (int32_t)i;
                return XAML_S_OK;
            }
        }
        *pindex = -1;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_at(int32_t index, xaml_object** ptr) noexcept override
    {
        if (index < 0 || index >= (int32_t)m_vec.size()) return XAML_E_OUTOFBOUNDS;
        auto& res = m_vec[index];
        return res->query(ptr);
    }

    xaml_result XAML_CALL set_at(int32_t index, xaml_object* obj) noexcept override
    {
        if (index < 0 || index >= (int32_t)m_vec.size()) return XAML_E_OUTOFBOUNDS;
        m_vec[index] = obj;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL append(xaml_object* obj) noexcept override
    try
    {
        m_vec.push_back(obj);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result XAML_CALL insert_at(int32_t index, xaml_object* obj) noexcept override
    try
    {
        if (index < 0 || index >= (int32_t)m_vec.size()) return XAML_E_OUTOFBOUNDS;
        m_vec.insert(m_vec.begin() + index, obj);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result XAML_CALL remove_at(int32_t index) noexcept override
    {
        if (index < 0 || index >= (int32_t)m_vec.size()) return XAML_E_OUTOFBOUNDS;
        m_vec.erase(m_vec.begin() + index);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL remove_at_end() noexcept override
    {
        if (m_vec.empty()) return XAML_E_OUTOFBOUNDS;
        m_vec.pop_back();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL clear() noexcept override
    {
        m_vec.clear();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator** ptr) noexcept override
    {
        return xaml_object_new<xaml_vector_enumerator_impl>(ptr, m_vec.begin(), m_vec.end());
    }
};

xaml_result XAML_CALL xaml_vector_new(inner_vector_type&& vec, xaml_vector** ptr) noexcept
{
    return xaml_object_new<xaml_vector_impl>(ptr, move(vec));
}

xaml_result XAML_CALL xaml_vector_new(xaml_vector** ptr) noexcept
try
{
    return xaml_vector_new({}, ptr);
}
XAML_CATCH_RETURN()
