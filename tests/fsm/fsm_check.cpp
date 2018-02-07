#include <gtest/gtest.h>
#include <wlib/fsm/StateMachine.h>
#include <wlib/stl/ArrayList.h>

using namespace wlp;

class TestData : public EventData {
public:
    int value;
};

class TestMachine : public StateMachine {
public:
    TestMachine()
            : StateMachine(States::ST_MAX_STATES, States::ST_IDLE),
              m_value(0) {
    }

    void setValue(int value) {
        TestData data;
        data.value = value;
        BEGIN_TRANSITION_MAP
            TRANSITION_MAP_ENTRY(ST_START)        // ST_IDLE
            TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_STOP
            TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED) // ST_START
            TRANSITION_MAP_ENTRY(ST_CHANGE_SPEED) // ST_CHANGE_SPEED
        END_TRANSITION_MAP(&data, TestData)
    }

    void zero() {
        BEGIN_TRANSITION_MAP
            TRANSITION_MAP_ENTRY(EVENT_IGNORED)
            TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)
            TRANSITION_MAP_ENTRY(ST_STOP)
            TRANSITION_MAP_ENTRY(ST_STOP)
        END_TRANSITION_MAP(nullptr, NoEventData)
    }

    int m_value;
    /**
     * Used to trace the order of state transitions to
     * verify validity.
     */
    ArrayList<int> state_trace{25};

    enum States {
        ST_IDLE,
        ST_STOP,
        ST_START,
        ST_CHANGE_SPEED,
        ST_MAX_STATES
    };

private:
    STATE_DECLARE(TestMachine, Idle)
    STATE_DECLARE(TestMachine, Stop)
    STATE_DECLARE(TestMachine, Start, TestData)
    STATE_DECLARE(TestMachine, ChangeSpeed, TestData)

    BEGIN_STATE_MAP
        STATE_MAP_ENTRY(&Idle)
        STATE_MAP_ENTRY(&Stop)
        STATE_MAP_ENTRY(&Start)
        STATE_MAP_ENTRY(&ChangeSpeed)
    END_STATE_MAP
};

STATE_DEFINE(TestMachine, Idle) {
    state_trace.push_back((int) ST_IDLE);
}

STATE_DEFINE(TestMachine, Stop) {
    state_trace.push_back((int) ST_STOP);
    m_value = 0;
    internalEvent<NoEventData>(ST_IDLE);
}

STATE_DEFINE(TestMachine, Start, TestData) {
    state_trace.push_back((int) ST_START);
    m_value = data->value;
}

STATE_DEFINE(TestMachine, ChangeSpeed, TestData) {
    state_trace.push_back((int) ST_CHANGE_SPEED);
    m_value = data->value;
    if (m_value == 0) {
        internalEvent<NoEventData>(ST_IDLE);
    }
}

TEST(fsm_test, test_machine_transitions) {
    TestMachine sm{};
    int expected = TestMachine::States::ST_IDLE;
    int current = sm.getCurrentState();
    ASSERT_EQ(expected, current);
    ASSERT_EQ(0, sm.state_trace.size());
    sm.zero();
    // Event is ignored
    ASSERT_EQ(0, sm.state_trace.size());
    sm.setValue(10);
    ASSERT_EQ(10, sm.m_value);
    sm.zero();
    ASSERT_EQ(0, sm.m_value);
    sm.setValue(15);
    ASSERT_EQ(15, sm.m_value);
    sm.setValue(20);
    ASSERT_EQ(20, sm.m_value);
    sm.zero();
    ASSERT_EQ(0, sm.m_value);
    sm.zero();
    ASSERT_EQ(0, sm.m_value);
    int idle = TestMachine::States::ST_IDLE;
    int stop = TestMachine::States::ST_STOP;
    int start = TestMachine::States::ST_START;
    int changeSpeed = TestMachine::States::ST_CHANGE_SPEED;
    int expected_state_traverse[] = {
            start,
            stop,
            idle,
            start,
            changeSpeed,
            stop,
            idle
    };
    ASSERT_EQ(7, sm.state_trace.size());
    for (int i = 0; i < sm.state_trace.size(); i++) {
        ASSERT_EQ(expected_state_traverse[i], (int) sm.state_trace[(size_type) i]);
    }
}
