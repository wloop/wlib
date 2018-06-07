#ifndef state_machine_H
#define state_machine_H

#include <wlib/Wlib.h>
#include <wlib/Types.h>
#include <wlib/mem/Memory.h>
#include <wlib/util/Tmp.h>
#include <wlib/exceptions/Exceptions.h>

namespace wlp {

    /**
     * Unique state machine event data must
     * inherit from this class.
     */
    class sm_event_data {
    public:
        virtual ~sm_event_data() {}
    };

    typedef uint8_t state_type;
    typedef sm_event_data sm_no_data;

    class state_machine;

    /**
     * Abstract state base class that all
     * states inherit from.
     */
    class state_base {
    public:
        virtual ~state_base() {}

        /**
         * Called by the state machine engine to execute a
         * state action. If a guard condition exists and it
         * evaluates to false, the state action will not
         * execute.
         *
         * @param sm a state machine instance
         * @param data the event data
         */
        virtual void invoke_state_action(state_machine *sm, const sm_event_data *data) const = 0;
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
    class state_action : public state_base {
    public:
        virtual ~state_action() {}

        /**
         * @see state_base::invoke_state_action
         */
        virtual void invoke_state_action(state_machine *sm, const sm_event_data *data) const {
            // Downcast the state machine and event data to the correct derived type
            SM *derivedSM = static_cast<SM *>(sm);
            /*
             * If the derived data dynamic cast returns a null pointer, then
             * there is a mismatch between the event type in STATE_DECLARE and
             * the event type passed to the function.
             */
#ifndef __cpp_rtti
            const Data *derivedData = reinterpret_cast<const Data *>(data);
#else
            const Data *derivedData = dynamic_cast<const Data *>(data);
#endif
            /* assert(derivedData != nullptr) */
            if (derivedData == nullptr) {
                THROW(BAD_STATE_EXCEPTION("Invalid sm_event_data type passed to state"))
            }
            // Call the state function
            (derivedSM->*Func)(derivedData);
        }
    };

    /**
     * Abstract guard base class that all
     * guards classes inherit from.
     */
    class guard_base {
    public:
        virtual ~guard_base() {}

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
        virtual bool invoke_guard_action(state_machine *sm, const sm_event_data *data) const = 0;
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
    class guard_condition : public guard_base {
    public:
        virtual ~guard_condition() {}

        virtual bool invoke_guard_action(state_machine *sm, const sm_event_data *data) const {
            SM *derivedSM = static_cast<SM *>(sm);
#ifndef __cpp_rtti
            const Data *derivedData = reinterpret_cast<const Data *>(data);
#else
            const Data *derivedData = dynamic_cast<const Data *>(data);
#endif
            /* assert(derivedData != nullptr) */
            if (derivedData == nullptr) {
                THROW(BAD_STATE_EXCEPTION("Invalid sm_event_data type passed to guard"))
            }
            // Call the guard function
            return (derivedSM->*Func)(derivedData);
        }
    };

    /**
     * Abstract entry base class that all
     * entry classes inherit from.
     */
    class entry_base {
    public:
        virtual ~entry_base() {}

        /**
         * Called by the state machine engine to execute
         * a state entry action. Called when entering a state.
         *
         * @param sm a state machine instance
         * @param data the event data
         */
        virtual void invoke_entry_action(state_machine *sm, const sm_event_data *data) const = 0;
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
    class entry_action : public entry_base {
    public:
        virtual ~entry_action() {}

        virtual void invoke_entry_action(state_machine *sm, const sm_event_data *data) const {
            SM *derivedSM = static_cast<SM *>(sm);
#ifndef __cpp_rtti
            const Data *derivedData = reinterpret_cast<const Data *>(data);
#else
            const Data *derivedData = dynamic_cast<const Data *>(data);
#endif
            /* assert(derivedData != nullptr) */
            if (derivedData == nullptr) {
                THROW(BAD_STATE_EXCEPTION("Invalid sm_event_data type passed to entry"))
            }
            // Call the entry function
            (derivedSM->*Func)(derivedData);
        }
    };

    /**
     * Abstract exit base class that all
     * exit classes inherit from.
     */
    class exit_base {
    public:
        virtual ~exit_base() {}

        /**
         * Called by the state machine engine to exeucte a
         * state exit action. Called when leaving a state.
         *
         * @param sm a state machine instance
         */
        virtual void invoke_exit_action(state_machine *sm) const = 0;
    };

    /**
     * This class represents an action that is executed
     * upon exiting a state.
     *
     * @tparam SM state machine class
     * @tparam Func state machine member function pointer
     */
    template<class SM, void (SM::*Func)(void)>
    class exit_action : public exit_base {
    public:
        virtual ~exit_action() {}

        virtual void invoke_exit_action(state_machine *sm) const {
            SM *derivedSM = static_cast<SM *>(sm);

            // Call the exit function
            (derivedSM->*Func)();
        }
    };

    /**
     * A structure to hold a single row within the state map.
     */
    struct state_map_row {
        const state_base *const state;
    };

    /**
     * A structure to hold a single row within the extended state map.
     */
    struct state_map_row_ex {
        const state_base *const state;
        const guard_base *const guard;
        const entry_base *const entry;
        const exit_base *const exit;
    };

    /**
     * state_machine implements a software-based state machine.
     */
    class state_machine {
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
        state_machine(state_type maxStates, state_type initialState = 0) :
            m_event_data(nullptr),
            m_dataless(),
            m_event_generated(false),
            m_max_states(maxStates),
            m_current_state(initialState),
            m_new_state(0) {
            /* assert(m_max_states < EVENT_IGNORED) */
            if (m_max_states >= EVENT_IGNORED) {
                THROW(BAD_STATE_EXCEPTION("Max states cannot equal or exceed EVENT_IGNORED"))
            }
        }

        virtual ~state_machine() {}

        /**
         * @return the current state machine state
         * as an ordinal value
         */
        state_type current_state() { return m_current_state; }

        /**
         * @return the maximum ordinal value that
         * a state may have
         */
        state_type max_states() { return m_max_states; }

    protected:
        /**
         * Trigger an external state machine event.
         *
         * @param newState the state machine state to
         * which the machine should transition
         * @param pData the event data to send
         * to the new state
         */
        template<typename sm_event_dataType>
        void external_event(state_type newState, sm_event_dataType *pData = nullptr) {
            if (newState == CANNOT_HAPPEN) {
                THROW(ILLEGAL_TRANSITION_EXCEPTION("Transition cannot occur"))
            }
            if (newState != EVENT_IGNORED) {
                internal_event<sm_event_dataType>(newState, pData);
                state_engine<sm_event_dataType>();
            }
        }

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
        template<typename sm_event_dataType>
        void internal_event(state_type newState, sm_event_dataType *pData = nullptr) {
            if (pData == nullptr) {
                pData = static_cast<sm_event_dataType *>(&m_dataless);
            }
            m_event_data = pData;
            m_event_generated = true;
            m_new_state = newState;
        }

    private:
        /**
         * The state event data pointer.
         */
        sm_event_data *m_event_data;

        /**
         * Class-level event data instance used to
         * represent an event with no data.
         */
        sm_event_data m_dataless;

        /**
         * This value is set to true when an event
         * is generated.
         */
        bool m_event_generated;

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
         * Get the state map as defined in a derived class of State Machine.
         * The @code BEGIN_STATE_MAP @endcode, @code STATE_MAP_ENTRY @endcode, and
         * @code END_STATE_MAP @endcode macros are used to assist in constructing
         * the map. Derived classes only need to implement one of @code state_map @endcode
         * and @code state_map_ex @endcode.
         *
         * @return an array of state map rows with an array size of
         * @code MAX_STATES @endcode or nullptr if the state machine
         * uses @code state_map_ex @endcode
         */
        virtual const state_map_row *state_map() = 0;

        /**
         * Get the extended state map as defined in the derived class of State Machine.
         * The @code BEGIN_STATE_MAP_EX @endcode, @code STATE_MAP_ENTRY_EX @endcode,
         * @code STATE_MAP_ENTRY_ALL_EX @endcode, and @code END_STATE_MAP_EX @endcode are
         * used to assist in constructing the map.
         *
         * @return an array of extended state map rows of size @code MAX_STATES @endcode
         * or nullptr if the state machine uses @code state_map @endcode
         */
        virtual const state_map_row_ex *state_map_ex() = 0;

        /**
         * Set a new current state.
         *
         * @param newState the new state of the machine
         */
        void set_current_state(state_type newState) { m_current_state = newState; }

        /**
         * The state machine engine that executes the external events and, optionally,
         * all internal events generated during state execution.
         *
         * This function will pull the state map from the state
         * map definition. If both the state map and the extended
         * state map are undefined, no execution occurs.
         */
        template<typename sm_event_dataType>
        void state_engine() {
            const state_map_row *pStateMap = state_map();
            if (pStateMap != nullptr) {
                state_engine<sm_event_dataType>(pStateMap);
            } else {
                const state_map_row_ex *pStateMapEx = state_map_ex();
                if (pStateMapEx != nullptr) {
                    state_engine<sm_event_dataType>(pStateMapEx);
                } else {
                    /* fail() */
                    THROW(BAD_STATE_EXCEPTION("Unable to find state map"))
                }
            }
        }

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
        template<typename sm_event_dataType>
        void state_engine(const state_map_row *const pStateMap);

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
        template<typename sm_event_dataType>
        void state_engine(const state_map_row_ex *const pStateMapEx);
    };

    template<typename sm_event_dataType>
    void state_machine::state_engine(const state_map_row *const pStateMap) {
        sm_event_dataType *pDataTemp = nullptr;
        while (m_event_generated) {
            /* assert(m_new_state < m_max_states) */
            if (m_new_state >= m_max_states) {
                THROW(UNEXPECTED_STATE_EXCEPTION("New state ordinal exceeds maximum states"))
            }
            const state_base *state = pStateMap[m_new_state].state;
            pDataTemp = static_cast<sm_event_dataType *>(m_event_data);
            m_event_data = nullptr;
            m_event_generated = false;
            set_current_state(m_new_state);
            /* assert(state != nullptr) */
            if (state == nullptr) {
                THROW(UNEXPECTED_STATE_EXCEPTION("Target state does not exist"))
            }
            state->invoke_state_action(this, pDataTemp);
        }
    }

    template<typename sm_event_dataType>
    void state_machine::state_engine(const state_map_row_ex *const pStateMapEx) {
        sm_event_dataType *pDataTemp = nullptr;
        while (m_event_generated) {
            /* assert(m_new_state < m_max_states) */
            if (m_new_state >= m_max_states) {
                THROW(UNEXPECTED_STATE_EXCEPTION("New state ordinal exceeds maximum states"))
            }
            const state_base *state = pStateMapEx[m_new_state].state;
            const guard_base *guard = pStateMapEx[m_new_state].guard;
            const entry_base *entry = pStateMapEx[m_new_state].entry;
            const exit_base *exit = pStateMapEx[m_current_state].exit;
            pDataTemp = static_cast<sm_event_dataType *>(m_event_data);
            m_event_data = nullptr;
            m_event_generated = false;
            bool guardResult = true;
            if (guard != nullptr) {
                guardResult = guard->invoke_guard_action(this, pDataTemp);
            }
            if (pDataTemp == nullptr) {
                return;
            }
            if (guardResult) {
                if (m_new_state != m_current_state) {
                    if (exit != nullptr) {
                        exit->invoke_exit_action(this);
                    }
                    if (entry != nullptr) {
                        entry->invoke_entry_action(this, pDataTemp);
                    }
                    /* assert(m_event_generated == false) */
                    if (m_event_generated) {
                        THROW(UNEXPECTED_STATE_EXCEPTION("Entry or Exit action cannot generate events"))
                    }
                }
                set_current_state(m_new_state);
                /* assert(state != nullptr) */
                if (state == nullptr) {
                    THROW(UNEXPECTED_STATE_EXCEPTION("Target state does not exist"))
                }
                state->invoke_state_action(this, pDataTemp);
            }
        }
    }

}


/**
 * Macros used to declare and define state machine
 * states, guards, entries, and exits.
 */
#define STATE_DECLARE(...) VFUNC(STATE_DECLARE, __VA_ARGS__)
#define STATE_DEFINE(...) VFUNC(STATE_DEFINE, __VA_ARGS__)
#define STATE_DECLARE2(state_machine, stateName) \
    void ST_##stateName(const sm_no_data*); \
    state_action<state_machine, sm_no_data, &state_machine::ST_##stateName> stateName;
#define STATE_DECLARE3(state_machine, stateName, sm_event_data) \
    void ST_##stateName(const sm_event_data*); \
    state_action<state_machine, sm_event_data, &state_machine::ST_##stateName> stateName;
#define STATE_DEFINE2(state_machine, stateName) \
    void state_machine::ST_##stateName(const sm_no_data*)
#define STATE_DEFINE3(state_machine, stateName, sm_event_data) \
    void state_machine::ST_##stateName(const sm_event_data* data)

#define GUARD_DECLARE(...) VFUNC(GUARD_DECLARE, __VA_ARGS__)
#define GUARD_DEFINE(...) VFUNC(GUARD_DEFINE, __VA_ARGS__)
#define GUARD_DECLARE2(state_machine, guardName) \
    bool GD_##guardName(const sm_no_data*); \
    guard_condition<state_machine, sm_no_data, &state_machine::GD_##guardName> guardName;
#define GUARD_DECLARE3(state_machine, guardName, sm_event_data) \
    bool GD_##guardName(const sm_event_data*); \
    guard_condition<state_machine, sm_event_data, &state_machine::GD_##guardName> guardName;
#define GUARD_DEFINE2(state_machine, guardName) \
    bool state_machine::GD_##guardName(const sm_no_data*)
#define GUARD_DEFINE3(state_machine, guardName, sm_event_data) \
    bool state_machine::GD_##guardName(const sm_event_data* data)

#define ENTRY_DELCARE(...) VFUNC(ENTRY_DELCARE, __VA_ARGS__)
#define ENTRY_DEFINE(...) VFUNC(ENTRY_DEFINE, __VA_ARGS__)
#define ENTRY_DECLARE2(state_machine, entryName) \
    void EN_##entryName(const sm_no_data*); \
    entry_action<state_machine, sm_no_data, &state_machine::EN_##entryName> entryName;
#define ENTRY_DECLARE3(state_machine, entryName, sm_event_data) \
    void EN_##entryName(const sm_event_data*); \
    entry_action<state_machine, sm_event_data, &state_machine::EN_##entryName> entryName;
#define ENTRY_DEFINE2(state_machine, entryName) \
    void state_machine::EN_##entryName(const sm_no_data*)
#define ENTRY_DEFINE3(state_machine, entryName, sm_event_data) \
    void state_machine::EN_##entryName(const sm_event_data* data)

#define EXIT_DECLARE2(state_machine, exitName) \
    void EX_##exitName(void); \
    exit_action<state_machine, &state_machine::EX_##exitName> exitName;
#define EXIT_DEFINE2(state_machine, exitName) \
    void state_machine::EX_##exitName(void)


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
#define END_TRANSITION_MAP(data, dataType) \
    }; \
    ASSERT_TRUE(current_state() < ST_MAX_STATES); \
    external_event<dataType>(TRANSITIONS[current_state()], data); \
    static_assert((sizeof(TRANSITIONS) / sizeof(state_type)) == ST_MAX_STATES, "Invalid number of transitions");


/**
 * Transition in the parent state_machine class,
 * used to trigger that transition
 */
#define PARENT_TRANSITION(state) \
    if (current_state() >= ST_MAX_STATES && current_state() < max_states()) { \
        external_event(state); \
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
        virtual const state_map_row_ex* state_map_ex() { return nullptr; } \
        virtual const state_map_row* state_map() { \
            static const state_map_row STATE_MAP[] = {
#define STATE_MAP_ENTRY(stateName)\
                { stateName },
#define END_STATE_MAP \
            }; \
            static_assert((sizeof(STATE_MAP) / sizeof(state_map_row)) == ST_MAX_STATES, "Invalid state map size"); \
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
        virtual const state_map_row* state_map() { return nullptr; } \
        virtual const state_map_row_ex* state_map_ex() { \
            static const state_map_row_ex STATE_MAP[] = {
#define STATE_MAP_ENTRY_EX(stateName) \
                { stateName, 0, 0, 0 },
#define STATE_MAP_ENTRY_ALL_EX(stateName, guardName, entryName, exitName) \
                { stateName, guardName, entryName, exitName },
#define END_STATE_MAP_EX \
            }; \
            static_assert((sizeof(STATE_MAP) / sizeof(state_map_row_ex)) == ST_MAX_STATES, "Invalid state map size"); \
            return &STATE_MAP[0]; \
        }

#endif // state_machine_H
