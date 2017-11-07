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

/*
 * TODO: might not be needed if we enforce gcc 5 all the time
 */

#ifndef EMBEDDEDTESTS_WLIBCONFIG_H
#define EMBEDDEDTESTS_WLIBCONFIG_H

#if defined(__sgi) && !defined(__GNUC__)
#    ifdef _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#        define __WLIB_CLASS_PARTIAL_SPECIALIZATION
#    endif
#    if (_COMPILER_VERSION >= 721) && define(_NAMESPACES)
#        define __WLIB_HAS_NAMESPACES
#    endif
#endif

#ifdef __GNUC__
#if __GNUC__ >= 2 && (__GNUC__ != 2 || __GNUC_MINOR__ >= 8)
#    define __WLIB_CLASS_PARTIAL_SPECIALIZATION
#    define __WLIB_HAS_NAMESPACES
#endif
#endif

#if defined(__COMO__)
#    define __WLIB_CLASS_PARTIAL_SPECIALIZATION
#    define __WLIB_HAS_NAMESPACES
#endif

#if defined(_MSC_VER)
#    if _MSC_VER >= 1200
#        define __WLIB_PARTIAL_SPECIALIZATION_SYNTAX
#        define __WLIB_HAS_NAMESPACES
#    endif
#endif

#if defined(__WLIB_CLASS_PARTIAL_SPECIALIZATION) || defined (__WLIB_PARTIAL_SPECIALIZATION_SYNTAX)
#    define TEMPLATE_NULL template<>
#else
#    define TEMPLATE_NULL
#endif

#if defined(__WLIB_HAS_NAMESPACES)
#    define NAMESPACE_START namespace wlp {
#    define NAMESPACE_END }
#else
#    define NAMESPACE_START
#    define NAMESPACE_END
#endif

#endif //EMBEDDEDTESTS_WLIBCONFIG_H
