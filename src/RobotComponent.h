//
// Created by Raghavasimhan Sankaranarayanan on 3/14/21.
//

#ifndef MIDI_OSC_ROBOTCOMPONENT_H
#define MIDI_OSC_ROBOTCOMPONENT_H

#include "JuceHeader.h"
#include "Robot.h"
#include "Defines.h"

// UI for each robot
class RobotComponent :  public Component,
                        public juce::Button::Listener,
                        public juce::Label::Listener,
                        public juce::ComboBox::Listener
{
public:
    explicit RobotComponent(Robot& robot, int padding = 8);
    ~RobotComponent() override;

    void paint(Graphics &g) override;
    void resized() override;

    void updateUi();
    void setUiEnabled(bool enable);

    [[nodiscard]] ComboBox& getComboBox() { return m_nameChSection.getComboBox(); }

private:
    void initBtns();
    void buttonClicked(Button* btn) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void labelTextChanged (Label* labelThatHasChanged) override;

private:
    Robot& m_robot;

    class NameChSection: public Component {
    public:
        explicit NameChSection(Component& p, Robot& robot, int padding);
        ~NameChSection() override;
        void paint(Graphics& g) override;
        void resized() override;

        void updateUi();

        [[nodiscard]] ComboBox& getComboBox() { return m_cbChannel; }

    private:
        Component& m_parent;
        Robot& m_robot;

        Label m_nameLabel;
        Label m_chLabel;
        ComboBox m_cbChannel;

        int m_iPadding;
    } m_nameChSection;

    class HostPortSection: public Component {
    public:
        explicit HostPortSection(Component& p, Robot& robot, int padding);
        ~HostPortSection() override;
        void paint(Graphics& g) override;
        void resized() override;

        void updateUi();
        void updateBtnUi();

        [[nodiscard]] const Label* getHostTxt() const { return &m_hostTxt; }
        [[nodiscard]] const Label* getPortTxt() const { return &m_portTxt; }
        [[nodiscard]] const TextButton* getConnectBtn() const { return &m_connectBtn; }

    private:
        Component& m_parent;
        Robot& m_robot;

        Label m_hostTxt;
        Label m_portTxt;

        Label m_hostLabel;
        Label m_portLabel;

        TextButton m_connectBtn;

        int m_iPadding;

    } m_hostPortSection;

    DrawableButton m_deleteBtn;
    int m_iPadding;

    Colour m_activeColor = Colours::orange;
    Colour m_borderColor = Colours::grey;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RobotComponent)
};


#endif //MIDI_OSC_ROBOTCOMPONENT_H
