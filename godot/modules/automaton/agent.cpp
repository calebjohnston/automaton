/* agent.cpp */

#include "agent.h"

bool Agent::say_text(String p_txt) {

    //convert Godot String to Godot CharString to C string
    print_line(p_txt);
    // return festival_say_text(p_txt.ascii().get_data());
    return true;
}

String Agent::getPort() const
{
    return String(localPort.getAddress().c_str());
}

void Agent::_bind_methods() {
    ClassDB::bind_method(D_METHOD("say_text", "txt"), &Agent::say_text);
    ClassDB::bind_method(D_METHOD("getter"), &Agent::getter);
    ClassDB::bind_method(D_METHOD("getPort"), &Agent::getPort);
}

Agent::Agent() {

}
