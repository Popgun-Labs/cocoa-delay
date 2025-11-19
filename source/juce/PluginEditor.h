#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Style.h"

// Helper class for labelled sections
class ParameterComponent : public juce::Component
{
public:
    ParameterComponent(juce::String name, juce::Component* control)
    {
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
        addAndMakeVisible(control);
        this->control = control;
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds();
        label.setBounds(bounds.removeFromTop(20));
        control->setBounds(bounds.reduced(5));
    }
    
private:
    juce::Label label;
    juce::Component* control;
};

class CocoaDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CocoaDelayAudioProcessorEditor (CocoaDelayAudioProcessor&);
    ~CocoaDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CocoaDelayAudioProcessor& audioProcessor;
    CocoaStyle::CocoaLookAndFeel cocoaLookAndFeel;

    // Attachments
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

    // Components
    // -- Delay --
    juce::Slider delayTimeSlider;
    juce::ComboBox tempoSyncCombo;
    std::unique_ptr<SliderAttachment> delayTimeAttachment;
    std::unique_ptr<ComboBoxAttachment> tempoSyncAttachment;
    
    // -- LFO --
    juce::Slider lfoAmountSlider;
    juce::Slider lfoFreqSlider;
    std::unique_ptr<SliderAttachment> lfoAmountAttachment;
    std::unique_ptr<SliderAttachment> lfoFreqAttachment;
    
    // -- Drift --
    juce::Slider driftAmountSlider;
    juce::Slider driftSpeedSlider;
    std::unique_ptr<SliderAttachment> driftAmountAttachment;
    std::unique_ptr<SliderAttachment> driftSpeedAttachment;
    
    // -- Feedback --
    juce::Slider feedbackSlider;
    juce::Slider stereoOffsetSlider;
    juce::Slider panSlider;
    juce::ComboBox panModeCombo;
    std::unique_ptr<SliderAttachment> feedbackAttachment;
    std::unique_ptr<SliderAttachment> stereoOffsetAttachment;
    std::unique_ptr<SliderAttachment> panAttachment;
    std::unique_ptr<ComboBoxAttachment> panModeAttachment;
    
    // -- Ducking --
    juce::Slider duckAmountSlider;
    juce::Slider duckAttackSlider;
    juce::Slider duckReleaseSlider;
    std::unique_ptr<SliderAttachment> duckAmountAttachment;
    std::unique_ptr<SliderAttachment> duckAttackAttachment;
    std::unique_ptr<SliderAttachment> duckReleaseAttachment;
    
    // -- Filter --
    juce::ComboBox filterModeCombo;
    juce::Slider lowCutSlider;
    juce::Slider highCutSlider;
    std::unique_ptr<ComboBoxAttachment> filterModeAttachment;
    std::unique_ptr<SliderAttachment> lowCutAttachment;
    std::unique_ptr<SliderAttachment> highCutAttachment;
    
    // -- Drive --
    juce::Slider driveGainSlider;
    juce::Slider driveMixSlider;
    juce::Slider driveCutoffSlider;
    juce::Slider driveIterationsSlider; // Integer parameter, but Slider is fine
    std::unique_ptr<SliderAttachment> driveGainAttachment;
    std::unique_ptr<SliderAttachment> driveMixAttachment;
    std::unique_ptr<SliderAttachment> driveCutoffAttachment;
    std::unique_ptr<SliderAttachment> driveIterationsAttachment;
    
    // -- Mix (Sidebar) --
    juce::Slider drySlider;
    juce::Slider wetSlider;
    std::unique_ptr<SliderAttachment> dryAttachment;
    std::unique_ptr<SliderAttachment> wetAttachment;

    // Labels mapping
    std::vector<std::unique_ptr<ParameterComponent>> paramComponents;
    
    void addKnob(juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, const juce::String& paramId, const juce::String& name);
    void addCombo(juce::ComboBox& combo, std::unique_ptr<ComboBoxAttachment>& attachment, const juce::String& paramId, const juce::String& name);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CocoaDelayAudioProcessorEditor)
};
