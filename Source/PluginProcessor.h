/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PinkNoise.h"

//==============================================================================
/**
*/
class SIGAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    SIGAudioProcessor();
    ~SIGAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState treeState;
private:
    
    juce::Random random;
    
    //juce dsp instantiations
    juce::dsp::Oscillator<float> osc { [](float x) { return std::sin (x); }, 200 }; // the 200 is lookup table value - not sure what that is but it makes it more efficient??
    juce::LinearSmoothedValue<float> gain { 0.0f };
    juce::dsp::Panner<float> panner;
    
    // return std::sin (x);  // sine wave
    // return x / juce::MathConstants<float>::pi;  // saw wave
    // return x < 0.0f ? -1.0f : 1.0f;   // square wave
    
    // variable instantiations
    float freq { 440.0f };
    bool bypass { false };
    int routingChoice { 1 };
    int signalType { 0 };
    
    //Functions for dsp
    float panRoutingFunc(int choice);
    void oscProcess(juce::AudioBuffer<float> &buffer);
    void whiteNoiseProcess(juce::AudioBuffer<float> &buffer);
    void pinkNoiseProcess(juce::AudioBuffer<float> &buffer);
    
    //Functions for param layout and changes
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    
    //Pink noise class // from here https://www.firstpr.com.au/dsp/pink-noise/#Voss
    
    //Pink noise object instantiation
    PinkNumber pn;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SIGAudioProcessor)
};
