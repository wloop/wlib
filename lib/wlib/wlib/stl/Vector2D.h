#ifndef EMBEDDEDCPLUSPLUS_VECTOR2D_H
#define EMBEDDEDCPLUSPLUS_VECTOR2D_H

#include "../utility/Tmp.h"

namespace wlp {

    template<typename Val>
    class Vector2D {
    public:
        typedef Val val_type;

        Vector2D() :
            m_x(0),
            m_y(0) {}

        Vector2D(val_type x, val_type y) :
            m_x(x),
            m_y(y) {}

        val_type &x() {
            return m_x;
        }

        val_type &y() {
            return m_y;
        }

        const val_type &x() const {
            return m_x;
        }

        const val_type &y() const {
            return m_y;
        }

        Vector2D<val_type> operator+(const Vector2D<val_type> &o) const {
            return {m_x + o.m_x, m_y + o.m_y};
        }

        Vector2D<val_type> operator-(const Vector2D<val_type> &o) const {
            return {m_x - o.m_x, m_y - o.m_y};
        }

        Vector2D<val_type> &operator+=(const Vector2D<val_type> &o) {
            m_x += o.m_x;
            m_y += o.m_y;
            return *this;
        }

        Vector2D<val_type> &operator-=(const Vector2D<val_type> &o) {
            m_x -= o.m_x;
            m_y -= o.m_y;
            return *this;
        }

        template<
            typename scalar_t,
            typename = typename enable_if<
                is_arithmetic<scalar_t>::value
            >::type
        >
        Vector2D<val_type> operator*(scalar_t b) {
            return {
                static_cast<val_type>(m_x * b),
                static_cast<val_type>(m_y * b)
            };
        };

        template<
            typename scalar_t,
            typename = typename enable_if<
                is_arithmetic<scalar_t>::value
            >::type
        >
        Vector2D<val_type> operator/(scalar_t b) {
            return {
                static_cast<val_type>(m_x / b),
                static_cast<val_type>(m_y / b)
            };
        };

    private:
        val_type m_x;
        val_type m_y;
    };

}

#endif //EMBEDDEDCPLUSPLUS_VECTOR2D_H
