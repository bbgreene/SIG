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
    freq.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
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
    
    
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "gain", gain);
    gain.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(gain);
    
    minusTwentyButton.setClickingTogglesState(false);
    minusTwentyButton.onClick = [this]()
    {
        gain.setValue(-20.0);
    };
    addAndMakeVisible(minusTwentyButton);

    minusTwelveButton.setClickingTogglesState(false);
    minusTwelveButton.onClick = [this]()
    {
        gain.setValue(-12.0);
    };
    addAndMakeVisible(minusTwelveButton);

    minusSixButton.setClickingTogglesState(false);
    minusSixButton.onClick = [this]()
    {
        gain.setValue(-6.0);
    };
    addAndMakeVisible(minusSixButton);
    
    onOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "bypass", onOffSwitch);
    addAndMakeVisible(onOffSwitch);
    
    // Titles
    sigTitle.setFont(juce::Font (30.0f, juce::Font::plain));
    sigTitle.setJustificationType(juce::Justification::centredLeft);
    sigTitle.setColour(juce::Label::textColourId, juce::Colours::darkslategrey);
    addAndMakeVisible(sigTitle);
    
    sigVersion.setFont(juce::Font (16.0f, juce::Font::plain));
    sigVersion.setJustificationType(juce::Justification::centredLeft);
    sigVersion.setColour(juce::Label::textColourId, juce::Colours::dimgrey.brighter());
    addAndMakeVisible(sigVersion);
    
    olumay.setFont(juce::Font (16.0f, juce::Font::plain));
    olumay.setJustificationType(juce::Justification::centredLeft);
    olumay.setColour(juce::Label::textColourId, juce::Colours::darkslategrey);
    addAndMakeVisible(olumay);
    
    // Borders
    signalGroup.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::lightgrey);
    signalGroup.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::grey);
    signalGroup.setTextLabelPosition(juce::Justification::centred);
    signalGroup.setText("SIGNAL TYPE");
    addAndMakeVisible(signalGroup);
    
    routingGroup.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::lightgrey);
    routingGroup.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::grey);
    routingGroup.setTextLabelPosition(juce::Justification::centred);
    routingGroup.setText("ROUTING");
    addAndMakeVisible(routingGroup);
    
    freqGroup.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::lightgrey);
    freqGroup.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::grey);
    freqGroup.setTextLabelPosition(juce::Justification::centred);
    freqGroup.setText("FREQUENCY");
    addAndMakeVisible(freqGroup);
    
    gainGroup.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::lightgrey);
    gainGroup.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::grey);
    gainGroup.setTextLabelPosition(juce::Justification::centred);
    gainGroup.setText("GAIN");
    addAndMakeVisible(gainGroup);
    
    // Resizing
    setResizable(true, true);
    setResizeLimits(350, 350, 500, 500);
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
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::powderblue, getHeight() * 0.78, juce::Colours::lightskyblue, getHeight()));
    g.fillRect(background);
}

void SIGAudioProcessorEditor::resized()
{
    auto dialSize = getWidth() * 0.33;
    auto freqDialXPos = getWidth() * 0.087;
    auto gainDialXPos = getWidth() * 0.577;
    auto dialY = getHeight() * 0.55;
    auto leftMargin = getHeight() * 0.05;
    
    auto buttonWidth = getWidth() * 0.13;
    auto buttonHeight = (getHeight()/7) * 0.5;
    auto buttonTopMargin = getHeight() * 0.23;
    auto buttonYPosSecondRow = getHeight() * 0.4586;
    auto buttonGap = getWidth() * 0.01428;
    auto buttonRightSideStartPos = getWidth() * 0.54;
    
    freq.setBounds(freqDialXPos, dialY, dialSize, dialSize);
    gain.setBounds(gainDialXPos, dialY, dialSize, dialSize);
    
    sineButton.setBounds(leftMargin, buttonTopMargin, buttonWidth, buttonHeight);
    whiteButton.setBounds(sineButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);
    pinkButton.setBounds(whiteButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);

    LButton.setBounds(buttonRightSideStartPos, buttonTopMargin, buttonWidth, buttonHeight);
    LRButton.setBounds(LButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);
    RButton.setBounds(LRButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);

    hundredButton.setBounds(leftMargin, buttonYPosSecondRow, buttonWidth, buttonHeight);
    oneThousButton.setBounds(hundredButton.getRight() + buttonGap, buttonYPosSecondRow, buttonWidth, buttonHeight);
    tenThousButton.setBounds(oneThousButton.getRight() + buttonGap, buttonYPosSecondRow, buttonWidth, buttonHeight);
    
    minusTwentyButton.setBounds(buttonRightSideStartPos, buttonYPosSecondRow, buttonWidth, buttonHeight);
    minusTwelveButton.setBounds(minusTwentyButton.getRight() + buttonGap, buttonYPosSecondRow, buttonWidth, buttonHeight);
    minusSixButton.setBounds(minusTwelveButton.getRight() + buttonGap, buttonYPosSecondRow, buttonWidth, buttonHeight);
    
    auto borderColOneX = sineButton.getX() * 0.52;
    auto borderColTwoX = LButton.getX() * 0.95;
    auto borderRowOneY = sineButton.getY() * 0.73;
    auto borderRowTwoY = hundredButton.getY() * 0.86;
    auto borderWidth = sineButton.getWidth() * 3.6;
    auto smallBorderH = sineButton.getHeight() * 2.58;
    auto largeBorderH = hundredButton.getHeight() * 6.9;
    
    signalGroup.setBounds(borderColOneX, borderRowOneY, borderWidth, smallBorderH);
    routingGroup.setBounds(borderColTwoX, borderRowOneY, borderWidth, smallBorderH);
    freqGroup.setBounds(borderColOneX, borderRowTwoY, borderWidth, largeBorderH);
    gainGroup.setBounds(borderColTwoX, borderRowTwoY, borderWidth, largeBorderH);
    
    auto olumayX = getWidth() * 0.015;
    auto olumayY = getHeight() * 0.92;
    auto olumayWidth = getWidth() * 0.3;
    auto sigTitleWidth = getWidth() * 0.17;
    auto sigTitleHeight = getHeight() * 0.058;
    auto sigVersionX = getWidth() * 0.175;
    auto sigVersionY = getHeight() * 0.05;
    auto sigVersionWidth = getWidth() * 0.233;
    auto allTitlesHeight = getHeight() * 0.05;
    auto titlesTopMargin = getHeight() * 0.05;
    
    olumay.setBounds(olumayX, olumayY, olumayWidth, allTitlesHeight);
    sigTitle.setBounds(olumayX, titlesTopMargin, sigTitleWidth, sigTitleHeight);
    sigVersion.setBounds(sigVersionX, sigVersionY, sigVersionWidth, allTitlesHeight);
    
    onOffSwitch.setBounds(buttonRightSideStartPos, titlesTopMargin, buttonWidth, buttonHeight);
    
}
