//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#ifndef MIDI_OSC_ROBOT_H
#define MIDI_OSC_ROBOT_H

#include "JuceHeader.h"
#include "juce_osc/juce_osc.h"

#include "Defines.h"

using namespace  Identifiers;

class Robot: public ValueTree::Listener {
public:
    Robot(int id, int midiChannel, const String& host = DEFAULT_HOST, int port = DEFAULT_PORT, const String &name = INVALID_NAME);
    ~Robot() override;

    [[nodiscard]] bool isEnabled() const {
        return m_node[Enabled];
    }
    void setEnabled(bool enable) {
        m_node.setProperty(Enabled, enable, nullptr);
    }

    int connect();
    int disconnect();
    [[nodiscard]] bool isConnected() const {
        return m_node[ConnectionStatus];
    }

    [[nodiscard]] int getMidiChannel() const {
        return m_node[MidiChannel];
    }
    void setMidiChannel(int iMidiChannel) {
        m_node.setProperty(MidiChannel, iMidiChannel, nullptr);
    }

    [[nodiscard]] int getId() const {
        return m_node[Id];
    }
    void setId(int id) {
        m_node.setProperty(Id, id, nullptr);
    }

    [[nodiscard]] String getHost() const {
        return m_node[Host];
    }
    void setHost(const String& host) {
        m_node.setProperty(Host, host, nullptr);
    }

    [[nodiscard]] int getPort() const {
        return m_node[Port];
    }
    void setPort(int port) {
        m_node.setProperty(Port, port, nullptr);
    }

    void send(const MidiMessage &msg);
    void toggleConnection();

public:
    ValueTree m_node;

private:
    OSCSender m_sender;

};

class Robots: public ValueTree::Listener {
public:
    Robots();
    ~Robots();

    [[nodiscard]] size_t getNumRobots() const { return (size_t) rootData.getNumChildren(); }

    void addRobots();
    void removeRobots();

    std::shared_ptr<Robot> at(size_t id) { return m_robots[id]; }

    int getAvailableChannel();

public:
    ValueTree rootData {"Robots"};
    std::array<bool, MAX_MIDI_CHANNELS> enabledChannels {};

private:
    std::array<std::shared_ptr<Robot>, MAX_ROBOTS> m_robots;
    static inline std::array<int, MAX_ROBOTS> m_chMap;
};
#endif //MIDI_OSC_ROBOT_H
