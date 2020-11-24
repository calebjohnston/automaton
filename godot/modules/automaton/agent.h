/* agent.h */

#ifndef GODOT_AGENT_H
#define GODOT_AGENT_H

#include "core/reference.h"

#include "Port.h"

class Agent : public Reference {
    GDCLASS(Agent, Reference);

    Port localPort;

protected:
    static void _bind_methods();

public:
    bool say_text(String p_txt);

    int getter() const { return 15; }

    String getPort() const;

    Agent();
};

#endif // GODOT_AGENT_H
