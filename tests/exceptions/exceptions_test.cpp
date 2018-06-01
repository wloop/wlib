#include <gtest/gtest.h>
#include <wlib/memory/Memory.h>
#include <wlib/exceptions/Exceptions.h>

TEST(exceptions_test, simple_try_catch) {
    int h = 0;

    exception *logicException = LOGIC_FAILURE_EXCEPTION("my bad");

    TRY
        h += 4;
        THROW(logicException);
    CATCH(e)
        h += 2;
            ASSERT_EQ(logicException->getType(), e->getType());
    TRY_END

    ASSERT_EQ(6, h);
}

TEST(exception_test, different_exceptions) {
    int counter = 0;

    exception *nullptrException = NULLPTR_EXCEPTION("my bad");
    exception *badallocException = BAD_ALLOC_EXCEPTION("my bad");
    exception *logicException = LOGIC_FAILURE_EXCEPTION("my bad");
    exception *runtimeException = RUNTIME_EXCEPTION("my bad");

    TRY
        counter += 1;
        THROW(nullptrException);
    CATCH(e)
        counter += 1;
        ASSERT_EQ(nullptrException->getType(), e->getType());
        ASSERT_STREQ(nullptrException->getName(), e->getName());
    TRY_END

    TRY
        counter += 1;
        THROW(badallocException);
    CATCH(e)
        counter += 1;
        ASSERT_EQ(badallocException->getType(), e->getType());
        ASSERT_STREQ(badallocException->getName(), e->getName());
    TRY_END

    TRY
        counter += 1;
        THROW(logicException);
    CATCH(e)
        counter += 1;
        ASSERT_EQ(logicException->getType(), e->getType());
        ASSERT_STREQ(logicException->getName(), e->getName());
    TRY_END

    TRY
        counter += 1;
        THROW(runtimeException);
    CATCH(e)
        counter += 1;
        ASSERT_EQ(runtimeException->getType(), e->getType());
        ASSERT_STREQ(runtimeException->getName(), e->getName());
    TRY_END

    ASSERT_EQ(8, counter);
}

TEST(exception_test, embedded_exceptions) {
    int counter = 0;

    TRY
        TRY
            TRY
                ++counter;
                THROW(NULLPTR_EXCEPTION("HEEEE"));
            CATCH(e)
                ASSERT_EQ(__LINE__ - 2, e->getLineNum());
                ++counter;
                THROW(e);
            TRY_END
        CATCH(e)
            ASSERT_EQ(__LINE__ - 3, e->getLineNum());
            ++counter;
            THROW(e);
        TRY_END
    CATCH(e)
        ASSERT_EQ(__LINE__ - 3, e->getLineNum());
        ASSERT_STREQ("HEEEE", e->getMessage());
        ++counter;
    TRY_END

    ASSERT_EQ(4, counter);
}

void func1() {
    THROW(LOGIC_FAILURE_EXCEPTION("RIP"))
}

void func2() {
    func1();
}

void func3() {
    func2();
}

TEST(exception_test, multifunction_exception) {
    TRY
        func3();
    CATCH(e)
        ASSERT_STREQ(__FILE__, e->getFileName());
        ASSERT_STREQ("RIP", e->getMessage());
        ASSERT_STREQ("Logic Failure Exception", e->getName());
        ASSERT_EQ(__LINE__ - 18, e->getLineNum());
    TRY_END
}

TEST(exception_test, unexpected_exception) {
    wlp::size32_type startMem;
    wlp::size32_type endMem;

    startMem = getTotalMemoryFree();
    while (true){
        TRY
            func1();
        CATCH(e)
            THROW(e);
            break;
        TRY_END
    }

    endMem = getTotalMemoryFree();

    // since there was break and TRY_END was not used the memory
    // was cleaned by the default handler that handles when an
    // exception is not caught
    ASSERT_EQ(startMem, endMem);
}

class Sample {
public:
    static int constr;

    Sample() {
        constr += 4;
    }

    ~Sample() {
        constr += 5;
    }
};

TEST(exceptions_test, scopes) {
    Sample::constr = 0;

    TRY
        int y = 7;
        wlp::swallow(y);
    CATCH(e)
        int y = 8;
        wlp::swallow(y, e);
    TRY_END

    // y was only in the scope of TRY and CATCH
    int y = 9;
    wlp::swallow(y);

    TRY
        Sample obj;
    CATCH(e)
        wlp::swallow(e);
    TRY_END

    // objects are destroyed after that scope is gone
    ASSERT_EQ(9, Sample::constr);
}
