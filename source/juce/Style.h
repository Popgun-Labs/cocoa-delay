#pragma once

#include <JuceHeader.h>

namespace CocoaStyle
{
    const juce::Colour backgroundDark = juce::Colour::fromString("FF1A1A1F");
    const juce::Colour backgroundPanel = juce::Colour::fromString("FF27272E");
    const juce::Colour backgroundPurple = juce::Colour::fromString("FF4A2B4B");
    const juce::Colour accentPink = juce::Colour::fromString("FFD65D8E");
    const juce::Colour knobBody = juce::Colour::fromString("FFE8E8EC");
    const juce::Colour knobBodyDark = juce::Colour::fromString("FF4A4A52");
    const juce::Colour textWhite = juce::Colour::fromString("FFFFFFFF");
    const juce::Colour textGrey = juce::Colour::fromString("FF9A9AA0");
    const juce::Colour sidebar = juce::Colour::fromString("FF252529");

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

            // Outer shadow/ring
            g.setColour(juce::Colours::black.withAlpha(0.2f));
            g.fillEllipse(rx - 1, ry - 1, rw + 2, rw + 2);

            // Body
            g.setColour(knobBody);
            g.fillEllipse(rx, ry, rw, rw);

            // Value Arc (Pink)
            // Draw a filled arc using a thick stroke
            juce::Path valueArc;
            float arcRadius = radius * 0.75f;
            float strokeThickness = radius * 0.3f;
            
            valueArc.addCentredArc(centreX, centreY, arcRadius, arcRadius, 0.0f, rotaryStartAngle, angle, true);
            g.setColour(accentPink);
            g.strokePath(valueArc, juce::PathStrokeType(strokeThickness, juce::PathStrokeType::curved, juce::PathStrokeType::butt));

            // Dot indicator
            auto dotRadius = radius * 0.15f;
            auto dotDistance = arcRadius;
            auto dotX = centreX + dotDistance * std::cos(angle - juce::MathConstants<float>::halfPi);
            auto dotY = centreY + dotDistance * std::sin(angle - juce::MathConstants<float>::halfPi);
            
            g.setColour(accentPink);
            g.fillEllipse(dotX - dotRadius, dotY - dotRadius, dotRadius * 2.0f, dotRadius * 2.0f);
        }
        
        void drawComboBox (juce::Graphics& g, int width, int height, bool,
                                       int, int, int, int, juce::ComboBox& box) override
        {
            auto cornerSize = 2.0f;
            juce::Rectangle<int> boxBounds (0, 0, width, height);

            g.setColour (knobBody);
            g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

            // Arrow
            auto arrowW = height * 0.25f;
            auto arrowH = height * 0.25f;
            auto arrowX = width - height * 0.6f;
            auto arrowY = height * 0.5f;
            
            juce::Path path;
            path.startNewSubPath (arrowX, arrowY - arrowH);
            path.lineTo (arrowX + arrowW, arrowY);
            path.lineTo (arrowX, arrowY + arrowH);
            path.closeSubPath();
            
            g.setColour (accentPink);
            g.fillPath (path);
        }
        
        juce::Font getLabelFont (juce::Label& label) override
        {
            return juce::Font (14.0f, juce::Font::italic);
        }
        
        juce::Font getComboBoxFont (juce::ComboBox&) override
        {
            return juce::Font (15.0f);
        }

        juce::Font getPopupMenuFont() override
        {
            return juce::Font (15.0f);
        }
    };
}
