#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CocoaDelayAudioProcessorEditor::CocoaDelayAudioProcessorEditor (CocoaDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

CocoaDelayAudioProcessorEditor::~CocoaDelayAudioProcessorEditor()
{
}

//==============================================================================
void CocoaDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Cocoa Delay\n(Use Generic Editor for Parameters)", getLocalBounds(), juce::Justification::centred, 1);
}

void CocoaDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

