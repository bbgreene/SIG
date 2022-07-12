/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../Source/bbg_gui/Dial.h"
#include "../Source/bbg_gui/Toggle.h"
#include "../Source/bbg_gui/PushButton.h"
#include "../Source/bbg_gui/Menu.h"
#include "../Source/bbg_gui/Label.h"

//==============================================================================
/**
*/
class SIGAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SIGAudioProcessorEditor (SIGAudioProcessor&);
    ~SIGAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    //dials and buttons
    bbg_gui::bbg_Dial signalType { " Signal", 0.0, 2.0, 1.0, 0.0, 0.0 };
    bbg_gui::bbg_PushButton sineButton { "Sine" };
    bbg_gui::bbg_PushButton whiteButton { "White" };
    bbg_gui::bbg_PushButton pinkButton { "Pink" };
    
    bbg_gui::bbg_Dial routing { " Routing", 0.0, 2.0, 1.0, 1.0, 0.0 };
    bbg_gui::bbg_PushButton LButton { "L" };
    bbg_gui::bbg_PushButton LRButton { "L+R" };
    bbg_gui::bbg_PushButton RButton { "R" };
    
    bbg_gui::bbg_Dial freq { " Hz", 0.0, 21000.0, 1.0, 440.0, 0.0 };
    bbg_gui::bbg_PushButton hundredButton { "100Hz" };
    bbg_gui::bbg_PushButton oneThousButton { "1kHz" };
    bbg_gui::bbg_PushButton tenThousButton { "10kHz" };
    
    bbg_gui::bbg_Dial gain { " dB", -120.0, 0.0, 0.01, -20.0, 0.0 };
    bbg_gui::bbg_PushButton minusTwentyButton { "-20dB" };
    bbg_gui::bbg_PushButton minusTwelveButton { "-12dB" };
    bbg_gui::bbg_PushButton minusSixButton { "-6dB" };
    
    bbg_gui::bbg_PushButton onOffSwitch { "On" };
    
    //Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> signalAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> routingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> onOffAttachment;
    
    
    //Labels
    bbg_gui::bbg_dialLabel olumay { "Olumay dsp" };
    bbg_gui::bbg_dialLabel sigTitle { "S I G" };
    bbg_gui::bbg_dialLabel sigVersion { "version 1.1" };
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SIGAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SIGAudioProcessorEditor)
};
