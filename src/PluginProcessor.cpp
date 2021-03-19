//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
        : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
), m_data("MidiOscPlugin"), m_robots(m_data), m_bDummyUi(false), m_bCanUseStateInfo(true)
{
    if (iInstanceRefCount > 0) {
        DBG("Already running");
        ulNumRobots = 0;
        m_bDummyUi = true;
        m_bCanUseStateInfo = false;
        return;
    }

    initData();
    m_robots.addRobots();
    std::fill(m_enabledChannels.begin(), m_enabledChannels.end(), true);
    auto list = juce::MidiInput::getAvailableDevices();
    m_midiInput = juce::MidiInput::openDevice(list[0].identifier, this);
    if (!m_midiInput) {
        DBG("Cannot open MIDI device");
    } else {
        m_midiInput->start();
    }
    iInstanceRefCount++;
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
    if (iInstanceRefCount < 1) {
        m_robots.removeRobots();
    }
    iInstanceRefCount--;
}

void AudioPluginAudioProcessor::initData() {
    for (int i=0; i<MAX_ROBOTS; ++i) {
        ValueTree node {kRobotList[i]};
        m_data.appendChild(node, nullptr);
    }
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    buffer.clear();

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this, m_robots, m_data, m_bDummyUi);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    if (!m_bCanUseStateInfo)
        return;
    m_robots.disconnectAll();
//    DBG("Get state");
//    DBG(m_robots);

    copyXmlToBinary(*m_data.createXml(), destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    if (!m_bCanUseStateInfo)
        return;

    auto xml = getXmlFromBinary(data, sizeInBytes);
    if (xml) {
        if (xml->hasTagName(m_data.getType())) {
            auto tree = ValueTree::fromXml(*xml);
            m_robots.setData(tree); // First set the child tree before the parents to have the callback fired after the child is updated
            m_data = tree; // Callback fired here
            auto* editor = getActiveEditor();
            if (editor) {
                editor->repaint();
//                DBG("Set state");
//                DBG(m_robots);
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

//==============================================================================
void AudioPluginAudioProcessor::updateChannelStatus() {
    std::fill(m_enabledChannels.begin(), m_enabledChannels.end(), true);
    for (auto node: m_data) {
        int ch = node[MidiChannel];
        enableChannel(ch, false);
    }
}

void AudioPluginAudioProcessor::enableChannel(int ch, bool enable) {
    m_enabledChannels[(size_t)ch] = enable;
}

int AudioPluginAudioProcessor::getAvailableChannel() {
    for (size_t i = 1; i < MAX_MIDI_CHANNELS; ++i) {
        if (m_enabledChannels[i])
            return (int)i;
    }
    return 0;
}

void AudioPluginAudioProcessor::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) {
    juce::ignoreUnused(source);
    m_robots.send(message);
}
