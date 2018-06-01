#ifndef EMBEDDEDCPLUSPLUS_VECTOR2D_H
#define EMBEDDEDCPLUSPLUS_VECTOR2D_H

#include <math.h>

#include <wlib/stl/InitializerList.h>
#include <wlib/utility/Tmp.h>

namespace wlp {

    template<typename val_t>
    class vector2d {
    public:
        vector2d() :
            m_x(0),
            m_y(0) {}

        vector2d(val_t x, val_t y) :
            m_x(x),
            m_y(y) {}

        vector2d(wlp::initializer_list<val_t> l) :
            m_x(l.begin()[0]),
            m_y(l.begin()[1]) {}

        template<typename u_val_t>
        vector2d(wlp::initializer_list<u_val_t> l) :
            m_x(static_cast<val_t>(l.begin()[0])),
            m_y(static_cast<val_t>(l.begin()[1])) {}

        template<typename u_val_t>
        vector2d(const vector2d<u_val_t> &p) :
            m_x(static_cast<val_t>(p.x())),
            m_y(static_cast<val_t>(p.y())) {}

        val_t &x() {
            return m_x;
        }

        val_t &y() {
            return m_y;
        }

        val_t norm() const {
            return sqrt(norm_sq());
        }

        val_t norm_sq() const {
            return m_x * m_x + m_y * m_y;
        }

        const val_t &x() const {
            return m_x;
        }

        const val_t &y() const {
            return m_y;
        }

        vector2d<val_t> n() const {
            return *this / norm();
        }

        vector2d<val_t> n_inv() const {
            val_t n = norm();
            return {n / m_x, n / m_y};
        }

        vector2d<val_t> &operator=(wlp::initializer_list<val_t> l) {
            m_x = l.begin()[0];
            m_y = l.begin()[1];
            return *this;
        }

        template<typename u_val_t>
        vector2d<val_t> &operator=(wlp::initializer_list<u_val_t> l) {
            m_x = static_cast<val_t>(l.begin()[0]);
            m_y = static_cast<val_t>(l.begin()[1]);
            return *this;
        }

        vector2d<val_t> operator+(const vector2d<val_t> &o) const {
            return {m_x + o.m_x, m_y + o.m_y};
        }

        vector2d<val_t> operator-(const vector2d<val_t> &o) const {
            return {m_x - o.m_x, m_y - o.m_y};
        }

        vector2d<val_t> &operator+=(const vector2d<val_t> &o) {
            m_x += o.m_x;
            m_y += o.m_y;
            return *this;
        }

        vector2d<val_t> &operator-=(const vector2d<val_t> &o) {
            m_x -= o.m_x;
            m_y -= o.m_y;
            return *this;
        }

        bool operator==(const vector2d<val_t> &o) const {
            return (m_x == o.m_x) && (m_y == o.m_y);
        }

        bool operator!=(const vector2d<val_t> &o) const {
            return (m_x != o.m_x) || (m_y != o.m_y);
        }

        template<
            typename scalar_t,
            typename = typename enable_if<
                is_arithmetic<scalar_t>::value
            >::type
        >
        vector2d<val_t> operator*(scalar_t b) const {
            return {
                static_cast<val_t>(m_x * b),
                static_cast<val_t>(m_y * b)
            };
        };

        template<
            typename scalar_t,
            typename = typename enable_if<
                is_arithmetic<scalar_t>::value
            >::type
        >
        vector2d<val_t> operator/(scalar_t b) const {
            return {
                static_cast<val_t>(m_x / b),
                static_cast<val_t>(m_y / b)
            };
        };

        val_t dot(const vector2d<val_t> &v) const {
            return m_x * v.m_x + m_y * v.m_y;
        }

        val_t cross(const vector2d<val_t> &w) const {
            return m_x * w.m_y - m_y * w.m_x;
        }

    private:
        val_t m_x;
        val_t m_y;
    };

}

#endif //EMBEDDEDCPLUSPLUS_VECTOR2D_H
