/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SIGAudioProcessor::SIGAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("gain", this);
    treeState.addParameterListener("freq", this);
    treeState.addParameterListener("bypass", this);
    treeState.addParameterListener("routing", this);
}

SIGAudioProcessor::~SIGAudioProcessor()
{
    treeState.removeParameterListener("gain", this);
    treeState.removeParameterListener("freq", this);
    treeState.removeParameterListener("bypass", this);
    treeState.removeParameterListener("routing", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout SIGAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    juce::StringArray routingSelector = { "L", "L+R", "R" };
    
    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Gain", juce::NormalisableRange<float>(-120.0f, 0.0, 0.01, 1.0f), -60.0f);
    auto pFreq = std::make_unique<juce::AudioParameterFloat>("freq", "Freq", juce::NormalisableRange<float>(20.0f, 21000.0, 0.01, 0.3f), 440.0f);
    auto pBypass = std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", 0);
    auto pRoutingChoice = std::make_unique<juce::AudioParameterChoice>("routing", "Routing", routingSelector, 1);
    
    params.push_back(std::move(pGain));
    params.push_back(std::move(pFreq));
    params.push_back(std::move(pBypass));
    params.push_back(std::move(pRoutingChoice));
    
    return { params.begin(), params.end() };
}

void SIGAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load());
    treeState.getRawParameterValue("freq")->load();
    
    if(parameterID == "bypass")
    {
        bypass = newValue;
    }
    if(parameterID == "routing")
    {
        routingChoice = newValue;
    }
}

//==============================================================================
const juce::String SIGAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SIGAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SIGAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SIGAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SIGAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SIGAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SIGAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SIGAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SIGAudioProcessor::getProgramName (int index)
{
    return {};
}

void SIGAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SIGAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    osc.prepare(spec);
    gain.prepare(spec);
    
    osc.setFrequency(treeState.getRawParameterValue("freq")->load());
    gain.setGainLinear(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load()));
    
    panner.reset();
    panner.prepare(spec);
    panner.setRule(juce::dsp::PannerRule::sin3dB);
    
    routingChoice = treeState.getRawParameterValue("routing")->load();
}

void SIGAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SIGAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
#endif

void SIGAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // panner L, L+R, R choices coming from panRoutingFunc
    panner.setPan(panRoutingFunc(routingChoice));
    
    juce::dsp::AudioBlock<float> block { buffer };
    
    osc.setFrequency(*treeState.getRawParameterValue("freq"));
    gain.setGainLinear(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load()));
    
    if(bypass){}
    else
    {
        osc.process(juce::dsp::ProcessContextReplacing<float> (block));
        gain.process(juce::dsp::ProcessContextReplacing<float> (block));
    }
    
    panner.process(juce::dsp::ProcessContextReplacing<float> (block));
}

//Function returns value for panner.setPan in process block
float SIGAudioProcessor::panRoutingFunc(int choice)
{
    switch (choice)
    {
        case 0:
            return -1.0f;
            break;
        case 1:
            return 0.0f;
            break;
        case 2:
            return 1.0f;
            break;
        default:
            return 0.0f;
            break;
    }
}

//==============================================================================
bool SIGAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SIGAudioProcessor::createEditor()
{
//    return new SIGAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void SIGAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SIGAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SIGAudioProcessor();
}
