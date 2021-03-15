//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
        : AudioProcessorEditor (&p), m_processor (p), m_robots(p.getRobots())
{
    setSize(iComponentWidth, iCompHeight * MAX_ROBOTS);
    m_robots.rootData.addListener(this);
    for (auto node: m_robots.rootData) {
        int id = node[Id];
        m_robotUi[(size_t)id] = std::make_unique<RobotComponent>(node, *m_robots.at((size_t)id));
        addAndMakeVisible(*m_robotUi[(size_t)id]);
    }
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    m_robots.rootData.removeListener(this);
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

void AudioPluginAudioProcessorEditor::buttonClicked(Button *btn) {

}

void AudioPluginAudioProcessorEditor::updateChannelComboBox() {
    auto& channels = m_processor.getEnabledChannels();
    for (auto& ui: m_robotUi) {
        for (size_t i=1; i<channels.size(); ++i)
            ui->getComboBox().setItemEnabled((int)i+1, channels[i]);
    }
}

void AudioPluginAudioProcessorEditor::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) {
    if (property == MidiChannel) {
        m_processor.updateChannelStatus();
        updateChannelComboBox();
    }
}

void AudioPluginAudioProcessorEditor::valueTreeChildAdded(ValueTree &/*parentTree*/, ValueTree &childWhichHasBeenAdded) {

}

void AudioPluginAudioProcessorEditor::valueTreeChildRemoved(ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved,
                                                            int indexFromWhichChildWasRemoved) {

}
