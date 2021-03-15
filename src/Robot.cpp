//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#include "Robot.h"

Robot::Robot(int id, int midiChannel, const String &host, int port, const String &name) : m_node(String(id)) {
    m_node.setProperty(Id, id, nullptr);
    m_node.setProperty(MidiChannel, midiChannel, nullptr);
    m_node.setProperty(Host, host, nullptr);
    m_node.setProperty(Port, port, nullptr);
    m_node.setProperty(Name, name, nullptr);
    m_node.setProperty(ConnectionStatus, false, nullptr);
    m_node.setProperty(Enabled, false, nullptr);
}

Robot::~Robot() {

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

}

void Robot::toggleConnection() {
    if (m_node[ConnectionStatus])
        disconnect();
    else
        connect();
}

/******************************************* Robots *******************************************/
Robots::Robots() {
}

Robots::~Robots() {

}

void Robots::addRobots() {
    for (int id=0; id<kRobotList.size(); ++id) {
        auto robot = std::make_shared<Robot>(id, 0, DEFAULT_HOST, DEFAULT_PORT, kRobotList[id]);
        robot->disconnect();
        rootData.addChild(robot->m_node, id, nullptr);
        m_robots[(size_t)id] = robot;
    }
}

void Robots::removeRobots() {
    for (int id=0; id<kRobotList.size(); ++id) {
        rootData.removeChild(id, nullptr);
        auto robot = m_robots[(size_t)id];
        robot.reset();
    }
}
