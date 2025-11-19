#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Util.h"

//==============================================================================
CocoaDelayAudioProcessor::CocoaDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    delayTimeParam = apvts.getRawParameterValue("delayTime");
    lfoAmountParam = apvts.getRawParameterValue("lfoAmount");
    lfoFrequencyParam = apvts.getRawParameterValue("lfoFrequency");
    driftAmountParam = apvts.getRawParameterValue("driftAmount");
    driftSpeedParam = apvts.getRawParameterValue("driftSpeed");
    tempoSyncTimeParam = apvts.getRawParameterValue("tempoSyncTime");
    feedbackParam = apvts.getRawParameterValue("feedback");
    stereoOffsetParam = apvts.getRawParameterValue("stereoOffset");
    panModeParam = apvts.getRawParameterValue("panMode");
    panParam = apvts.getRawParameterValue("pan");
    duckAmountParam = apvts.getRawParameterValue("duckAmount");
    duckAttackSpeedParam = apvts.getRawParameterValue("duckAttackSpeed");
    duckReleaseSpeedParam = apvts.getRawParameterValue("duckReleaseSpeed");
    filterModeParam = apvts.getRawParameterValue("filterMode");
    lowPassCutoffParam = apvts.getRawParameterValue("lowPassCutoff");
    highPassCutoffParam = apvts.getRawParameterValue("highPassCutoff");
    driveGainParam = apvts.getRawParameterValue("driveGain");
    driveMixParam = apvts.getRawParameterValue("driveMix");
    driveCutoffParam = apvts.getRawParameterValue("driveCutoff");
    driveIterationsParam = apvts.getRawParameterValue("driveIterations");
    dryVolumeParam = apvts.getRawParameterValue("dryVolume");
    wetVolumeParam = apvts.getRawParameterValue("wetVolume");
}

CocoaDelayAudioProcessor::~CocoaDelayAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout CocoaDelayAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("delayTime", "Delay Time", 0.001f, 2.0f, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("lfoAmount", "LFO Amount", 0.0f, 0.5f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("lfoFrequency", "LFO Frequency", 0.1f, 10.0f, 2.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("driftAmount", "Drift Amount", 0.0f, 0.05f, 0.001f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("driftSpeed", "Drift Speed", 0.1f, 10.0f, 1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>("tempoSyncTime", "Tempo Sync Time", 
        juce::StringArray{ "Off", "1/8D", "1/2D", "1/2", "1/2T", "1/4D", "1/4", "1/4T", "1/8D", "1/8", "1/8T", "1/16D", "1/16", "1/16T", "1/32D", "1/32", "1/32T", "1/64D", "1/64", "1/64T", "1" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", -1.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("stereoOffset", "Stereo Offset", -0.5f, 0.5f, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>("panMode", "Pan Mode", 
        juce::StringArray{ "Static", "Ping Pong", "Circular" }, 0));
        
    params.push_back(std::make_unique<juce::AudioParameterFloat>("pan", "Panning", -Util::pi * 0.5f, Util::pi * 0.5f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("duckAmount", "Ducking Amount", 0.0f, 10.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("duckAttackSpeed", "Ducking Attack", 0.1f, 100.0f, 10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("duckReleaseSpeed", "Ducking Release", 0.1f, 100.0f, 10.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>("filterMode", "Filter Mode", 
        juce::StringArray{ "1 Pole", "2 Pole", "4 Pole", "State Variable" }, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("lowPassCutoff", "Low Pass Cutoff", 0.01f, 1.0f, 0.75f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("highPassCutoff", "High Pass Cutoff", 0.001f, 0.99f, 0.001f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("driveGain", "Drive Amount", 0.0f, 10.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("driveMix", "Drive Mix", 0.0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("driveCutoff", "Drive Filter Cutoff", 0.01f, 1.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterInt>("driveIterations", "Drive Iterations", 1, 16, 1));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("dryVolume", "Dry Volume", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("wetVolume", "Wet Volume", 0.0f, 2.0f, 0.5f));

    return { params.begin(), params.end() };
}

const juce::String CocoaDelayAudioProcessor::getName() const
{
    return "Cocoa Delay";
}

bool CocoaDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CocoaDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CocoaDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CocoaDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CocoaDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CocoaDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CocoaDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CocoaDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void CocoaDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CocoaDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dt = 1.0 / sampleRate;
    InitBuffer();
    
    // Reset filters/drive if needed
    // lp.Reset(); 
    // hp.Reset();
}

void CocoaDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CocoaDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void CocoaDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Pointers to channel data
    float* channelL = buffer.getWritePointer(0);
    float* channelR = totalNumOutputChannels > 1 ? buffer.getWritePointer(1) : channelL;
    
    float* inputs[2] = { channelL, channelR };

    int nFrames = buffer.getNumSamples();

    for (int s = 0; s < nFrames; s++)
    {
        if (!warmedUp)
        {
            GetReadPositions(readPositionL, readPositionR);
            warmedUp = true;
        }

        UpdateParameters();
        UpdateReadPositions();
        
        double inputSum = inputs[0][s];
        if (totalNumOutputChannels > 1) inputSum += inputs[1][s];
        UpdateDucking(inputSum);
        
        UpdateLfo();
        UpdateDrift();

        // read from buffer
        auto outL = GetSample(bufferL, writePosition - readPositionL);
        auto outR = GetSample(bufferR, writePosition - readPositionR);

        // circular panning
        Util::adjustPanning(outL, outR, circularPanAmount, outL, outR);

        // filters
        lp.Process(dt, outL, outR, (double)*lowPassCutoffParam);
        hp.Process(dt, outL, outR, (double)*highPassCutoffParam, true);

        // drive
        auto driveAmount = (double)*driveGainParam;
        auto driveMix = (double)*driveMixParam;
        if (driveAmount > 0)
        {
            auto iterations = (int)*driveIterationsParam;
            for (int i = 0; i < iterations; i++)
            {
                outL = statefulDrive.Process(outL * driveAmount, driveMix) / driveAmount;
                outR = statefulDrive.Process(outR * driveAmount, driveMix) / driveAmount;
                driveFilter.Process(dt, outL, outR, (double)*driveCutoffParam, outL, outR);
            }
        }

        // write to buffer
        WriteToBuffer(inputs, s, outL, outR);
        UpdateWritePosition();

        // output
        auto dry = (double)*dryVolumeParam;
        auto wet = (double)*wetVolumeParam;
        auto duckValue = (double)*duckAmountParam * duckFollower;
        duckValue = duckValue > 1.0 ? 1.0 : duckValue;
        wet *= 1.0 - duckValue;
        
        inputs[0][s] = inputs[0][s] * dry + outL * wet;
        if (totalNumOutputChannels > 1)
            inputs[1][s] = inputs[1][s] * dry + outR * wet;
    }
}

//==============================================================================
juce::AudioProcessorEditor* CocoaDelayAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
    // return new CocoaDelayAudioProcessorEditor (*this);
}

bool CocoaDelayAudioProcessor::hasEditor() const
{
    return true;
}

//==============================================================================
void CocoaDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void CocoaDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================

void CocoaDelayAudioProcessor::InitBuffer()
{
    bufferL.clear();
    bufferR.clear();
    int size = (int)(getSampleRate() * tapeLength);
    if (size <= 0) size = 44100 * tapeLength; // Fallback
    
    bufferL.resize(size, 0.0);
    bufferR.resize(size, 0.0);

    writePosition = 0;
    GetReadPositions(readPositionL, readPositionR);
}

double CocoaDelayAudioProcessor::GetDelayTime()
{
    double delayTime = 0.0;
    
    // Helper for BPM
    double bpm = 120.0;
    if (auto* ph = getPlayHead())
    {
        if (auto pos = ph->getPosition())
            if (pos->getBpm().hasValue())
                bpm = *pos->getBpm();
    }
    
    auto beatLength = 60.0 / bpm;
    
    int modeIndex = (int)*tempoSyncTimeParam;
    
    // Re-map to Params::TempoSyncTimes or use switch case if order is same
    Params::TempoSyncTimes mode = (Params::TempoSyncTimes)modeIndex;

    switch (mode)
    {
    case Params::TempoSyncTimes::tempoSyncOff:
        delayTime = (double)*delayTimeParam;
        break;
    case Params::TempoSyncTimes::whole:               delayTime = beatLength * 4;    break;
    case Params::TempoSyncTimes::dottedHalf:          delayTime = beatLength * 3;    break;
    case Params::TempoSyncTimes::half:                delayTime = beatLength * 2;    break;
    case Params::TempoSyncTimes::tripletHalf:         delayTime = beatLength * 4.0/3.0;  break;
    case Params::TempoSyncTimes::dottedQuarter:       delayTime = beatLength * 3.0/2.0;  break;
    case Params::TempoSyncTimes::quarter:             delayTime = beatLength * 1;    break;
    case Params::TempoSyncTimes::tripletQuarter:      delayTime = beatLength * 2.0/3.0;  break;
    case Params::TempoSyncTimes::dottedEighth:        delayTime = beatLength * 3.0/4.0;  break;
    case Params::TempoSyncTimes::eighth:              delayTime = beatLength * 1.0/2.0;  break;
    case Params::TempoSyncTimes::tripletEighth:       delayTime = beatLength * 1.0/3.0;  break;
    case Params::TempoSyncTimes::dottedSixteenth:     delayTime = beatLength * 3.0/8.0;  break;
    case Params::TempoSyncTimes::sixteenth:           delayTime = beatLength * 1.0/4.0;  break;
    case Params::TempoSyncTimes::tripletSixteenth:    delayTime = beatLength * 1.0/6.0;  break;
    case Params::TempoSyncTimes::dottedThirtysecond:  delayTime = beatLength * 3.0/16.0; break;
    case Params::TempoSyncTimes::thirtysecond:        delayTime = beatLength * 1.0/8.0;  break;
    case Params::TempoSyncTimes::tripletThirtysecond: delayTime = beatLength * 1.0/12.0; break;
    case Params::TempoSyncTimes::dottedSixtyforth:    delayTime = beatLength * 3.0/32.0; break;
    case Params::TempoSyncTimes::sixtyforth:          delayTime = beatLength * 1.0/16.0; break;
    case Params::TempoSyncTimes::tripletSixtyforth:   delayTime = beatLength * 1.0/24.0; break;
    default: delayTime = (double)*delayTimeParam; break;
    }

    // modulation
    auto lfoAmount = (double)*lfoAmountParam;
    if (lfoAmount != 0.0) delayTime = pow(delayTime, 1.0 + lfoAmount * sin(lfoPhase * 2 * Util::pi));
    auto driftAmount = (double)*driftAmountParam;
    if (driftAmount != 0.0) delayTime = pow(delayTime, 1.0 + driftAmount * sin(driftPhase));

    return delayTime;
}

void CocoaDelayAudioProcessor::GetReadPositions(double &l, double &r)
{
    auto offset = (double)*stereoOffsetParam * .5;
    auto baseTime = GetDelayTime();
    auto timeL = pow(baseTime, 1.0 + offset);
    auto timeR = pow(baseTime, 1.0 - offset);
    l = timeL * getSampleRate();
    r = timeR * getSampleRate();
}

void CocoaDelayAudioProcessor::UpdateReadPositions()
{
    double targetReadPositionL, targetReadPositionR;
    GetReadPositions(targetReadPositionL, targetReadPositionR);
    readPositionL += (targetReadPositionL - readPositionL) * 10.0 * dt;
    readPositionR += (targetReadPositionR - readPositionR) * 10.0 * dt;
}

void CocoaDelayAudioProcessor::UpdateWritePosition()
{
    writePosition += 1;
    if (bufferL.size() > 0)
        writePosition %= bufferL.size();
}

void CocoaDelayAudioProcessor::UpdateParameters()
{
    // pan mode fadeout
    auto targetMode = (Params::PanModes)(int)*panModeParam;
    
    if (currentPanMode != targetMode)
    {
        parameterChangeVolume -= 100.0 * dt;
        if (parameterChangeVolume <= 0.0)
        {
            parameterChangeVolume = 0.0;
            currentPanMode = targetMode;
        }
    }
    else if (parameterChangeVolume < 1.0)
    {
        parameterChangeVolume += 100.0 * dt;
        if (parameterChangeVolume > 1.0) parameterChangeVolume = 1.0;
    }

    // pan amount smoothing
    auto panAmount = (double)*panParam;
    auto stationaryPanAmountTarget = (currentPanMode == Params::PanModes::stationary || currentPanMode == Params::PanModes::pingPong) ? panAmount : 0.0;
    stationaryPanAmount += (stationaryPanAmountTarget - stationaryPanAmount) * 100.0 * dt;
    auto circularPanAmountTarget = (currentPanMode == Params::PanModes::circular ? panAmount : 0.0);
    circularPanAmount += (circularPanAmountTarget - circularPanAmount) * 100.0 * dt;
    
    // Update filter mode
    auto fMode = (FilterModes)(int)*filterModeParam;
    lp.SetMode(fMode);
    hp.SetMode(fMode);
}

void CocoaDelayAudioProcessor::UpdateDucking(double input)
{
    auto attackSpeed = (double)*duckAttackSpeedParam;
    auto releaseSpeed = (double)*duckReleaseSpeedParam;
    auto speed = duckFollower < std::abs(input) ? attackSpeed : releaseSpeed;
    duckFollower += (std::abs(input) - duckFollower) * speed * dt;
}

void CocoaDelayAudioProcessor::UpdateLfo()
{
    lfoPhase += (double)*lfoFrequencyParam * dt;
    while (lfoPhase > 1.0) lfoPhase -= 1.0;
}

void CocoaDelayAudioProcessor::UpdateDrift()
{
    auto driftSpeed = (double)*driftSpeedParam;
    driftVelocity += Util::random() * 10000.0 * driftSpeed * dt;
    driftVelocity -= driftVelocity * 2.0 * sqrt(driftSpeed) * dt;
    driftPhase += driftVelocity * dt;
}

double CocoaDelayAudioProcessor::GetSample(std::vector<double> &buffer, double position)
{
    if (buffer.empty()) return 0.0;
    
    int sz = (int)buffer.size();
    int p0 = Util::wrap(floor(position) - 1, 0, sz - 1);
    int p1 = Util::wrap(floor(position), 0, sz - 1);
    int p2 = Util::wrap(ceil(position), 0, sz - 1);
    int p3 = Util::wrap(ceil(position) + 1, 0, sz - 1);

    auto x = position - floor(position);
    auto y0 = buffer[p0];
    auto y1 = buffer[p1];
    auto y2 = buffer[p2];
    auto y3 = buffer[p3];

    return Util::interpolate(x, y0, y1, y2, y3);
}

void CocoaDelayAudioProcessor::WriteToBuffer(float** inputs, int s, double outL, double outR)
{
    auto writeL = 0.0, writeR = 0.0;
    writeL += inputs[0][s];
    // Handle mono input/output cases slightly gracefully, though this assumes stereo usually
    writeR += (getTotalNumOutputChannels() > 1) ? inputs[1][s] : inputs[0][s]; 
    
    Util::adjustPanning(writeL, writeR, stationaryPanAmount * .5, writeL, writeR);
    writeL += outL * (double)*feedbackParam;
    writeR += outR * (double)*feedbackParam;
    
    if (bufferL.empty() || bufferR.empty()) return;

    switch (currentPanMode)
    {
    case Params::PanModes::pingPong:
        bufferL[writePosition] = writeR * parameterChangeVolume;
        bufferR[writePosition] = writeL * parameterChangeVolume;
        break;
    default:
        bufferL[writePosition] = writeL * parameterChangeVolume;
        bufferR[writePosition] = writeR * parameterChangeVolume;
        break;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CocoaDelayAudioProcessor();
}

