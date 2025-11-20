#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CocoaDelayAudioProcessorEditor::CocoaDelayAudioProcessorEditor (CocoaDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&cocoaLookAndFeel);

    // Helper to add sliders
    auto addKnob = [&](juce::Slider& slider, std::unique_ptr<SliderAttachment>& attachment, juce::String paramId, juce::String name)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 16);
        attachment.reset(new SliderAttachment(audioProcessor.apvts, paramId, slider));
        
        auto comp = std::make_unique<ParameterComponent>(name, &slider);
        addAndMakeVisible(comp.get());
        paramComponents.push_back(std::move(comp));
    };
    
    auto addCombo = [&](juce::ComboBox& combo, std::unique_ptr<ComboBoxAttachment>& attachment, juce::String paramId, juce::String name)
    {
        // Manually populate items to ensure they appear
        if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(audioProcessor.apvts.getParameter(paramId)))
        {
            combo.addItemList(choiceParam->choices, 1);
        }
        
        // Items are populated by attachment
        attachment.reset(new ComboBoxAttachment(audioProcessor.apvts, paramId, combo));
        
        auto comp = std::make_unique<ParameterComponent>(name, &combo);
        addAndMakeVisible(comp.get());
        paramComponents.push_back(std::move(comp));
    };

    // DELAY
    addKnob(delayTimeSlider, delayTimeAttachment, "delayTime", "Time");
    addCombo(tempoSyncCombo, tempoSyncAttachment, "tempoSyncTime", "Tempo sync");
    
    // LFO
    addKnob(lfoAmountSlider, lfoAmountAttachment, "lfoAmount", "Amount");
    addKnob(lfoFreqSlider, lfoFreqAttachment, "lfoFrequency", "Frequency");
    
    // DRIFT
    addKnob(driftAmountSlider, driftAmountAttachment, "driftAmount", "Amount");
    addKnob(driftSpeedSlider, driftSpeedAttachment, "driftSpeed", "Speed");
    
    // FEEDBACK
    addKnob(feedbackSlider, feedbackAttachment, "feedback", "Amount");
    addKnob(stereoOffsetSlider, stereoOffsetAttachment, "stereoOffset", "Stereo");
    addKnob(panSlider, panAttachment, "pan", "Pan");
    addCombo(panModeCombo, panModeAttachment, "panMode", "Pan mode");
    
    // DUCKING
    addKnob(duckAmountSlider, duckAmountAttachment, "duckAmount", "Amount");
    addKnob(duckAttackSlider, duckAttackAttachment, "duckAttackSpeed", "Attack");
    addKnob(duckReleaseSlider, duckReleaseAttachment, "duckReleaseSpeed", "Release");
    
    // FILTER
    addCombo(filterModeCombo, filterModeAttachment, "filterMode", "Mode");
    addKnob(lowCutSlider, lowCutAttachment, "lowPassCutoff", "Low cut");
    addKnob(highCutSlider, highCutAttachment, "highPassCutoff", "High cut");
    
    // DRIVE
    addKnob(driveGainSlider, driveGainAttachment, "driveGain", "Gain");
    addKnob(driveMixSlider, driveMixAttachment, "driveMix", "Mix");
    addKnob(driveCutoffSlider, driveCutoffAttachment, "driveCutoff", "Filter");
    addKnob(driveIterationsSlider, driveIterationsAttachment, "driveIterations", "Iterations");
    
    // SIDEBAR (Dry/Wet)
    addKnob(drySlider, dryAttachment, "dryVolume", "Dry");
    addKnob(wetSlider, wetAttachment, "wetVolume", "Wet");

    setSize (900, 430);
}

CocoaDelayAudioProcessorEditor::~CocoaDelayAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void CocoaDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (CocoaStyle::backgroundDark);
    
    // Sidebar
    auto sidebarWidth = 140;
    juce::Rectangle<int> sidebar(0, 0, sidebarWidth, getHeight());
    g.setColour(CocoaStyle::sidebar);
    g.fillRect(sidebar);
    
    // Top Right Gradient - Subtle purple glow
    juce::Rectangle<int> mainArea(sidebarWidth, 0, getWidth() - sidebarWidth, getHeight());
    
    // Top Section Background - Distinct Purple Panel
    // The top section (Delay/LFO/Drift) has a solid purple background
    // It spans the full width of the main area and height approx 140
    juce::Rectangle<int> topPanelRect(sidebarWidth, 0, getWidth() - sidebarWidth, 140);
    g.setColour(CocoaStyle::backgroundPurple.withAlpha(0.5f)); // Solid purple with some transparency or mixing
    g.fillRect(topPanelRect);

    // Subtle gradient for the rest?
    // Actually, let's just stick to the solid backgroundDark for the bottom and the purple for the top
    // But maybe blend it down?
    // The reference shows a sharp-ish distinction or at least a clear "top block".
    
    // Let's draw a gradient overlay on the whole thing to tie it together
    juce::ColourGradient linearGrad(
        CocoaStyle::backgroundPurple.withAlpha(0.1f), (float)getWidth(), 0.0f,
        CocoaStyle::backgroundDark, (float)sidebarWidth, (float)getHeight(),
        false);
        
    g.setGradientFill(linearGrad);
    g.fillRect(mainArea);
    
    // Sidebar Text
    g.setColour(CocoaStyle::textWhite);
    g.setFont(24.0f);
    
    // Draw vertical text
    auto centerX = sidebarWidth / 2.0f;
    auto centerY = getHeight() / 2.0f;
    auto transform = juce::AffineTransform::rotation(-juce::MathConstants<float>::halfPi, centerX, centerY);
    g.addTransform(transform);
    g.drawText("COCOA DELAY", centerY - 100, centerX - 25, 200, 50, juce::Justification::centred);
    g.addTransform(transform.inverted());
    
    // Draw Section Headers
    g.setColour(CocoaStyle::textWhite);
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    
    // Positions
    int topRowY = 10;
    int midRowY = 150;
    int botRowY = 290;
    
    int mainX = sidebarWidth;
    int sectionW = (getWidth() - sidebarWidth) / 3;
    
    g.drawText("DELAY", mainX, topRowY, sectionW, 30, juce::Justification::centred);
    g.drawText("LFO", mainX + sectionW, topRowY, sectionW / 2 + 20, 30, juce::Justification::centred);
    g.drawText("DRIFT", mainX + sectionW * 1.8f, topRowY, sectionW, 30, juce::Justification::centred);
    
    g.drawText("FEEDBACK", mainX, midRowY, sectionW * 1.5f, 30, juce::Justification::centred);
    g.drawText("DUCKING", mainX + sectionW * 1.8f, midRowY, sectionW, 30, juce::Justification::centred);
    
    g.drawText("FILTER", mainX, botRowY, sectionW, 30, juce::Justification::centred);
    g.drawText("DRIVE", mainX + sectionW * 1.5f, botRowY, sectionW, 30, juce::Justification::centred);
    
    // Draw version
    g.setColour(CocoaStyle::textGrey);
    g.setFont(12.0f);
    g.drawText("v1.0", 0, getHeight() - 30, sidebarWidth, 20, juce::Justification::centred);
}

void CocoaDelayAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto sidebar = area.removeFromLeft(140);
    
    // Sidebar Mix controls
    auto mixArea = sidebar.removeFromBottom(280);
    mixArea.removeFromBottom(40); // Space for version
    
    // Assuming paramComponents order:
    // Delay(2), LFO(2), Drift(2), Feedback(4), Ducking(3), Filter(3), Drive(4), Dry(1), Wet(1)
    // Total 22
    
    // Indices:
    // 0-1: Delay
    // 2-3: LFO
    // 4-5: Drift
    // 6-9: Feedback
    // 10-12: Ducking
    // 13-15: Filter
    // 16-19: Drive
    // 20: Dry
    // 21: Wet
    
    if (paramComponents.size() != 22) return; // Safety check
    
    paramComponents[21]->setBounds(mixArea.removeFromBottom(120).reduced(20)); // Wet
    paramComponents[20]->setBounds(mixArea.removeFromBottom(120).reduced(20)); // Dry
    
    // Main Grid
    auto topRow = area.removeFromTop(140);
    auto midRow = area.removeFromTop(140);
    auto botRow = area.removeFromTop(140);
    
    // Top Row: Delay (2), LFO (2), Drift (2)
    // Layout: [Time][Sync]  [Amt][Freq]  [Amt][Speed]
    int cellW = 80;
    int margin = 12;
    int sidebarWidth = 140;
    int x = sidebarWidth + 25;

    int comboHeight = 50;
    int comboWidth = 110;
    
    topRow.removeFromTop(50); // Header space
    
    paramComponents[0]->setBounds(x, topRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[1]->setBounds(x, topRow.getY() + 15, comboWidth, comboHeight); x += comboWidth + margin + 15;
    
    paramComponents[2]->setBounds(x, topRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[3]->setBounds(x, topRow.getY(), cellW, 80); x += cellW + margin + 20;
    
    paramComponents[4]->setBounds(x, topRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[5]->setBounds(x, topRow.getY(), cellW, 80);
    
    // Mid Row: Feedback (4), Ducking (3)
    x = sidebarWidth + 25;
    midRow.removeFromTop(50);
    
    paramComponents[6]->setBounds(x, midRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[7]->setBounds(x, midRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[8]->setBounds(x, midRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[9]->setBounds(x, midRow.getY() + 15, comboWidth, comboHeight); x += comboWidth + margin + 30;
    
    paramComponents[10]->setBounds(x, midRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[11]->setBounds(x, midRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[12]->setBounds(x, midRow.getY(), cellW, 80);
    
    // Bot Row: Filter (3), Drive (4)
    x = sidebarWidth + 25;
    botRow.removeFromTop(50);
    
    paramComponents[13]->setBounds(x, botRow.getY() + 15, comboWidth, comboHeight); x += comboWidth + margin;
    paramComponents[14]->setBounds(x, botRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[15]->setBounds(x, botRow.getY(), cellW, 80); x += cellW + margin + 30;
    
    paramComponents[16]->setBounds(x, botRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[17]->setBounds(x, botRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[18]->setBounds(x, botRow.getY(), cellW, 80); x += cellW + margin;
    paramComponents[19]->setBounds(x, botRow.getY(), cellW, 80);
}
