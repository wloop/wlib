#ifndef EMBEDDEDCPLUSPLUS_ARRAY2D_H
#define EMBEDDEDCPLUSPLUS_ARRAY2D_H

#include <string.h>

#include <wlib/Types.h>
#include <wlib/stl/InitializerList.h>
#include <wlib/util/Utility.h>

namespace wlp {

    template<typename val_t, typename size_t>
    class __array2d_access {
    public:

        __array2d_access(val_t *sub_arr, size_t len)
            : m_sub_arr(sub_arr),
              m_len(len) {}

        val_t *get() {
            return m_sub_arr;
        }

        val_t &operator[](size_t t) {
            return m_sub_arr[t];
        }

    private:
        val_t *m_sub_arr;
        size_t m_len;
    };

    template<typename val_t, typename size_t = wlp::size_type>
    class array2d {
    public:

        array2d(size_t x, size_t y)
            : m_x(x),
              m_y(y) {
            make_array(x, y);
        }

        array2d(wlp::initializer_list<wlp::initializer_list<val_t>> l) {
            m_x = l.size();
            m_y = l.begin()->size();
            make_array(m_x, m_y);

            int i = 0;
            for (auto &array : l) {
                int j = 0;
                for (auto &e : array) {
                    m_arr[i][j] = wlp::move(e);
                    j++;
                }
                i++;
            }
        }

        array2d(array2d<val_t, size_t> &&arr) noexcept
            : m_x(arr.m_x),
              m_y(arr.m_y),
              m_arr(arr.m_arr) {
            arr.m_x = 0;
            arr.m_y = 0;
            arr.m_arr = nullptr;
        }

        ~array2d() {
            if (m_arr) {
                delete_array();
            }
        }

        size_t x() const {
            return m_x;
        }

        size_t y() const {
            return m_y;
        }

        size_t xy() const {
            return m_x * m_y;
        }

        void zero_clear() {
            for (size_t x = 0; x < m_x; ++x) {
                memset(m_arr[x], 0, m_y * sizeof(val_t));
            }
        }

        array2d<val_t, size_t> &operator=(array2d<val_t, size_t> &&arr) noexcept {
            m_x = arr.m_x;
            m_y = arr.m_y;
            m_arr = arr.m_arr;
            arr.m_x = 0;
            arr.m_y = 0;
            arr.m_arr = nullptr;
            return *this;
        };

        val_t **get() {
            return m_arr;
        }

        __array2d_access<val_t, size_t> operator[](size_t x) {
            return __array2d_access<val_t, size_t>(m_arr[x], m_y);
        };

        // Disable copy constructor and assignment
        array2d(const array2d<val_t, size_t> &) = delete;

        array2d<val_t, size_t> &operator=(const array2d<val_t, size_t> &) = delete;

    private:
        void make_array(int x, int y) {
            if (m_arr) {
                delete_array();
            }
            m_arr = new val_t *[x];
            for (int tx = 0; tx < x; ++tx) {
                m_arr[tx] = new val_t[y];
                memset(m_arr[tx], 0, y * sizeof(val_t));
            }
        }

        void delete_array() noexcept {
            for (size_t x = 0; x < m_x; ++x) {
                delete[] m_arr[x];
            }
            delete[] m_arr;
        }

        size_t m_x = 0;
        size_t m_y = 0;
        val_t **m_arr = nullptr;
    };

}

#endif //EMBEDDEDCPLUSPLUS_ARRAY2D_H
