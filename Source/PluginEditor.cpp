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
    // SET DEFAULT FONT
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Avenir Next");
    
    //SIGNAL TYPE BUTTONS AND ATTACHMENTS
    sineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "sine", sineButton);
    whiteAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "white", whiteButton);
    pinkAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "pink", pinkButton);
    
    sineButton.setClickingTogglesState(true);
    sineButton.setRadioGroupId(1);
    addAndMakeVisible(sineButton);

    whiteButton.setClickingTogglesState(true);
    whiteButton.setRadioGroupId(1);
    addAndMakeVisible(whiteButton);
    
    pinkButton.setClickingTogglesState(true);
    pinkButton.setRadioGroupId(1);
    addAndMakeVisible(pinkButton);

    //ROUTING BUTTONS AND ATTACHMENTS
    lAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "l", lButton);
    lrAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "lr", lRButton);
    rAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "r", rButton);
    
    lButton.setClickingTogglesState(true);
    lButton.setRadioGroupId(2);
    addAndMakeVisible(lButton);

    lRButton.setClickingTogglesState(true);
    lRButton.setRadioGroupId(2);
    addAndMakeVisible(lRButton);
    
    rButton.setClickingTogglesState(true);
    rButton.setRadioGroupId(2);
    addAndMakeVisible(rButton);
    
    //FREQ BUTTONS AND ATTACHMENTS
    freqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "freq", freq);
    freq.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(freq);
    
    hundredAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "hundred", hundredButton);
    oneThousAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "thousand", oneThousButton);
    tenThousAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "tenThous", tenThousButton);
    
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
    
    //GAIN BUTTONS AND ATTACHMENTS
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "gain", gain);
    gain.setDialStyle(bbg_gui::bbg_Dial::DialStyle::kDialModernStyle);
    addAndMakeVisible(gain);
    
    minusTwentyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "minus twenty", minusTwentyButton);
    minusTwelveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "minus twelve", minusTwelveButton);
    minusSixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "minus six", minusSixButton);
    
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
    
    //BYPASS ON/OFF BUTTON AND ATTACHMENT
    onOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, "bypass", onOffSwitch);
    addAndMakeVisible(onOffSwitch);
    
    // TITLE
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
    
    // BORDERS
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
    
    // RESIZING
    setResizable(false, false);
//    setResizeLimits(350, 350, 500, 500);
//    getConstrainer()->setFixedAspectRatio(1.0);
    
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
    auto dialY = getHeight() * 0.597;
    auto leftMargin = getHeight() * 0.05;
    
    auto buttonWidth = getWidth() * 0.13;
    auto buttonHeight = (getHeight()/7) * 0.5;
    auto buttonTopMargin = getHeight() * 0.26;
    auto buttonYPosSecondRow = getHeight() * 0.51;
    auto buttonGap = getWidth() * 0.01428;
    auto buttonRightSideStartPos = getWidth() * 0.54;
    
    freq.setBounds(freqDialXPos, dialY, dialSize, dialSize);
    gain.setBounds(gainDialXPos, dialY, dialSize, dialSize);
    
    sineButton.setBounds(leftMargin, buttonTopMargin, buttonWidth, buttonHeight);
    whiteButton.setBounds(sineButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);
    pinkButton.setBounds(whiteButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);

    lButton.setBounds(buttonRightSideStartPos, buttonTopMargin, buttonWidth, buttonHeight);
    lRButton.setBounds(lButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);
    rButton.setBounds(lRButton.getRight() + buttonGap, buttonTopMargin, buttonWidth, buttonHeight);

    hundredButton.setBounds(leftMargin, buttonYPosSecondRow, buttonWidth, buttonHeight);
    oneThousButton.setBounds(hundredButton.getRight() + buttonGap, buttonYPosSecondRow, buttonWidth, buttonHeight);
    tenThousButton.setBounds(oneThousButton.getRight() + buttonGap, buttonYPosSecondRow, buttonWidth, buttonHeight);
    
    minusTwentyButton.setBounds(buttonRightSideStartPos, buttonYPosSecondRow, buttonWidth, buttonHeight);
    minusTwelveButton.setBounds(minusTwentyButton.getRight() + buttonGap, buttonYPosSecondRow, buttonWidth, buttonHeight);
    minusSixButton.setBounds(minusTwelveButton.getRight() + buttonGap, buttonYPosSecondRow, buttonWidth, buttonHeight);
    
    auto borderColOneX = sineButton.getX() * 0.52;
    auto borderColTwoX = lButton.getX() * 0.95;
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
    auto olumayY = getHeight() * 0.938;
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
