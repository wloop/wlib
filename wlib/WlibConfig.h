/**
 * @file WlibConfig.h
 * @brief Some basic definitions for compilers.
 *
 * __WLIB_CLASS_PARTIAL_SPECIALIZATION: defined if compiler supports class partial specialization
 * __WLIB_PARTIAL_SPECIALIZATION_SYNTAX: defined if compiler supports partial specialization syntax
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDTESTS_WLIBCONFIG_H
#define EMBEDDEDTESTS_WLIBCONFIG_H

# if defined(__sgi) && !defined(__GNUC__)
#   ifdef _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#     define __WLIB_CLASS_PARTIAL_SPECIALIZATION
#   endif
# endif

# ifdef __GNUC__
# if __GNUC__ >= 2 && (__GNUC__ != 2 || __GNUC_MINOR__ >= 8)
#   define __WLIB_CLASS_PARTIAL_SPECIALIZATION
# endif
# endif

# if defined(__COMO__)
#   define __WLIB_CLASS_PARTIAL_SPECIALIZATION
# endif

# if defined(_MSC_VER)
#   if _MSC_VER >= 1200
#     define __WLIB_PARTIAL_SPECIALIZATION_SYNTAX
#   endif
# endif

# if defined(__WLIB_CLASS_PARTIAL_SPECIALIZATION) || defined (__WLIB_PARTIAL_SPECIALIZATION_SYNTAX)
#   define TEMPLATE_NULL template<>
# else
#   define TEMPLATE_NULL
# endif

#endif //EMBEDDEDTESTS_WLIBCONFIG_H
