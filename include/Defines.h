//
// Created by Raghavasimhan Sankaranarayanan on 3/12/21.
//

#ifndef MIDI_OSC_PLUGIN_DEFINES_H
#define MIDI_OSC_PLUGIN_DEFINES_H

#include "JuceHeader.h"

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_PORT 7001
#define MAX_ROBOTS 5
#define MAX_MIDI_CHANNELS 17

struct MainProcessorInfo {
    static inline AudioProcessor* ptr = nullptr;
    static inline AudioProcessor::TrackProperties properties {};
};

static size_t ulNumRobots = MAX_ROBOTS;

const int iComponentWidth = 440;
const int iCompHeight = 100;

const juce::StringArray kRobotList {"Shimon", "GuitarBot", "xArm", "SanturBot", "Stretch"};

namespace Identifiers {
    const juce::Identifier Name {"name"};
    const juce::Identifier Id {"id"};
    const juce::Identifier MidiChannel {"midiChannel"};
    const juce::Identifier Host {"host"};
    const juce::Identifier Port {"port"};
    const juce::Identifier ConnectionStatus {"connectionStatus"};
    const juce::Identifier Enabled {"enabled"};
}

namespace AddrPattern {
    const String noteOn = "/noteon";
    const String noteOff = "/noteoff";
    const String velocity = "/velocity";
    const String pressure = "/pressure";
    const String touch = "/touch"; // Aftertouch
}

#endif //MIDI_OSC_PLUGIN_DEFINES_H