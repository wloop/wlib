/**
 * @file Exceptions.cpp
 * @brief Implementation of Exceptions using C style
 *
 * These exceptions have very little overhead and use very little memory
 *
 * @author Deep Dhillon
 * @date December 4, 2017
 * @bug No known bugs
 */

#include <stdlib.h>

#include <wlib/exceptions/Exceptions.h>
#include <wlib/mem/Memory.h>

using namespace wlp;

const char *except[4] = {
        "NullPtr Exception",
        "Bad Alloc Exception",
        "Logic Failure Exception",
        "Runtime Exception"
};

exception::exception(uint8_t type, const char *fileName, uint16_t lineNum, const char *message)
        : type{type}, fileName{fileName}, lineNum{lineNum}, message{message} {}

exception::~exception() = default;

uint16_t exception::getType() { return type; }

const char *exception::getName() { return except[type]; }

const char *exception::getFileName() { return fileName; }

uint16_t exception::getLineNum() { return lineNum; }

void exception::__setLineNum(uint16_t lineNum) { this->lineNum = lineNum; }

void exception::__setFileName(const char *fileName) { this->fileName = fileName; }

const char *exception::getMessage() { return message; }

jmp_buf *__exc_context = nullptr;
exception *__exception_ptr = nullptr;

void __exc_clear() {
    if (__exception_ptr == nullptr) { return; }

    free<exception>(__exception_ptr);
    __exception_ptr = nullptr;
}

void __exc_default_handler() {
#ifdef UNEXPECTED
    UNEXPECTED;
#endif
    __exc_clear();
}

#define DEFINE_EXCEPTION(Index, ExceptionName) \
class ExceptionName : public exception { \
public: \
    ExceptionName(const char *fileName, const uint16_t lineNum, const char *message) \
            : exception(Index, fileName, lineNum, message) {} \
};

DEFINE_EXCEPTION(0, NullPtrException)
DEFINE_EXCEPTION(1, BadAllocException)
DEFINE_EXCEPTION(2, LogicFailureException)
DEFINE_EXCEPTION(3, RuntimeException)
DEFINE_EXCEPTION(4, IllegalTransitionException)
DEFINE_EXCEPTION(5, UnexpectedStateException)
DEFINE_EXCEPTION(6, IndexException)
DEFINE_EXCEPTION(7, KeyException)
DEFINE_EXCEPTION(8, BadStateException)
DEFINE_EXCEPTION(9, BadWeakPtrException)

exception *__new_nullptr_exception(const char *message) {
    auto *exception = malloc<NullPtrException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_bad_alloc_exception(const char *message) {
    auto *exception = malloc<BadAllocException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_logic_failure_exception(const char *message) {
    auto *exception = malloc<LogicFailureException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_runtime_exception(const char *message) {
    auto *exception = malloc<RuntimeException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_illegal_transition_exception(const char *message) {
    auto *exception = malloc<IllegalTransitionException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_unexpected_state_exception(const char *message) {
    auto *exception = malloc<UnexpectedStateException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_index_exception(const char *message) {
    auto *exception = malloc<IndexException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_key_exception(const char *message) {
    auto *exception = malloc<KeyException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_bad_state_exception(const char *message) {
    auto *exception = malloc<BadStateException>(__FILE__, __LINE__, message);

    return exception;
}

exception *__new_bad_weak_ptr_exception(const char *message) {
    auto *exception = malloc<BadWeakPtrException>(__FILE__, __LINE__, message);

    return exception;
}
