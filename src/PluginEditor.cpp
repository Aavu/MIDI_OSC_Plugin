//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p, Robots& robots, const ValueTree& data)
        : AudioProcessorEditor (&p), m_processor (p), m_data(data), m_robots(robots)
{
    setSize(iComponentWidth, iCompHeight * MAX_ROBOTS);
    for (size_t id=0; id<MAX_ROBOTS; ++id) {
        m_robotUi[id] = std::make_unique<RobotComponent>(*m_robots[id]);
        addAndMakeVisible(*m_robotUi[id]);
    }
    m_data.addListener(this);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    m_data.removeListener(this);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void AudioPluginAudioProcessorEditor::updateChannelComboBox() {
    auto& channels = m_processor.getEnabledChannels();
    for (auto& ui: m_robotUi) {
        for (size_t i=1; i<channels.size(); ++i)
            ui->getComboBox().setItemEnabled((int)i+1, channels[i]);
    }
}

void AudioPluginAudioProcessorEditor::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) {
    int id = treeWhosePropertyHasChanged[Id];
    if (property == MidiChannel) {
        auto ch = (int)treeWhosePropertyHasChanged[MidiChannel] + 1;
        m_robotUi[(size_t) id]->getComboBox().setSelectedId(ch, dontSendNotification);
        m_processor.updateChannelStatus();
        updateChannelComboBox();
    }

    if (property == Enabled) {
        bool enabled = treeWhosePropertyHasChanged[Enabled];
        m_robotUi[(size_t)id]->setUiEnabled(enabled);
    }

    for (auto& ui: m_robotUi) {
        ui->updateUi();
    }
}
