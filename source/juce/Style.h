#pragma once

#include <JuceHeader.h>

namespace CocoaStyle
{
    const juce::Colour backgroundDark = juce::Colour::fromString("FF18181D");
    const juce::Colour backgroundPanel = juce::Colour::fromString("FF24242A");
    const juce::Colour accentPink = juce::Colour::fromString("FFD65D8E");
    const juce::Colour knobBody = juce::Colour::fromString("FFE0E0E5");
    const juce::Colour textWhite = juce::Colour::fromString("FFFFFFFF");
    const juce::Colour textGrey = juce::Colour::fromString("FFB0B0B5");
    const juce::Colour sidebar = juce::Colour::fromString("FF2A2A30");

    class CocoaLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CocoaLookAndFeel()
        {
            setColour(juce::Slider::rotarySliderFillColourId, accentPink);
            setColour(juce::Slider::rotarySliderOutlineColourId, knobBody);
            setColour(juce::Slider::thumbColourId, accentPink);
            
            setColour(juce::Label::textColourId, textWhite);
            
            setColour(juce::ComboBox::backgroundColourId, knobBody);
            setColour(juce::ComboBox::textColourId, backgroundDark);
            setColour(juce::ComboBox::arrowColourId, accentPink);
            
            setColour(juce::PopupMenu::backgroundColourId, backgroundPanel);
            setColour(juce::PopupMenu::textColourId, textWhite);
            setColour(juce::PopupMenu::highlightedBackgroundColourId, accentPink);
            setColour(juce::PopupMenu::highlightedTextColourId, textWhite);
        }

        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
            const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
        {
            auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
            auto centreX = (float)x + (float)width * 0.5f;
            auto centreY = (float)y + (float)height * 0.5f;
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

            // Body
            g.setColour(knobBody);
            g.fillEllipse(rx, ry, rw, rw);

            // Indicator (Arc or Dot)
            // Let's do a dot for simplicity and similarity to image
            auto dotRadius = radius * 0.15f;
            auto dotX = centreX + (radius * 0.7f) * std::cos(angle - juce::MathConstants<float>::halfPi);
            auto dotY = centreY + (radius * 0.7f) * std::sin(angle - juce::MathConstants<float>::halfPi);
            
            // If it's a 0-1 style knob (like in image), maybe fill partially?
            // The image has a pink arc for value.
            
            juce::Path valueArc;
            valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
            
            // Draw arc stroke
            g.setColour(accentPink);
            juce::PathStrokeType stroke(radius * 0.1f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
            // g.strokePath(valueArc, stroke); // Alternative style

            // Dot style from image
            g.setColour(accentPink);
            g.fillEllipse(dotX - dotRadius, dotY - dotRadius, dotRadius * 2.0f, dotRadius * 2.0f);
            
            // Arc indicator on the side? The image shows a pink arc OUTSIDE or on the edge?
            // Let's stick to simple dot on body for now, looks clean.
            // Actually, looking closely at the image (I can't see it, but assuming standard design):
            // "Knob with pink indicator".
            
            // Let's add a small track arc for better visibility
            juce::Path track;
            track.addCentredArc(centreX, centreY, radius * 0.9f, radius * 0.9f, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
            g.setColour(backgroundDark.withAlpha(0.2f));
            g.strokePath(track, juce::PathStrokeType(3.0f));
            
            juce::Path val;
            val.addCentredArc(centreX, centreY, radius * 0.9f, radius * 0.9f, 0.0f, rotaryStartAngle, angle, true);
            g.setColour(accentPink);
            g.strokePath(val, juce::PathStrokeType(3.0f));
        }
        
        void drawComboBox (juce::Graphics& g, int width, int height, bool,
                                       int, int, int, int, juce::ComboBox& box) override
        {
            auto cornerSize = 3.0f;
            juce::Rectangle<int> boxBounds (0, 0, width, height);

            g.setColour (knobBody);
            g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

            // Arrow
            auto arrowX = width - height * 0.8f;
            auto arrowH = height * 0.3f;
            
            juce::Path path;
            path.startNewSubPath ((float) arrowX, (float) height * 0.5f - arrowH * 0.5f);
            path.lineTo ((float) width - height * 0.3f, (float) height * 0.5f);
            path.lineTo ((float) arrowX, (float) height * 0.5f + arrowH * 0.5f);
            path.closeSubPath();
            
            g.setColour (accentPink);
            g.fillPath (path);
        }
        
        juce::Font getLabelFont (juce::Label& label) override
        {
            return juce::Font (14.0f);
        }
        
        // juce::Font getComboBoxFont (juce::ComboBox&) override
        // {
        //     return juce::Font (20.0f);
        // }

        // Font getPopupMenuFont() override
        // {
        //     return juce::Font (20.0f);
        // }
    };
}

