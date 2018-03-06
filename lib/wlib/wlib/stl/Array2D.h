#ifndef EMBEDDEDCPLUSPLUS_ARRAY2D_H
#define EMBEDDEDCPLUSPLUS_ARRAY2D_H

#include "../Types.h"

namespace wlp {

    template<typename Val, typename Size>
    class __array2d_access {
    public:
        typedef Val val_type;
        typedef Size size_type;

        __array2d_access(val_type *sub_arr, size_type len)
            : m_sub_arr(sub_arr),
              m_len(len) {}

        val_type &operator[](size_type t) {
            if (t >= m_len) {
                return val_type();
            }
            return m_sub_arr[t];
        }

    private:
        val_type *m_sub_arr;
        size_type m_len;
    };

    template<typename Val, typename Size = wlp::size_type>
    class Array2D {
    public:
        typedef Val val_type;
        typedef Size size_type;

        Array2D(size_type x, size_type y)
            : m_x(x),
              m_y(y) {
            make_array(x, y);
        }

        Array2D(Array2D<val_type, size_type> &&arr) noexcept
            : m_x(arr.m_x),
              m_y(arr.m_y),
              m_arr(arr.m_arr) {
            arr.m_x = 0;
            arr.m_y = 0;
            arr.m_arr = nullptr;
        }

        ~Array2D() {
            if (m_arr) {
                delete_array();
            }
        }

        Array2D<val_type, size_type> &operator=(Array2D<val_type, size_type> &&arr) noexcept {
            m_x = arr.m_x;
            m_y = arr.m_y;
            m_arr = arr.m_arr;
            arr.m_x = 0;
            arr.m_y = 0;
            arr.m_arr = nullptr;
        };

        __array2d_access<val_type, size_type> operator[](size_type x) {
            return __array2d_access<val_type, size_type>(m_arr[x], m_y);
        };

        // Disable copy constructor and assignment
        Array2D(const Array2D<val_type, size_type> &) = delete;

        Array2D<val_type, size_type> &operator=(const Array2D<val_type, size_type> &) = delete;

    private:
        void make_array(int x, int y) {
            if (m_arr) {
                delete_array();
            }
            m_arr = new val_type *[x];
            for (int tx = 0; tx < x; ++tx) {
                m_arr = new val_type[y];
            }
        }

        void delete_array() noexcept {
            for (size_type x = 0; x < m_x; ++x) {
                delete[] m_arr[x];
            }
            delete[] m_arr;
        }

        val_type **m_arr = nullptr;
        size_type m_x = 0;
        size_type m_y = 0;
    };

}

#endif //EMBEDDEDCPLUSPLUS_ARRAY2D_H
