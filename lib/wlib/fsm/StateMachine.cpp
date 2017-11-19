#include "StateMachine.h"

namespace wlp {

    StateMachine::StateMachine(state_type maxStates, state_type initialState) :
            m_max_states(maxStates),
            m_current_state(initialState),
            m_new_state(0),
            m_event_generated(false),
            m_event_data(nullptr) {
        /* assert(m_max_states < EVENT_IGNORED) */
    }

    void StateMachine::externalEvent(state_type newState, const EventData *pData) {
        // If we are supposed to ignore this event
        if (newState == EVENT_IGNORED || newState == CANNOT_HAPPEN) {
            // Just delete the event data, if any
            if (pData != nullptr) {
                delete pData;
            }
        } else {
            // TODO - capture software lock here for thread-safety if necessary

            // Generate the event
            internalEvent(newState, pData);
            // Execute the state engine
            // Function returns only when all events are processed
            stateEngine();

            // TODO - release software lock here
        }
    }

    void StateMachine::internalEvent(state_type newState, const EventData *pData) {
        if (pData == nullptr) {
            pData = new NoEventData();
        }
        m_event_data = pData;
        m_event_generated = true;
        m_new_state = newState;
    }

    void StateMachine::stateEngine(void) {
        const StateMapRow *pStateMap = getStateMap();
        if (pStateMap != nullptr) {
            stateEngine(pStateMap);
        } else {
            const StateMapRowEx *pStateMapEx = getStateMapEx();
            if (pStateMapEx != nullptr) {
                stateEngine(pStateMapEx);
            } else {
                /* fail() */
            }
        }
    }

    void StateMachine::stateEngine(const StateMapRow *const pStateMap) {
        const EventData *pDataTemp = nullptr;
        // While events are being generated keep executing states
        while (m_event_generated) {
            /* assert(m_new_state < m_max_states) */
            if (m_new_state >= m_max_states) {
                return;
            }
            // Get the pointer from the state map
            const StateBase *state = pStateMap[m_new_state].State;
            // Copy of event data pointer
            pDataTemp = m_event_data;
            // Event data used up, reset the pointer
            m_event_data = nullptr;
            // Event used up, reset the flag
            m_event_generated = false;
            // Switch to the new current state
            setCurrentState(m_new_state);
            // Execute the state action passing in event data
            /* assert(state != nullptr) */
            if (state == nullptr) {
                return;
            }
            state->InvokeStateAction(this, pDataTemp);
            if (pDataTemp) {
                delete pDataTemp;
            }
        }
    }

    void StateMachine::stateEngine(const StateMapRowEx *const pStateMapEx) {
        const EventData *pDataTemp = nullptr;
        // While events are being generated keep executing states
        while (m_event_generated) {
            /* assert(m_new_state < m_max_states) */
            if (m_new_state >= m_max_states) {
                return;
            }
            // Get the pointers from the state map
            const StateBase *state = pStateMapEx[m_new_state].State;
            const GuardBase *guard = pStateMapEx[m_new_state].Guard;
            const EntryBase *entry = pStateMapEx[m_new_state].Entry;
            const ExitBase *exit = pStateMapEx[m_current_state].Exit;
            // Copy of event data pointer
            pDataTemp = m_event_data;
            // Event data used up, reset the pointer
            m_event_data = nullptr;
            // Event used up, reset the flag
            m_event_generated = false;
            // Execute the guard condition
            bool guardResult = true;
            if (guard != nullptr) {
                guardResult = guard->InvokeGuardCondition(this, pDataTemp);
            }
            // If the guard condition succeeds
            if (guardResult == true) {
                // Transitioning to a new state?
                if (m_new_state != m_current_state) {
                    // Execute the state exit action on current state before switching to new state
                    if (exit != nullptr) {
                        exit->InvokeExitAction(this);
                    }
                    // Execute the state entry action on the new state
                    if (entry != nullptr) {
                        entry->InvokeEntryAction(this, pDataTemp);
                    }
                    // Ensure exit/entry actions didn't call InternalEvent by accident
                    /* assert(m_event_generated == false) */
                    if (m_event_generated) {
                        return;
                    }
                }
                // Switch to the new current state
                setCurrentState(m_new_state);
                // Execute the state action passing in event data
                /* assert(state != nullptr) */
                if (state == nullptr) {
                    return;
                }
                state->InvokeStateAction(this, pDataTemp);
            }
            if (pDataTemp) {
                delete pDataTemp;
            }
        }
    }


}
