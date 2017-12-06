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
#include "../memory/Memory.h"
#include "Exceptions.h"

using namespace wlp;

const char *except[4] = {
        "NullPtr Exception",
        "Bad Alloc Exception",
        "Logic Failure Exception",
        "Runtime Exception"
};

Exception::Exception(uint8_t type, const char *fileName, uint16_t lineNum, const char *message)
        : type{type}, fileName{fileName}, lineNum{lineNum}, message{message} {}

Exception::~Exception() = default;

uint16_t Exception::getType() { return type; }

const char *Exception::getName() { return except[type]; }

const char *Exception::getFileName() { return fileName; }

uint16_t Exception::getLineNum() { return lineNum; }

void Exception::__setLineNum(uint16_t lineNum) { this->lineNum = lineNum; }

void Exception::__setFileName(const char* fileName) { this->fileName = fileName; }

const char *Exception::getMessage() { return message; }

jmp_buf *__exc_context = nullptr;
Exception *__exception_ptr = nullptr;

void __exc_clear() {
    if (__exception_ptr == nullptr) return;

    free<Exception>(__exception_ptr);
    __exception_ptr = nullptr;
}

void __exc_default_handler() {
#ifdef UNEXPECTED
    UNEXPECTED;
#endif
    __exc_clear();
}

class NullPtrException : public Exception {
public:
    NullPtrException(const char *fileName, const uint16_t lineNum, const char *message)
            : Exception(0, fileName, lineNum, message) {}
};

class BadAllocException : public Exception {
public:
    BadAllocException(const char *fileName, const uint16_t lineNum, const char *message)
            : Exception(1, fileName, lineNum, message) {}
};

class LogicFailureException : public Exception {
public:
    LogicFailureException(const char *fileName, const uint16_t lineNum, const char *message)
            : Exception(2, fileName, lineNum, message) {}
};

class RuntimeException : public Exception {
public:
    RuntimeException(const char *fileName, const uint16_t lineNum, const char *message)
            : Exception(3, fileName, lineNum, message)  {}
};

Exception *__new_nullptr_exception(const char *message) {
    auto *exception = malloc<NullPtrException>(__FILE__, __LINE__, message);

    return exception;
}

Exception *__new_bad_alloc_exception(const char *message) {
    auto *exception = malloc<BadAllocException>(__FILE__, __LINE__, message);

    return exception;
}

Exception *__new_logic_failure_exception(const char *message) {
    auto *exception = malloc<LogicFailureException>(__FILE__, __LINE__, message);

    return exception;
}

Exception *__new_runtime_exception(const char *message) {
    auto *exception = malloc<RuntimeException>(__FILE__, __LINE__, message);

    return exception;
}
