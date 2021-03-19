//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//
#pragma once

#include "PluginProcessor.h"
#include "Robot.h"
#include "RobotComponent.h"
#include "Defines.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor, public ValueTree::Listener
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&, Robots&, ValueTree&, bool isDummy = false);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void setMainProcessorTrackName(const String& name) { m_mainProcessorTrackName = name; DBG(m_mainProcessorTrackName); }

    void updateChannelComboBox();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& m_processor;
    ValueTree& m_data;
    Robots& m_robots;
    std::array<std::unique_ptr<RobotComponent>, MAX_ROBOTS> m_robotUi;

    std::atomic<bool> m_bIsDummy;
    String m_mainProcessorTrackName = "";

    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property) override;
    void valueTreeRedirected (ValueTree& treeWhichHasBeenChanged) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};