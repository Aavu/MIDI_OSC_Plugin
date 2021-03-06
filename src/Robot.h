//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#ifndef MIDI_OSC_ROBOT_H
#define MIDI_OSC_ROBOT_H

#include "JuceHeader.h"
#include "juce_osc/juce_osc.h"

#include "Defines.h"

using namespace  Identifiers;

class Robot {
public:
    explicit Robot(int id, ValueTree data);
    ~Robot();

    void setData(ValueTree data) { m_node = data; }
    ValueTree getNode() { return m_node; }

    void setName(const String& name) { m_node.setProperty(Name, name, nullptr); }
    [[nodiscard]] String getName() const { return m_node[Name]; }

    [[nodiscard]] bool isEnabled() const { return m_node[Enabled]; }
    void setEnabled(bool enable) { m_node.setProperty(Enabled, enable, nullptr); }

    int connect();
    int disconnect();
    [[nodiscard]] bool isConnected() const { return m_node[ConnectionStatus]; }

    [[nodiscard]] int getMidiChannel() const { return m_node[MidiChannel]; }
    void setMidiChannel(int iMidiChannel) { m_node.setProperty(MidiChannel, iMidiChannel, nullptr); }

    [[nodiscard]] int getId() const { return m_node[Id]; }
    void setId(int id) { m_node.setProperty(Id, id, nullptr); }

    [[nodiscard]] String getHost() const { return m_node[Host]; }
    void setHost(const String& host) { m_node.setProperty(Host, host, nullptr); }

    [[nodiscard]] int getPort() const { return m_node[Port]; }
    void setPort(int port) { m_node.setProperty(Port, port, nullptr); }

    void send(const MidiMessage &msg);
    void toggleConnection();

    friend std::ostream& operator<<(std::ostream& os, const Robot& r) {
        os  << "Id: " << r.getId()
            << "\t Port: " << r.getPort()
            << "\t Host: " << r.getHost()
            << "\t Ch: " << r.getMidiChannel()
            << "\t connected: " << r.isConnected()
            << "\t enabled: " << r.isEnabled()
            << "\t Name: " << r.getName();
        return os;
    }

    friend String& operator<<(String& os, const Robot& r) {
        os  << "Id: " << r.getId()
            << "\t Port: " << r.getPort()
            << "\t Host: " << r.getHost()
            << "\t Ch: " << r.getMidiChannel()
            << "\t connected: " << (int)r.isConnected()
            << "\t enabled: " << (int)r.isEnabled()
            << "\t Name: " << r.getName();
        return os;
    }

private:
    ValueTree m_node;
    OSCSender m_sender;

};

class Robots {
public:
    using iterator = std::array<std::shared_ptr<Robot>, MAX_ROBOTS>::iterator;
    explicit Robots(ValueTree data);
    ~Robots();

    void addRobots();
    void removeRobots();

    void setData(ValueTree data);

    void addRobot(int id, ValueTree node);
    void removeRobot(size_t id);

    void disconnectAll();

    void send(const MidiMessage& msg);

    std::shared_ptr<Robot>operator[](size_t id) { return m_robots[id]; }

    iterator begin() {
        return m_robots.begin();
    }

    iterator end() {
        return m_robots.end();
    }

    [[nodiscard]] size_t size() const {
        return m_robots.size();
    }

    friend std::ostream& operator<<(std::ostream& os, Robots& r) {
        for (const auto& i : r) {
            os << *i << std::endl;
        }
        return os;
    }

    friend String& operator<<(String& os, Robots& r) {
        for (const auto& i : r) {
            os << *i << "\n";
        }
        return os;
    }

private:
    ValueTree m_rootData;
    std::array<std::shared_ptr<Robot>, MAX_ROBOTS> m_robots;
    static inline std::array<size_t, MAX_ROBOTS> m_chMap;
};
#endif //MIDI_OSC_ROBOT_H
