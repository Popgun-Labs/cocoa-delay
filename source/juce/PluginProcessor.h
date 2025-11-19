#pragma once

#include <JuceHeader.h>
#include "Filter.h"
#include "StatefulDrive.h"
#include "Parameters.h"
#include <vector>

class CocoaDelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    CocoaDelayAudioProcessor();
    ~CocoaDelayAudioProcessor() override;

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

    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void InitBuffer();
    void UpdateReadPositions();
    void UpdateWritePosition();
    void UpdateParameters();
    void UpdateDucking(double input);
    void UpdateLfo();
    void UpdateDrift();
    double GetDelayTime();
    void GetReadPositions(double & l, double & r);
    double GetSample(std::vector<double> &buffer, double position);
    void WriteToBuffer(float** inputs, int s, double outL, double outR);

    static const int tapeLength = 10;

    // delay
    std::vector<double> bufferL;
    std::vector<double> bufferR;
    int writePosition;
    double readPositionL;
    double readPositionR;
    bool warmedUp = false;
    double dt = 0.0;

    // fading parameters
    Params::PanModes currentPanMode = Params::PanModes::stationary;
    double parameterChangeVolume = 1.0;
    double stationaryPanAmount = 0.0;
    double circularPanAmount = 0.0;

    // filters
    MultiFilter lp;
    MultiFilter hp;

    // drive
    StatefulDrive statefulDrive;
    DualFilter<TwoPoleFilter> driveFilter;

    // modulation
    double duckFollower = 0.0;
    double lfoPhase = 0.0;
    double driftVelocity = 0.0;
    double driftPhase = 0.0;

    // Parameter pointers
    std::atomic<float>* delayTimeParam = nullptr;
    std::atomic<float>* lfoAmountParam = nullptr;
    std::atomic<float>* lfoFrequencyParam = nullptr;
    std::atomic<float>* driftAmountParam = nullptr;
    std::atomic<float>* driftSpeedParam = nullptr;
    std::atomic<float>* tempoSyncTimeParam = nullptr;
    std::atomic<float>* feedbackParam = nullptr;
    std::atomic<float>* stereoOffsetParam = nullptr;
    std::atomic<float>* panModeParam = nullptr;
    std::atomic<float>* panParam = nullptr;
    std::atomic<float>* duckAmountParam = nullptr;
    std::atomic<float>* duckAttackSpeedParam = nullptr;
    std::atomic<float>* duckReleaseSpeedParam = nullptr;
    std::atomic<float>* filterModeParam = nullptr;
    std::atomic<float>* lowPassCutoffParam = nullptr;
    std::atomic<float>* highPassCutoffParam = nullptr;
    std::atomic<float>* driveGainParam = nullptr;
    std::atomic<float>* driveMixParam = nullptr;
    std::atomic<float>* driveCutoffParam = nullptr;
    std::atomic<float>* driveIterationsParam = nullptr;
    std::atomic<float>* dryVolumeParam = nullptr;
    std::atomic<float>* wetVolumeParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CocoaDelayAudioProcessor)
};

