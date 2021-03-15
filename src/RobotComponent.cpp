//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#include "RobotComponent.h"

RobotComponent::RobotComponent(ValueTree& data,
                               Robot& robot,
                               int padding) :
                               m_data(data),
                               m_robot(robot),
                               m_nameChSection(*this, m_data, padding),
                               m_hostPortSection(*this, m_data, padding/2),
                               m_deleteBtn("delete", DrawableButton::ImageFitted),
                               m_iPadding(padding)
{
    setSize(iComponentWidth, iCompHeight);
    addAndMakeVisible(m_nameChSection);
    addAndMakeVisible(m_hostPortSection);
    initBtns();

    setUiEnabled(m_data[Enabled]);
}

RobotComponent::~RobotComponent() {
    m_deleteBtn.removeListener(this);
}

void RobotComponent::paint(Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    if (m_data[Enabled])
        g.setColour(Colours::orange);
    else
        g.setColour (juce::Colours::grey);

    auto centralArea = getLocalBounds().toFloat().reduced((float)m_iPadding/2);
    g.drawRoundedRectangle (centralArea, 5.0f, 2.0f);
}

void RobotComponent::resized() {
    int iPowerBtnWidth = 56;
    auto area = getLocalBounds();
    m_nameChSection.setBounds(area.removeFromLeft(m_nameChSection.getWidth()).reduced(0, m_iPadding));
    m_hostPortSection.setBounds(area.removeFromLeft(m_hostPortSection.getWidth()).reduced(0, m_iPadding));
    m_deleteBtn.setBounds(area.removeFromLeft(iPowerBtnWidth).reduced(m_iPadding));
    setTopLeftPosition(0, (int)m_data[Id]*iCompHeight);
}

void RobotComponent::buttonClicked(Button *btn) {
    if (btn == m_hostPortSection.getConnectBtn()) {
        m_robot.toggleConnection();
    }

    if (btn == &m_deleteBtn) {
        m_data.setProperty(Enabled, btn->getToggleState(), nullptr);
        DBG("Enabled :" << (int)m_data[Enabled]);
        setUiEnabled(m_data[Enabled]);
        repaint();
    }
}

void RobotComponent::comboBoxChanged(ComboBox *comboBoxThatHasChanged) {
    if (comboBoxThatHasChanged == &m_nameChSection.getComboBox()) {
        int ch = comboBoxThatHasChanged->getSelectedItemIndex();
        m_data.setProperty(MidiChannel, ch, nullptr);
    }
}

void RobotComponent::setUiEnabled(bool enable) {
    m_nameChSection.setEnabled(enable);
    m_hostPortSection.setEnabled(enable);
}

void RobotComponent::initBtns() {
    addAndMakeVisible(m_deleteBtn);
    m_deleteBtn.setClickingTogglesState(true);
    auto icon = Drawable::createFromImageData(BinaryData::poweroffsolid_svg, BinaryData::poweroffsolid_svgSize);
    auto OverOnIcon = icon->createCopy();
    auto OnIcon = icon->createCopy();
    auto OverIcon = icon->createCopy();
    icon->replaceColour(Colours::black, Colours::darkgrey);
    OverIcon->replaceColour(Colours::black, Colours::grey);
    OverOnIcon->replaceColour(Colours::black, Colours::orange);
    OnIcon->replaceColour(Colours::black, Colours::darkorange);
    m_deleteBtn.setImages(icon.get(), OverIcon.get(), nullptr, nullptr, OnIcon.get(), OverOnIcon.get());
    m_deleteBtn.setColour(DrawableButton::backgroundOnColourId, Colours::transparentWhite);
    m_deleteBtn.addListener(this);
}

void RobotComponent::labelTextChanged(Label *labelThatHasChanged) {
    if (labelThatHasChanged == m_hostPortSection.getHostTxt()) {
        m_data.setProperty(Host, labelThatHasChanged->getText(), nullptr);
    } else if (labelThatHasChanged == m_hostPortSection.getPortTxt()) {
        m_data.setProperty(Port, labelThatHasChanged->getText().getIntValue(), nullptr);
    }
}

/******************* Name Channel Section Component *******************/
RobotComponent::NameChSection::NameChSection(Component &p, ValueTree &data, int padding) : m_parent(p), m_data(data), m_iPadding(padding) {
    setSize(120, iCompHeight);
    addAndMakeVisible(m_nameLabel);
    m_nameLabel.setText(data[Name], dontSendNotification);
    m_nameLabel.setFont(Font(24, Font::bold));
    m_nameLabel.setJustificationType(Justification::centred);

    addAndMakeVisible(m_chLabel);
    m_chLabel.setText("Ch", dontSendNotification);
    m_chLabel.attachToComponent(&m_cbChannel, true);
    m_chLabel.setJustificationType(Justification::right);

    addAndMakeVisible(m_cbChannel);
    StringArray ch;
    ch.add("-");
    for (int i=1; i<MAX_MIDI_CHANNELS; ++i) {
        ch.add(String(i));
    }

    m_cbChannel.addListener(dynamic_cast<ComboBox::Listener *>(&m_parent));
    m_cbChannel.addItemList(ch, 1);
    m_cbChannel.setSelectedId((int)m_data[MidiChannel] + 1);
}

RobotComponent::NameChSection::~NameChSection() {
    m_cbChannel.removeListener(dynamic_cast<ComboBox::Listener *>(&m_parent));
}

void RobotComponent::NameChSection::resized() {
    int iChLabelWidth = 28;
    int iChCbWidth = 80;

    auto area = getLocalBounds();
    m_nameLabel.setBounds(area.removeFromTop(getHeight()/2).reduced(m_iPadding));
    area.removeFromLeft(iChLabelWidth);
    m_cbChannel.setBounds(area.removeFromLeft(iChCbWidth).reduced(m_iPadding));
}

/****************** HostPortSection Component ******************/
RobotComponent::HostPortSection::HostPortSection(Component &p,
                                                 ValueTree &data,
                                                 int padding) :
                                                 m_parent(p),
                                                 m_data(data),
                                                 m_iPadding(padding) {
    setSize(256, iCompHeight);
    addAndMakeVisible(m_hostTxt);
    m_hostTxt.setName(Host.toString());
    m_hostTxt.setText(data[Host], dontSendNotification);
    m_hostTxt.setEditable(true);
    m_hostTxt.setColour(Label::backgroundColourId, Colours::darkgrey);
    m_hostTxt.addListener(dynamic_cast<Label::Listener *>(&m_parent));

    addAndMakeVisible(m_portTxt);
    m_portTxt.setName(Port.toString());
    m_portTxt.setText(data[Port], dontSendNotification);
    m_portTxt.setEditable(true);
    m_portTxt.setColour(Label::backgroundColourId, Colours::darkgrey);
    m_portTxt.addListener(dynamic_cast<Label::Listener *>(&m_parent));

    addAndMakeVisible(m_hostLabel);
    m_hostLabel.setText("Host", dontSendNotification);
    m_hostLabel.attachToComponent(&m_hostTxt, true);
    m_hostLabel.setJustificationType(Justification::right);

    addAndMakeVisible(m_portLabel);
    m_portLabel.setText("Port", dontSendNotification);
    m_portLabel.attachToComponent(&m_portTxt, true);
    m_portLabel.setJustificationType(Justification::right);

    addAndMakeVisible(m_connectBtn);
    m_connectBtn.addListener(dynamic_cast<Button::Listener *>(&m_parent));
}

RobotComponent::HostPortSection::~HostPortSection() {
    m_hostTxt.removeListener(dynamic_cast<Label::Listener *>(&m_parent));
    m_portTxt.removeListener(dynamic_cast<Label::Listener *>(&m_parent));
    m_connectBtn.removeListener(dynamic_cast<Button::Listener *>(&m_parent));
}

void RobotComponent::HostPortSection::setEnabled(bool enable) {
    Component::setEnabled(enable);
    repaint();
}

void RobotComponent::HostPortSection::updateBtnUi() {
    bool stat = m_data[ConnectionStatus];
    auto color = m_data[Enabled] ? (stat ? Colours::red : Colours::grey) : Colours::darkgrey;
    m_connectBtn.setColour(TextButton::buttonColourId, color);
    String btnText = stat ? "Disconnect" : "Connect";
    m_connectBtn.setButtonText(btnText);
}

void RobotComponent::HostPortSection::paint(Graphics &g) {
    g.setColour (juce::Colours::grey);
    g.drawVerticalLine(0, 0, (float)getHeight());

    updateBtnUi();

}

void RobotComponent::HostPortSection::resized() {
    int iConnectionBtnWidth = 100;
    int iLabelWidth = 36;

    auto area = getLocalBounds();
    area.removeFromLeft(iLabelWidth);
    m_connectBtn.setBounds(area.removeFromRight(iConnectionBtnWidth).reduced(m_iPadding));
    m_hostTxt.setBounds(area.removeFromTop(getHeight()/2).reduced(m_iPadding));
    m_portTxt.setBounds(area.reduced(m_iPadding));
}
