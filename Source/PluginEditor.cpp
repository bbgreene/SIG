/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SIGAudioProcessorEditor::SIGAudioProcessorEditor (SIGAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // set default font
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Avenir Next");
    
    // Dials and buttons with their respective attachments
    signalAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "signal type", signalType);
    
    sineButton.setClickingTogglesState(false);
    sineButton.onClick = [this]()
    {
        signalType.setValue(0);
    };
    addAndMakeVisible(sineButton);
    
    whiteButton.setClickingTogglesState(false);
    whiteButton.onClick = [this]()
    {
        signalType.setValue(1);
    };
    addAndMakeVisible(whiteButton);
    
    pinkButton.setClickingTogglesState(false);
    pinkButton.onClick = [this]()
    {
        signalType.setValue(2);
    };
    addAndMakeVisible(pinkButton);
    
    
    routingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "routing", routing);
    
    LButton.setClickingTogglesState(false);
    LButton.onClick = [this]()
    {
        routing.setValue(0);
    };
    addAndMakeVisible(LButton);
    
    LRButton.setClickingTogglesState(false);
    LRButton.onClick = [this]()
    {
        routing.setValue(1);
    };
    addAndMakeVisible(LRButton);
    
    RButton.setClickingTogglesState(false);
    RButton.onClick = [this]()
    {
        routing.setValue(2);
    };
    addAndMakeVisible(RButton);
    
    freqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "freq", freq);
    freq.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialDotModernStyle);
    addAndMakeVisible(freq);
    
    hundredButton.setClickingTogglesState(false);
    hundredButton.onClick = [this]()
    {
        freq.setValue(100.0);
    };
    addAndMakeVisible(hundredButton);
    
    oneThousButton.setClickingTogglesState(false);
    oneThousButton.onClick = [this]()
    {
        freq.setValue(1000.0);
    };
    addAndMakeVisible(oneThousButton);
    
    tenThousButton.setClickingTogglesState(false);
    tenThousButton.onClick = [this]()
    {
        freq.setValue(10000.0);
    };
    addAndMakeVisible(tenThousButton);
    
    
//    minusTwentyButton.setClickingTogglesState(false);
//    minusTwentyButton.onClick = [this]()
//    {
//        freq.setValue(-20.0);
//    };
//    addAndMakeVisible(minusTwentyButton);
//
//    minusTwelveButton.setClickingTogglesState(false);
//    minusTwelveButton.onClick = [this]()
//    {
//        freq.setValue(-12.0);
//    };
//    addAndMakeVisible(minusTwelveButton);
//
//    minusSixButton.setClickingTogglesState(false);
//    minusSixButton.onClick = [this]()
//    {
//        freq.setValue(-6.0);
//    };
//    addAndMakeVisible(minusSixButton);
    
    
    //Titles
    sigTitle.setFont(juce::Font (26.0f, juce::Font::plain));
    sigTitle.setJustificationType(juce::Justification::centredLeft);
    sigTitle.setColour(juce::Label::textColourId, juce::Colours::darkslategrey);
    addAndMakeVisible(sigTitle);
    
    sigVersion.setFont(juce::Font (15.0f, juce::Font::plain));
    sigVersion.setJustificationType(juce::Justification::centredLeft);
    sigVersion.setColour(juce::Label::textColourId, juce::Colours::mintcream);
    addAndMakeVisible(sigVersion);
    
    olumay.setFont(juce::Font (15.0f, juce::Font::plain));
    olumay.setJustificationType(juce::Justification::centredLeft);
    olumay.setColour(juce::Label::textColourId, juce::Colours::rebeccapurple);
    addAndMakeVisible(olumay);
    
    // Resizing
    setResizable(true, true);
    setResizeLimits(350, 350, 490, 490);
    getConstrainer()->setFixedAspectRatio(1.0);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (350, 350);
}

SIGAudioProcessorEditor::~SIGAudioProcessorEditor()
{
}

//==============================================================================
void SIGAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> background = getLocalBounds();
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::lightskyblue, getHeight() - getHeight(), juce::Colours::lightskyblue.darker(), getHeight()));
    g.fillRect(background);
}

void SIGAudioProcessorEditor::resized()
{
    auto leftMargin = 20.0f;
    auto buttonWidth = getWidth() * 0.13;
    auto buttonHeight = (getHeight()/7) * 0.5;
    auto buttonTopMargin = getHeight() * 0.23;
    auto buttonGap = getWidth() * 0.01428;
    auto buttonRightSideStartPos = (getWidth() / 2) + leftMargin - 8;
    
    sineButton.setBounds(leftMargin, buttonTopMargin, buttonWidth, buttonHeight);
    whiteButton.setBounds(sineButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);
    pinkButton.setBounds(whiteButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);
    
    LButton.setBounds(buttonRightSideStartPos, buttonTopMargin, buttonWidth, buttonHeight);
    LRButton.setBounds(LButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);
    RButton.setBounds(LRButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);
    
    hundredButton.setBounds(leftMargin, buttonTopMargin + 80, buttonWidth, buttonHeight);
    oneThousButton.setBounds(hundredButton.getRight() + buttonGap, buttonTopMargin + 80, buttonWidth, buttonHeight);
    tenThousButton.setBounds(oneThousButton.getRight() + buttonGap, buttonTopMargin + 80, buttonWidth, buttonHeight);
    
//    minusTwentyButton.setBounds(leftMargin, buttonTopMargin + 80, buttonWidth, buttonHeight);
//    minusTwelveButton.setBounds(minusTwentyButton.getRight() + buttonGap, buttonTopMargin + 80, buttonWidth, buttonHeight);
//    minusSixButton.setBounds(minusTwelveButton.getRight() + buttonGap, buttonTopMargin + 80, buttonWidth, buttonHeight);
    
    auto dialSize = (getWidth() * 0.5) - 75;
    auto freqDialXPos = 37.5;
    
    freq.setBounds(freqDialXPos, 200, dialSize, dialSize);
    
    auto olumayY = 325.0;//getHeight() * 0.9596;
    auto olumayWidth = getWidth() * 0.3;
    auto sigVersionWidth = getWidth() * 0.233;
    auto allTitlesHeight = 20.0;//getHeight() * 0.0404;
    auto titlesTopMargin = 20.0;//getHeight() * 0.0101;
    
    olumay.setBounds(leftMargin, olumayY, olumayWidth, allTitlesHeight);
    sigTitle.setBounds(leftMargin, titlesTopMargin, allTitlesHeight * 3.0, allTitlesHeight);
    sigVersion.setBounds(sigTitle.getRight(), titlesTopMargin, sigVersionWidth, allTitlesHeight);
    
}
