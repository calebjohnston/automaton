/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "agent.h"
#include "monitor.h"

void register_automaton_types() {
    ClassDB::register_class<Agent>();
    ClassDB::register_class<Monitor>();

}

void unregister_automaton_types() {
    // Nothing to do here in this example.
}
