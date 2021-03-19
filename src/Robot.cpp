//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#include "Robot.h"

Robot::Robot(int id, ValueTree data) : m_node(data) {
    setName(data.getType().toString());
    setId(id);
    setHost(DEFAULT_HOST);
    setPort(DEFAULT_PORT);
    setEnabled(false);
    m_node.setProperty(ConnectionStatus, false, nullptr);
    disconnect();
}

Robot::~Robot() {
    disconnect();
}

int Robot::connect() {
    if (!m_node[ConnectionStatus]) {
        if (m_sender.connect(m_node[Host], m_node[Port])) {
            DBG("Connection Success!");
            m_node.setProperty(ConnectionStatus, true, nullptr);
            return 0;
        }
        DBG("Connection Failed");
        return 1;
    }

    return 0;
}

int Robot::disconnect() {
    if (m_node[ConnectionStatus]) {
        if (m_sender.disconnect()) {
            DBG("Disconnection Success!");
            m_node.setProperty(ConnectionStatus, false, nullptr);
            return 0;
        }
        DBG("Disconnection Failed!");
        return 1;
    }
    return 0;
}

void Robot::send(const MidiMessage &msg) {
    if (!isEnabled() || !isConnected())
        return;

    using namespace AddrPattern;
    if (msg.getChannel() != (int)m_node[MidiChannel]) {
        DBG("Warning: Midi Channel of msg is different from the robot's channel");
        return;
    }

    if (msg.isNoteOn()) {
        m_sender.send(noteOn, msg.getNoteNumber());
        m_sender.send(velocity, msg.getVelocity());
        DBG("Note: " << msg.getDescription());
    } else if (msg.isNoteOff()) {
        m_sender.send(noteOff, msg.getNoteNumber());
    } else if (msg.isChannelPressure()) {
        m_sender.send(pressure, msg.getChannelPressureValue());
    } else if (msg.isAftertouch()) {
        m_sender.send(touch, msg.getAfterTouchValue());
    } else {
        DBG("Unknown msg: " << msg.getDescription());
    }
}

void Robot::toggleConnection() {
    if (m_node[ConnectionStatus])
        disconnect();
    else
        connect();
}

/******************************************* Robots *******************************************/
Robots::Robots(ValueTree data) : m_rootData(data) {
}

Robots::~Robots() {
}

void Robots::addRobots() {
    for (size_t id = 0; id < MAX_ROBOTS; ++id) {
        auto child = m_rootData.getChild((int)id);
        addRobot((int)id, child);
    }
}

void Robots::removeRobots() {
    for (size_t id=0; id<MAX_ROBOTS; ++id) {
        removeRobot(id);
    }
}

void Robots::addRobot(int id, ValueTree node) {
    auto robot = std::make_shared<Robot>(id, node);
    m_robots[(size_t)id] = robot;
}

void Robots::removeRobot(size_t id) {
    m_robots[id]->disconnect();
    m_robots[id] = nullptr;
}

void Robots::setData(ValueTree data) {
    m_rootData = data;
    for (size_t id=0; id<MAX_ROBOTS; ++id) {
        m_robots[id]->setData(data.getChild((int)id));
    }
}

void Robots::disconnectAll() {
    for (const auto& robot: m_robots) {
        robot->disconnect();
    }
}

void Robots::send(const MidiMessage &msg) {
    auto ch = (size_t)msg.getChannel();
    auto id = m_chMap[ch];
    if (id < ulNumRobots) {
        auto robot = m_robots[id];
        if (robot->isEnabled()) {
            robot->send(msg);
        }
    }
}
