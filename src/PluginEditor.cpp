//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p, Robots& robots, ValueTree& data, bool isDummy)
        : AudioProcessorEditor (&p), m_processor (p), m_data(data), m_robots(robots), m_bIsDummy(isDummy)
{
    setSize(iComponentWidth, iCompHeight * MAX_ROBOTS);
    if (m_bIsDummy)
        return;

    for (size_t id=0; id<ulNumRobots; ++id) {
        m_robotUi[id] = std::make_unique<RobotComponent>(*m_robots[id]);
        addAndMakeVisible(*m_robotUi[id]);
    }
    m_data.addListener(this);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    if (m_bIsDummy) {
        g.setColour(Colours::white);
        auto area = getLocalBounds();
        String txt = "An instance is already open on track \"";
        txt << MainProcessorInfo::properties.name << "\".";
        g.setFont(18);
        g.drawText( txt,area.removeFromTop(area.getWidth()/2).reduced(8), Justification::centredBottom);
        g.drawText("Cannot create multiple instances of the plugin.",
                   area.reduced(8), Justification::centredTop);
    }
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
}

void AudioPluginAudioProcessorEditor::valueTreeRedirected(ValueTree &treeWhichHasBeenChanged) {
    for (auto& ui: m_robotUi) {
        ui->updateUi();
    }
    m_processor.updateChannelStatus();
    updateChannelComboBox();
//    DBG("Value redirected :" << treeWhichHasBeenChanged.getType());
}
