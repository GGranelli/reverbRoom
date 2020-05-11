/*
  ==============================================================================

    About.cpp
    Created: 14 Mar 2020 5:24:59pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#include <JuceHeader.h>
#include "About.h"

//==============================================================================
About::About()
{
    addAndMakeVisible(logoImg);
    logoImg.setImage(ImageFileFormat::loadFrom(BinaryData::Logo_Small_White_png, BinaryData::Logo_Small_White_pngSize));
    logoImg.setSize(20, 20);
}

About::~About()
{
}

void About::paint (Graphics& g)
{
    Font textFont("Montserrat","Regular",9.0f);
        
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.setFont(textFont);
    g.drawText("Coded at LIM", 4, 8, 76, 10, Justification::left);
    g.drawText("by G.Granelli", 4, 18, 76, 10, Justification::left);
    g.drawText("2020", 4, 28, 76, 10, Justification::left);
}

void About::resized()
{
    Rectangle<float> logoArea;
    logoArea.setBounds(64, 6, 32, 32);
    logoImg.setBoundingBox(logoArea);
}
