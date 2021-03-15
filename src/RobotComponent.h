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
    explicit RobotComponent(ValueTree& data, Robot& robot, int padding = 8);
    ~RobotComponent() override;

    void paint(Graphics &g) override;
    void resized() override;

    void setUiEnabled(bool enable);

    [[nodiscard]] ComboBox& getComboBox() { return m_nameChSection.getComboBox(); }

private:
    void initBtns();
    void buttonClicked(Button* btn) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void labelTextChanged (Label* labelThatHasChanged) override;

private:
    ValueTree m_data;
    Robot& m_robot;

    class NameChSection: public Component {
    public:
        explicit NameChSection(Component& p, ValueTree& data, int padding);
        ~NameChSection() override;
//        void paint(Graphics& g) override {
//            g.fillAll(Colours::black);
//        }
        void resized() override;
        [[nodiscard]] ComboBox& getComboBox() { return m_cbChannel; }

    private:
        Component& m_parent;
        ValueTree& m_data;

        Label m_nameLabel;
        Label m_chLabel;
        ComboBox m_cbChannel;

        int m_iPadding;
    } m_nameChSection;

    class HostPortSection: public Component {
    public:
        explicit HostPortSection(Component& p, ValueTree& data, int padding);
        ~HostPortSection() override;
        void setEnabled(bool enable);
        void paint(Graphics& g) override;
        void resized() override;

        void updateBtnUi();

        [[nodiscard]] const Label* getHostTxt() const { return &m_hostTxt; }
        [[nodiscard]] const Label* getPortTxt() const { return &m_portTxt; }
        [[nodiscard]] const TextButton* getConnectBtn() const { return &m_connectBtn; }

    private:
        Component& m_parent;
        ValueTree& m_data;

        Label m_hostTxt;
        Label m_portTxt;

        Label m_hostLabel;
        Label m_portLabel;

        TextButton m_connectBtn;

        int m_iPadding;

    } m_hostPortSection;

    DrawableButton m_deleteBtn;
    int m_iPadding;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RobotComponent)
};


#endif //MIDI_OSC_ROBOTCOMPONENT_H
