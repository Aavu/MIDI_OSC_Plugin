//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#include "RobotComponent.h"

RobotComponent::RobotComponent(Robot& robot,
                               int padding) :
                               m_robot(robot),
                               m_nameChSection(*this, m_robot, padding),
                               m_hostPortSection(*this, m_robot, padding/2),
                               m_deleteBtn("delete", DrawableButton::ImageFitted),
                               m_iPadding(padding)
{
    setSize(iComponentWidth, iCompHeight);
    if (MainProcessorInfo::properties.colour != Colours::transparentBlack) {
        m_activeColor = MainProcessorInfo::properties.colour;
    }

    addAndMakeVisible(m_nameChSection);
    addAndMakeVisible(m_hostPortSection);
    initBtns();

    setUiEnabled(m_robot.isEnabled());
}

RobotComponent::~RobotComponent() {
    m_deleteBtn.removeListener(this);
}

void RobotComponent::paint(Graphics &g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    auto enabled = m_robot.isEnabled();
    m_borderColor = enabled ? m_activeColor : Colours::grey;
    g.setColour(m_borderColor);

    auto centralArea = getLocalBounds().toFloat().reduced((float)m_iPadding/2);
    g.drawRoundedRectangle (centralArea, 5.0f, 2.0f);
}

void RobotComponent::resized() {
    int iPowerBtnWidth = 56;
    auto area = getLocalBounds();
    m_nameChSection.setBounds(area.removeFromLeft(m_nameChSection.getWidth()).reduced(0, m_iPadding));
    m_hostPortSection.setBounds(area.removeFromLeft(m_hostPortSection.getWidth()).reduced(0, m_iPadding));
    m_deleteBtn.setBounds(area.removeFromLeft(iPowerBtnWidth).reduced(m_iPadding));
    setTopLeftPosition(0, (int)m_robot.getId()*iCompHeight);
}

void RobotComponent::buttonClicked(Button *btn) {
    if (btn == m_hostPortSection.getConnectBtn()) {
        m_robot.toggleConnection();
    }

    if (btn == &m_deleteBtn) {
        auto on = btn->getToggleState();
        m_robot.setEnabled(on);
        if (!on) {
            m_robot.disconnect();
        }
        setUiEnabled(on);
    }
}

void RobotComponent::comboBoxChanged(ComboBox *comboBoxThatHasChanged) {
    if (comboBoxThatHasChanged == &m_nameChSection.getComboBox()) {
        int ch = comboBoxThatHasChanged->getSelectedItemIndex();
        m_robot.setMidiChannel(ch);
    }
}

void RobotComponent::updateUi() {
    bool enabled = m_robot.isEnabled();
    setUiEnabled(enabled);
    m_nameChSection.updateUi();
    m_hostPortSection.updateUi();
    repaint();
}

void RobotComponent::setUiEnabled(bool enable) {
    m_deleteBtn.setToggleState(enable, dontSendNotification);
    m_nameChSection.setEnabled(enable);
    m_hostPortSection.setEnabled(enable);
    repaint();
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
        m_robot.setHost(labelThatHasChanged->getText());
    } else if (labelThatHasChanged == m_hostPortSection.getPortTxt()) {
        m_robot.setPort(labelThatHasChanged->getText().getIntValue());
    }
}

/******************* Name Channel Section Component *******************/
RobotComponent::NameChSection::NameChSection(Component &p, Robot& robot, int padding) : m_parent(p), m_robot(robot), m_iPadding(padding) {
    setSize(120, iCompHeight);
    addAndMakeVisible(m_nameLabel);
    m_nameLabel.setText(m_robot.getName(), dontSendNotification);
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
    m_cbChannel.setSelectedId(m_robot.getMidiChannel() + 1);
}

RobotComponent::NameChSection::~NameChSection() {
    m_cbChannel.removeListener(dynamic_cast<ComboBox::Listener *>(&m_parent));
}

void RobotComponent::NameChSection::paint(Graphics& g) {
}

void RobotComponent::NameChSection::resized() {
    int iChLabelWidth = 28;
    int iChCbWidth = 80;

    auto area = getLocalBounds();
    m_nameLabel.setBounds(area.removeFromTop(getHeight()/2).reduced(m_iPadding));
    area.removeFromLeft(iChLabelWidth);
    m_cbChannel.setBounds(area.removeFromLeft(iChCbWidth).reduced(m_iPadding));
}

void RobotComponent::NameChSection::updateUi() {
    m_cbChannel.setSelectedId(m_robot.getMidiChannel() + 1, dontSendNotification);
}

/****************** HostPortSection Component ******************/
RobotComponent::HostPortSection::HostPortSection(Component &p,
                                                 Robot& robot,
                                                 int padding) :
                                                 m_parent(p),
                                                 m_robot(robot),
                                                 m_iPadding(padding) {
    setSize(256, iCompHeight);
    addAndMakeVisible(m_hostTxt);
    m_hostTxt.setText(m_robot.getHost(), dontSendNotification);
    m_hostTxt.setEditable(true);
    m_hostTxt.setColour(Label::backgroundColourId, Colours::darkgrey);
    m_hostTxt.addListener(dynamic_cast<Label::Listener *>(&m_parent));

    addAndMakeVisible(m_portTxt);
    m_portTxt.setText(String(m_robot.getPort()), dontSendNotification);
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
    updateBtnUi();
    m_connectBtn.addListener(dynamic_cast<Button::Listener *>(&m_parent));
}

RobotComponent::HostPortSection::~HostPortSection() {
    m_hostTxt.removeListener(dynamic_cast<Label::Listener *>(&m_parent));
    m_portTxt.removeListener(dynamic_cast<Label::Listener *>(&m_parent));
    m_connectBtn.removeListener(dynamic_cast<Button::Listener *>(&m_parent));
}

void RobotComponent::HostPortSection::updateUi() {
//    DBG(m_robot.getHost() << " " << m_robot.getPort());
    m_hostTxt.setText(m_robot.getHost(), dontSendNotification);
    m_portTxt.setText(String(m_robot.getPort()), dontSendNotification);
    updateBtnUi();
}

void RobotComponent::HostPortSection::updateBtnUi() {
    bool stat = m_robot.isConnected();
    auto color = m_robot.isEnabled() ? (stat ? Colours::red : Colours::grey) : Colours::darkgrey;
    m_connectBtn.setColour(TextButton::buttonColourId, color);
    m_connectBtn.setButtonText(stat ? "Disconnect" : "Connect");
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
