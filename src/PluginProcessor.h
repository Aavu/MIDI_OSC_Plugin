//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Defines.h"
#include "Robot.h"

//==============================================================================
class AudioPluginAudioProcessor  : public juce::AudioProcessor, public juce::MidiInputCallback
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    const std::array<bool, MAX_MIDI_CHANNELS>& getEnabledChannels() const { return m_enabledChannels; }
    void updateChannelStatus();
    void enableChannel(int ch, bool enable);
    int getAvailableChannel();

private:
    //==============================================================================
    ValueTree m_data;
    Robots m_robots;
    std::array<bool, MAX_MIDI_CHANNELS> m_enabledChannels;

    std::unique_ptr<MidiInput> m_midiInput;

    void initData();
    void handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) override;

    std::atomic<bool> m_bDummyUi;
    std::atomic<bool> m_bCanUseStateInfo;

    void updateTrackProperties (const TrackProperties& properties) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
