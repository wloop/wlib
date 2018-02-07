/**
 * @file Exceptions.h
 * @brief Implementation of Exceptions using C style
 *
 * These exceptions have very little overhead and use very little memory
 *
 * @author Deep Dhillon
 * @date December 4, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_EXCEPTIONS_H
#define EMBEDDEDCPLUSPLUS_EXCEPTIONS_H

#include <setjmp.h>
#include "wlib/Types.h"

/**
 * This is used to define the behaviour when there is no catch block and error is thrown
 * To access the Exception ptr use: @code __exception_ptr @endcode. Make sure to check if
 * this is null or not
 */
#define UNEXPECTED \

/**
 * Exception base class
 */
class Exception {
public:
    /**
     * Constructor for Exception takes exception type, the name of the file from where
     * the exception is thrown, line number and custom message
     *
     * @param type exception type
     * @param fileName file from where exception is thrown
     * @param lineNum line number of throw statement
     * @param message custom message
     */
    Exception(uint8_t type, const char *fileName, uint16_t lineNum, const char *message);

    virtual ~Exception() = 0;

    /**
     * Returns the type of exception. It is a numerical number between 0 and 255
     *
     * @return exception type
     */
    uint16_t getType();

    /**
     * Returns the name of the exception that was thrown
     *
     * @return the name of exception
     */
    const char *getName();

    /**
     * Returns the name of file from where the exception was thrown
     *
     * @return the file name
     */
    const char *getFileName();

    /**
     * Used internally to set the file name
     *
     * @param fileName the name of file
     */
    void __setFileName(const char* fileName);

    /**
     * Returns the number of line where the throw statement was
     *
     * @return the throw line number
     */
    uint16_t getLineNum();

    /**
     * Used internally to set the line number
     *
     * @param lineNum the throw line number
     */
    void __setLineNum(uint16_t lineNum);

    /**
     * Returns the custom message that was provided to the error
     *
     * @return the custom message
     */
    const char *getMessage();


private:
    const uint8_t type;
    const char *fileName;
    uint16_t lineNum;
    const char *message;
};

extern jmp_buf *__exc_context;
extern Exception *__exception_ptr;

void __exc_clear();
void __exc_default_handler();

#define TRY { \
    jmp_buf __new_context; \
    jmp_buf *__prev_context = __exc_context; \
    __exc_context = &__new_context; \
    if (setjmp(__new_context) == 0) {

#define CATCH(e) \
    __exc_context = __prev_context; \
    } else { \
    Exception *e = __exception_ptr; \
    __exc_context = __prev_context;

#define TRY_END \
    __exc_clear(); } } // END_SCOPE

#define THROW(e) { \
    uint16_t lineNum = __LINE__; \
    Exception *__e = (e); \
    __e->__setFileName(__FILE__); \
    __e->__setLineNum(lineNum); \
    if (__e && __e != __exception_ptr) { \
    __exc_clear(); \
    __exception_ptr = __e; \
    } \
    \
    if (__exc_context) longjmp(*__exc_context, 1); \
    __exc_default_handler(); }


/**
 * Exceptions to use and their macros. More can be added as we go
 */
Exception *__new_nullptr_exception(const char *message = "");

Exception *__new_bad_alloc_exception(const char *message = "");

Exception *__new_logic_failure_exception(const char *message = "");

Exception *__new_runtime_exception(const char *message = "");

Exception *__new_illegal_transition_exception(const char *message = "");

Exception *__new_unexpected_state_exception(const char *message = "");

Exception *__new_index_exception(const char *message = "");

Exception *__new_key_exception(const char *message = "");

Exception *__new_bad_state_exception(const char *message = "");

Exception *__new_bad_weak_ptr_exception(const char *message = "");

#define NULLPTR_EXCEPTION(str) __new_nullptr_exception(str)
#define BAD_ALLOC_EXCEPTION(str) __new_bad_alloc_exception(str)
#define LOGIC_FAILURE_EXCEPTION(str) __new_logic_failure_exception(str)
#define RUNTIME_EXCEPTION(str) __new_runtime_exception(str)
#define ILLEGAL_TRANSITION_EXCEPTION(str) __new_illegal_transition_exception(str)
#define UNEXPECTED_STATE_EXCEPTION(str) __new_unexpected_state_exception(str)
#define INDEX_EXCEPTION(str) __new_index_exception(str)
#define KEY_EXCEPTION(str) __new_key_exception(str)
#define BAD_STATE_EXCEPTION(str) __new_bad_state_exception(str)
#define BAD_WEAK_PTR_EXCEPTION(str) __new_bad_weak_ptr_exception(str)

#endif //EMBEDDEDCPLUSPLUS_EXCEPTIONS_H
