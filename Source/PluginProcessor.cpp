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
    treeState.addParameterListener("sine", this);
    treeState.addParameterListener("white", this);
    treeState.addParameterListener("pink", this);
    treeState.addParameterListener("l", this);
    treeState.addParameterListener("lr", this);
    treeState.addParameterListener("r", this);
    treeState.addParameterListener("hundred", this);
    treeState.addParameterListener("thousand", this);
    treeState.addParameterListener("tenThous", this);
    treeState.addParameterListener("minus twenty", this);
    treeState.addParameterListener("minus twelve", this);
    treeState.addParameterListener("minus six", this);
}

SIGAudioProcessor::~SIGAudioProcessor()
{
    treeState.removeParameterListener("gain", this);
    treeState.removeParameterListener("freq", this);
    treeState.removeParameterListener("bypass", this);
    treeState.removeParameterListener("routing", this);
    treeState.removeParameterListener("sine", this);
    treeState.removeParameterListener("white", this);
    treeState.removeParameterListener("pink", this);
    treeState.removeParameterListener("l", this);
    treeState.removeParameterListener("lr", this);
    treeState.removeParameterListener("r", this);
    treeState.removeParameterListener("hundred", this);
    treeState.removeParameterListener("thousand", this);
    treeState.removeParameterListener("tenThous", this);
    treeState.removeParameterListener("minus twenty", this);
    treeState.removeParameterListener("minus twelve", this);
    treeState.removeParameterListener("minus six", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout SIGAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain",
                                                             "Gain",
                                                             juce::NormalisableRange<float>(-120.0f, 0.0, 0.01, 1.0f),
                                                             -20.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {return (value < -10.0f) ? juce::String (value, 1) + " dB": juce::String (value, 2) + " dB";});

    auto pFreq = std::make_unique<juce::AudioParameterFloat>("freq",
                                                             "Freq",
                                                             juce::NormalisableRange<float>(20.0f, 21000.0, 0.1, 0.3f),
                                                             440.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {return (value < 1000.0) ? juce::String (value, 1) + " Hz" : juce::String (value / 1000.0f, 2) + " kHz";});
    
    auto pBypass = std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", 1);
    auto pSineChoice = std::make_unique<juce::AudioParameterBool>("sine", "Sine", 1);
    auto pWhiteChoice = std::make_unique<juce::AudioParameterBool>("white", "White", 0);
    auto pPinkChoice = std::make_unique<juce::AudioParameterBool>("pink", "Pink", 0);
    auto pLChoice = std::make_unique<juce::AudioParameterBool>("l", "L", 0);
    auto pLRChoice = std::make_unique<juce::AudioParameterBool>("lr", "LR", 1);
    auto pRChoice = std::make_unique<juce::AudioParameterBool>("r", "R", 0);
    auto pHundred = std::make_unique<juce::AudioParameterBool>("hundred", "Hundred", 0);
    auto pThousand = std::make_unique<juce::AudioParameterBool>("thousand", "Thousand", 0);
    auto pTenThousand = std::make_unique<juce::AudioParameterBool>("tenThous", "tenThous", 0);
    auto pMinusTwenty = std::make_unique<juce::AudioParameterBool>("minus twenty", "Minus Twenty", 0);
    auto pMinusTwelve = std::make_unique<juce::AudioParameterBool>("minus twelve", "Minus Twelve", 0);
    auto pMinusSix = std::make_unique<juce::AudioParameterBool>("minus six", "Minus Six", 0);
    
    params.push_back(std::move(pGain));
    params.push_back(std::move(pFreq));
    params.push_back(std::move(pBypass));
    params.push_back(std::move(pSineChoice));
    params.push_back(std::move(pWhiteChoice));
    params.push_back(std::move(pPinkChoice));
    params.push_back(std::move(pLChoice));
    params.push_back(std::move(pLRChoice));
    params.push_back(std::move(pRChoice));
    params.push_back(std::move(pHundred));
    params.push_back(std::move(pThousand));
    params.push_back(std::move(pTenThousand));
    params.push_back(std::move(pMinusTwenty));
    params.push_back(std::move(pMinusTwelve));
    params.push_back(std::move(pMinusSix));
    
    return { params.begin(), params.end() };
}

void SIGAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    gain.setTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load()));
    treeState.getRawParameterValue("freq")->load();

    if(parameterID == "bypass")
    {
        bypass = newValue;
    }
    treeState.getRawParameterValue("bypass")->load();
    
    auto sineChoice = treeState.getRawParameterValue("sine")->load();
    auto whiteChoice = treeState.getRawParameterValue("white")->load();
    auto pinkChoice = treeState.getRawParameterValue("pink")->load();
    
    if(sineChoice == 1 && whiteChoice == 0 && pinkChoice == 0)
    {
        signalType = 0;
    }
    if(sineChoice == 0 && whiteChoice == 1 && pinkChoice == 0)
    {
        signalType = 1;
    }
    if(sineChoice == 0 && whiteChoice == 0 && pinkChoice == 1)
    {
        signalType = 2;
    }
    
    auto lChoice = treeState.getRawParameterValue("l")->load();
    auto lRChoice = treeState.getRawParameterValue("lr")->load();
    auto rChoice = treeState.getRawParameterValue("r")->load();
    
    if(lChoice == 1 && lRChoice == 0 && rChoice == 0)
    {
        routingChoice = -1;
    }
    if(lChoice == 0 && lRChoice == 1 && rChoice == 0)
    {
        routingChoice = 0;
    }
    if(lChoice == 0 && lRChoice == 0 && rChoice == 1)
    {
        routingChoice = 1;
    }
    
    treeState.getRawParameterValue("hundred")->load();
    treeState.getRawParameterValue("thousand")->load();
    treeState.getRawParameterValue("tenThous")->load();
    
    treeState.getRawParameterValue("minus twenty")->load();
    treeState.getRawParameterValue("minus twelve")->load();
    treeState.getRawParameterValue("minus six")->load();
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
    
    gain.reset(sampleRate, 0.1f);
    gain.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load()));
    
    osc.prepare(spec);
    osc.setFrequency(treeState.getRawParameterValue("freq")->load());
    
    panner.reset();
    panner.prepare(spec);
    panner.setRule(juce::dsp::PannerRule::balanced); // L, L+R, R are all the same volume
        
    treeState.getRawParameterValue("sine")->load();
    treeState.getRawParameterValue("white")->load();
    treeState.getRawParameterValue("pink")->load();
    
    treeState.getRawParameterValue("l")->load();
    treeState.getRawParameterValue("lr")->load();
    treeState.getRawParameterValue("r")->load();
    
    treeState.getRawParameterValue("hundred")->load();
    treeState.getRawParameterValue("thousand")->load();
    treeState.getRawParameterValue("tenThous")->load();
    
    treeState.getRawParameterValue("minus twenty")->load();
    treeState.getRawParameterValue("minus twelve")->load();
    treeState.getRawParameterValue("minus six")->load();
    
    bypass = treeState.getRawParameterValue("bypass")->load();
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
    
    //Target value of gain coming from gain slider
    gain.setTargetValue(juce::Decibels::decibelsToGain(treeState.getRawParameterValue("gain")->load()));
    
    auto lChoice = treeState.getRawParameterValue("l")->load();
    auto lRChoice = treeState.getRawParameterValue("lr")->load();
    auto rChoice = treeState.getRawParameterValue("r")->load();
    
    if(lChoice == 1 && lRChoice == 0 && rChoice == 0)
    {
        routingChoice = 0;
    }
    if(lChoice == 0 && lRChoice == 1 && rChoice == 0)
    {
        routingChoice = 1;
    }
    if(lChoice == 0 && lRChoice == 0 && rChoice == 1)
    {
        routingChoice = 2;
    }
    
    // panner L, L+R, R choices coming from panRoutingFunc
    panner.setPan(panRoutingFunc(routingChoice));
    
    //oscillator frequency
    osc.setFrequency(*treeState.getRawParameterValue("freq"));

    //My dsp object
    juce::dsp::AudioBlock<float> block { buffer };
    
    auto sineChoice = treeState.getRawParameterValue("sine")->load();
    auto whiteChoice = treeState.getRawParameterValue("white")->load();
    auto pinkChoice = treeState.getRawParameterValue("pink")->load();
    
    if(sineChoice == 1 && whiteChoice == 0 && pinkChoice == 0)
    {
        signalType = 0;
    }
    if(sineChoice == 0 && whiteChoice == 1 && pinkChoice == 0)
    {
        signalType = 1;
    }
    if(sineChoice == 0 && whiteChoice == 0 && pinkChoice == 1)
    {
        signalType = 2;
    }
    
    //bypass if statement
    if(!bypass){} // if true, do nothing
    else //if false process osc, white, pink depending on signalType chosen
    {
        switch (signalType)
        {
            case 0: oscProcess(buffer); break;
            case 1: whiteNoiseProcess(buffer); break;
            case 2: pinkNoiseProcess(buffer); break;
            default: oscProcess(buffer); break;
        }
        panner.process(juce::dsp::ProcessContextReplacing<float> (block));
    }
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
//Function for oscillator processing
void SIGAudioProcessor::oscProcess(juce::AudioBuffer<float> &buffer)
{
    auto block = juce::dsp::AudioBlock<float> (buffer);
    
    osc.process(juce::dsp::ProcessContextReplacing<float> (block));
        
    for(int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for(int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            channelData[sample] *= gain.getNextValue();
        }
    }
}

//Function for white noise processing
void SIGAudioProcessor::whiteNoiseProcess(juce::AudioBuffer<float> &buffer)
{
    auto block = juce::dsp::AudioBlock<float> (buffer);
    
    for(int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for(int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            //channelData[sample] = ((float)rand()/RAND_MAX) * 2.0f - 1.0f; // white noise from a forum post - need something to compare this to
            channelData[sample] = random.nextFloat(); // from JUCE tutorial, seems the same as above but a bit quieter
            channelData[sample] *= gain.getNextValue();
        }
    }
}

//Function for pink noise processing
void SIGAudioProcessor::pinkNoiseProcess(juce::AudioBuffer<float> &buffer)
{
    auto block = juce::dsp::AudioBlock<float> (buffer);
    
    for(int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);

        for(int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            channelData[sample] = pn.GetNextValue();
            channelData[sample] *= gain.getNextValue() * 0.00849; //dropping pink noise output...check pink noise class for reason it is outputting such a high value
        }
    }
}

//==============================================================================
bool SIGAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SIGAudioProcessor::createEditor()
{
    return new SIGAudioProcessorEditor (*this);
//    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void SIGAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream(stream);
}

void SIGAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
        
    if(tree.isValid())
    {
        treeState.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SIGAudioProcessor();
}
