#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "../Wlib.h"
#include "../Types.h"

namespace wlp {

    /**
     * Unique state machine event data must
     * inherit from this class.
     */
    class EventData {
    public:
        virtual ~EventData() {}
    };

    typedef EventData NoEventData;

    class StateMachine;

    /**
     * Abstract state base class that all
     * states inherit from.
     */
    class StateBase {
    public:
        virtual ~StateBase() {}

        /**
         * Called by the state machine engine to execute a
         * state action. If a guard condition exists and it
         * evaluates to false, the state action will not
         * execute.
         *
         * @param sm a state machine instance
         * @param data the event data
         */
        virtual void InvokeStateAction(StateMachine *sm, const EventData *data) const = 0;
    };

    /**
     * This class represents the action that occurs when
     * a new state is executed.
     *
     * @tparam SM state machine class
     * @tparam Data state function event data type
     * @tparam Func state machine member function pointer
     */
    template<class SM, class Data, void (SM::*Func)(const Data *)>
    class StateAction : public StateBase {
    public:
        virtual ~StateAction() {}

        /**
         * @see StateBase::InvokeStateAction
         */
        virtual void InvokeStateAction(StateMachine *sm, const EventData *data) const {
            // Downcast the state machine and event data to the correct derived type
            SM *derivedSM = static_cast<SM *>(sm);
            /*
             * If the derived data dynamic cast returns a null pointer, then
             * there is a mismatch between the event type in STATE_DECLARE and
             * the event type passed to the fucntion.
             */
            const Data *derivedData = dynamic_cast<const Data *>(data);
            /* assert(derivedData != nullptr) */
            if (derivedData == nullptr) {
                return;
            }
            // Call the state function
            (derivedSM->*Func)(derivedData);
        }
    };

    /**
     * Abstract guard base class that all
     * guards classes inherit from.
     */
    class GuardBase {
    public:
        virtual ~GuardBase() {}

        /**
         * Called by the state machine engine to execute
         * a guard condition action. If guard condition
         * evaluates to true, the state action is executed.
         * If false, no state transition is performed.
         *
         * @param sm a state machine instance
         * @param data the event data
         * @return true if no guard condition or if the
         * condition evaluates to true
         */
        virtual bool InvokeGuardCondition(StateMachine *sm, const EventData *data) const = 0;
    };

    /**
     * The class represents a guard condition that
     * must be passed when transitioning to a new state.
     *
     * @tparam SM state machine class
     * @tparam Data state function event data type
     * @tparam Func state machine member function pointer
     */
    template<class SM, class Data, bool (SM::*Func)(const Data *)>
    class GuardCondition : public GuardBase {
    public:
        virtual ~GuardCondition() {}

        virtual bool InvokeGuardCondition(StateMachine *sm, const EventData *data) const {
            SM *derivedSM = static_cast<SM *>(sm);
            const Data *derivedData = dynamic_cast<const Data *>(data);
            /* assert(derivedData != nullptr) */
            if (derivedData == nullptr) {
                return false;
            }
            // Call the guard function
            return (derivedSM->*Func)(derivedData);
        }
    };

    /**
     * Abstract entry base class that all
     * entry classes inherit from.
     */
    class EntryBase {
    public:
        virtual ~EntryBase() {}

        /**
         * Called by the state machine engine to execute
         * a state entry action. Called when entering a state.
         *
         * @param sm a state machine instance
         * @param data the event data
         */
        virtual void InvokeEntryAction(StateMachine *sm, const EventData *data) const = 0;
    };

    /**
     * This class represents an action that is executed
     * upon entry into a new state.
     *
     * @tparam SM state machine class
     * @tparam Data state function event data type
     * @tparam Func state machine member function pointer
     */
    template<class SM, class Data, void (SM::*Func)(const Data *)>
    class EntryAction : public EntryBase {
    public:
        virtual ~EntryAction() {}

        virtual void InvokeEntryAction(StateMachine *sm, const EventData *data) const {
            SM *derivedSM = static_cast<SM *>(sm);
            const Data *derivedData = dynamic_cast<const Data *>(data);
            /* assert(derivedData != nullptr) */
            if (derivedData == nullptr) {
                return;
            }
            // Call the entry function
            (derivedSM->*Func)(derivedData);
        }
    };

    /**
     * Abstract exit base class that all
     * exit classes inherit from.
     */
    class ExitBase {
    public:
        virtual ~ExitBase() {}

        /**
         * Called by the state machine engine to exeucte a
         * state exit action. Called when leaving a state.
         *
         * @param sm a state machine instance
         */
        virtual void InvokeExitAction(StateMachine *sm) const = 0;
    };

    /**
     * This class represents an action that is executed
     * upon exiting a state.
     *
     * @tparam SM state machine class
     * @tparam Func state machine member function pointer
     */
    template<class SM, void (SM::*Func)(void)>
    class ExitAction : public ExitBase {
    public:
        virtual ~ExitAction() {}

        virtual void InvokeExitAction(StateMachine *sm) const {
            SM *derivedSM = static_cast<SM *>(sm);

            // Call the exit function
            (derivedSM->*Func)();
        }
    };

    /**
     * A structure to hold a single row within the state map.
     */
    struct StateMapRow {
        const StateBase *const State;
    };

    /**
     * A structure to hold a single row within the extended state map.
     */
    struct StateMapRowEx {
        const StateBase *const State;
        const GuardBase *const Guard;
        const EntryBase *const Entry;
        const ExitBase *const Exit;
    };

    /**
     * StateMachine implements a software-based state machine.
     */
    class StateMachine {
    public:
        enum {
            EVENT_IGNORED = 0xfe, CANNOT_HAPPEN
        };

        /**
         * Constructor for the state machine defines the
         * maximum number of states the machine may occupy
         * and the initial state of the machine
         *
         * There can be at most 254 states whose ordinal values
         * range from 0 to 253 inclusive. If the number of states
         * exceeds this range, behaviour is undefined.
         *
         * @param maxStates machine number of states
         * @param initialState initial machine state
         */
        StateMachine(state_type maxStates, state_type initialState = 0);

        virtual ~StateMachine() {}

        /**
         * @return the current state machine state
         * as an ordinal value
         */
        state_type getCurrentState() { return m_current_state; }

        /**
         * @return the maximum ordinal value that
         * a state may have
         */
        state_type getMaxStates() { return m_max_states; }

    protected:
        /**
         * Trigger an external state machine event.
         *
         * @param newState the state machine state to
         * which the machine should transition
         * @param pData the event data to send
         * to the new state
         */
        void externalEvent(state_type newState, const EventData *pData = nullptr);

        /**
         * Trigger an interval state machine event.
         * These events are geneated while executing a state
         * machine state.
         *
         * @param newState the state machine state to
         * which the machine should transition
         * @param pData the event data to send
         * to the new state
         */
        void internalEvent(state_type newState, const EventData *pData = nullptr);

    private:
        /**
         * The maximum number of state machine states,
         * which also corresponds to the strict upper
         * bound on the machine state ordinal value.
         */
        const state_type m_max_states;

        /**
         * The current state machine state as an
         * ordinal value.
         */
        state_type m_current_state;

        /**
         * The new state machine state to which the
         * machine has yet to make the transition.
         */
        state_type m_new_state;

        /**
         * This value is set to true when an event
         * is generated.
         */
        bool m_event_generated;

        /**
         * The state event data pointer.
         */
        const EventData *m_event_data;

        /**
         * Get the state map as defined in a derived class of State Machine.
         * The @code BEGIN_STATE_MAP @endcode, @code STATE_MAP_ENTRY @endcode, and
         * @code END_STATE_MAP @endcode macros are used to assist in constructing
         * the map. Derived classes only need to implement one of @code getStateMap @endcode
         * and @code getStateMapEx @endcode.
         *
         * @return an array of state map rows with an array size of
         * @code MAX_STATES @endcode or nullptr if the state machine
         * uses @code getStateMapEx @endcode
         */
        virtual const StateMapRow *getStateMap() = 0;

        /**
         * Get the extended state map as defined in the derived class of State Machine.
         * The @code BEGIN_STATE_MAP_EX @endcode, @code STATE_MAP_ENTRY_EX @endcode,
         * @code STATE_MAP_ENTRY_ALL_EX @endcode, and @code END_STATE_MAP_EX @endcode are
         * used to assist in constructing the map.
         *
         * @return an array of extended state map rows of size @code MAX_STATES @endcode
         * or nullptr if the state machine uses @code getStateMap @endcode
         */
        virtual const StateMapRowEx *getStateMapEx() = 0;

        /**
         * Set a new current state.
         *
         * @param newState the new state of the machine
         */
        void setCurrentState(state_type newState) { m_current_state = newState; }

        /**
         * The state machine engine that executes the external events and, optionally,
         * all internal events generated during state execution.
         *
         * This function will pull the state map from the state
         * map definition. If both the state map and the extended
         * state map are undefined, no execution occurs.
         */
        void stateEngine(void);

        /**
         * Execute the state transition as per the state map.
         *
         * If at any moment an invalid state is provided, that is
         * its ordinal value is not strictly less than the
         * maximum number of states, or a state in the map is
         * undefined, results are undefined.
         *
         * @param pStateMap the state machine state map
         */
        void stateEngine(const StateMapRow *const pStateMap);

        /**
         * Execute the state transition as per the state map.
         * This function also checks against guards and executes
         * entry and exit actions.
         *
         * If at any moment an invalid state is provided, that is
         * its ordinal value is not strictly less than the
         * maximum number of states, or a state in the map is
         * undefined, or an entry/exit action makes a call to
         * interval event, results are undefined.
         *
         * @param pStateMapEx the state machine extended state map
         */
        void stateEngine(const StateMapRowEx *const pStateMapEx);
    };

}


/**
 * Macros used to declare and define state machine
 * states, guards, entries, and exits.
 */
#define STATE_DECLARE(...) VFUNC(STATE_DECLARE, __VA_ARGS__)
#define STATE_DEFINE(...) VFUNC(STATE_DEFINE, __VA_ARGS__)
#define STATE_DECLARE2(stateMachine, stateName) \
    void ST_##stateName(const NoEventData*); \
    StateAction<stateMachine, NoEventData, &stateMachine::ST_##stateName> stateName;
#define STATE_DECLARE3(stateMachine, stateName, eventData) \
    void ST_##stateName(const eventData*); \
    StateAction<stateMachine, eventData, &stateMachine::ST_##stateName> stateName;
#define STATE_DEFINE2(stateMachine, stateName) \
    void stateMachine::ST_##stateName(const NoEventData*)
#define STATE_DEFINE3(stateMachine, stateName, eventData) \
    void stateMachine::ST_##stateName(const eventData* data)

#define GUARD_DECLARE(...) VFUNC(GUARD_DECLARE, __VA_ARGS__)
#define GUARD_DEFINE(...) VFUNC(GUARD_DEFINE, __VA_ARGS__)
#define GUARD_DECLARE2(stateMachine, guardName) \
    bool GD_##guardName(const NoEventData*); \
    GuardCondition<stateMachine, NoEventData, &stateMachine::GD_##guardName> guardName;
#define GUARD_DECLARE3(stateMachine, guardName, eventData) \
    bool GD_##guardName(const eventData*); \
    GuardCondition<stateMachine, eventData, &stateMachine::GD_##guardName> guardName;
#define GUARD_DEFINE2(stateMachine, guardName) \
    bool stateMachine::GD_##guardName(const NoEventData*)
#define GUARD_DEFINE3(stateMachine, guardName, eventData) \
    bool stateMachine::GD_##guardName(const eventData* data)

#define ENTRY_DELCARE(...) VFUNC(ENTRY_DELCARE, __VA_ARGS__)
#define ENTRY_DEFINE(...) VFUNC(ENTRY_DEFINE, __VA_ARGS__)
#define ENTRY_DECLARE2(stateMachine, entryName) \
    void EN_##entryName(const NoEventData*); \
    EntryAction<stateMachine, NoEventData, &stateMachine::EN_##entryName> entryName;
#define ENTRY_DECLARE3(stateMachine, entryName, eventData) \
    void EN_##entryName(const eventData*); \
    EntryAction<stateMachine, eventData, &stateMachine::EN_##entryName> entryName;
#define ENTRY_DEFINE2(stateMachine, entryName) \
    void stateMachine::EN_##entryName(const NoEventData*)
#define ENTRY_DEFINE3(stateMachine, entryName, eventData) \
    void stateMachine::EN_##entryName(const eventData* data)

#define EXIT_DECLARE2(stateMachine, exitName) \
    void EX_##exitName(void); \
    ExitAction<stateMachine, &stateMachine::EX_##exitName> exitName;
#define EXIT_DEFINE2(stateMachine, exitName) \
    void stateMachine::EX_##exitName(void)


/**
 * Transition map definition macros, used to
 * define the valid transitions between states.
 * Upon ending the transition map, the current
 * state is called and executed.
 *
 * A static assertion will fail if the number
 * of transitions defined does not equal the
 * number of states.
 */
#define BEGIN_TRANSITION_MAP \
    static const state_type TRANSITIONS[] = {
#define TRANSITION_MAP_ENTRY(entry) \
        entry,
#define END_TRANSITION_MAP(data) \
    }; \
    ASSERT_TRUE(getCurrentState() < ST_MAX_STATES); \
    externalEvent(TRANSITIONS[getCurrentState()], data); \
    static_assert((sizeof(TRANSITIONS) / sizeof(state_type)) == ST_MAX_STATES, "Invalid number of transitions");


/**
 * Transition in the parent StateMachine class,
 * used to trigger that transition
 */
#define PARENT_TRANSITION(state) \
    if (getCurrentState() >= ST_MAX_STATES && getCurrentState() < getMaxStates()) { \
        externalEvent(state); \
        return; \
    }


/**
 * State map macros, used to define a state
 * map with its entries.
 *
 * A static assertion occurs if the number of
 * states in the state map does not equal the
 * number of defined states.
 */
#define BEGIN_STATE_MAP \
    private:\
        virtual const StateMapRowEx* getStateMapEx() { return nullptr; } \
        virtual const StateMapRow* getStateMap() { \
            static const StateMapRow STATE_MAP[] = {
#define STATE_MAP_ENTRY(stateName)\
                { stateName },
#define END_STATE_MAP \
            }; \
            static_assert((sizeof(STATE_MAP) / sizeof(StateMapRow)) == ST_MAX_STATES, "Invalid state map size"); \
            return &STATE_MAP[0]; \
        }


/**
 * Extended state map macros, used to support entries,
 * exits, and guards in the state map
 *
 * A static assertion occurs if the number of
 * states in the state map does not equal the
 * number of defined states.
 */
#define BEGIN_STATE_MAP_EX \
    private: \
        virtual const StateMapRow* getStateMap() { return nullptr; } \
        virtual const StateMapRowEx* getStateMapEx() { \
            static const StateMapRowEx STATE_MAP[] = {
#define STATE_MAP_ENTRY_EX(stateName) \
                { stateName, 0, 0, 0 },
#define STATE_MAP_ENTRY_ALL_EX(stateName, guardName, entryName, exitName) \
                { stateName, guardName, entryName, exitName },
#define END_STATE_MAP_EX \
            }; \
            static_assert((sizeof(STATE_MAP) / sizeof(StateMapRowEx)) == ST_MAX_STATES, "Invalid state map size"); \
            return &STATE_MAP[0]; \
        }

#endif // STATEMACHINE_H
