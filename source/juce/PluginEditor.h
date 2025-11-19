#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CocoaDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CocoaDelayAudioProcessorEditor (CocoaDelayAudioProcessor&);
    ~CocoaDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CocoaDelayAudioProcessor& audioProcessor;

    // Helper to create sliders/attachments could go here
    // For now, we rely on GenericAudioProcessorEditor for testing if this was a quick port,
    // but we will provide this stub so it compiles.
    // If you want a generic UI, change createEditor() in Processor to return new juce::GenericAudioProcessorEditor(*this);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CocoaDelayAudioProcessorEditor)
};

