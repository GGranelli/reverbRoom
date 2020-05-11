/*
  ==============================================================================

    ReverbGain.cpp
    Created: 6 Dec 2019 3:55:41pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ReverbGain.h"

//==============================================================================
ReverbGain::ReverbGain(ReverbRoomAudioProcessor& p,AudioProcessorValueTreeState& vts) :
processor(p),valueTreeState(vts)
{
    //pointers
    gainSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"gain",gainSlider));
    
    setSize(100, 200);
    
    gainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 25);
    gainSlider.setColour(Slider::thumbColourId, Colours::cyan);
    gainSlider.setRange(-48, 12);
    gainSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(gainSlider);

}

ReverbGain::~ReverbGain()
{
    gainSliderAttachment.reset();
}

void ReverbGain::paint (Graphics& g)
{
    Font titleFont("Montserrat","Regular",14.0f);
    
    Rectangle<int> area=getLocalBounds();
    Rectangle<int> titleArea=area.removeFromTop(20);
       
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.setFont(titleFont);
    g.drawText("Gain", titleArea, Justification::centred);
    g.setColour(Colours::darkslategrey);
    g.drawRect(area);
    g.setColour(Colours::darkslategrey);
    g.drawRect(titleArea);
}

void ReverbGain::resized()
{
    Rectangle<int> area=getLocalBounds();
    area.removeFromTop(20);
    gainSlider.setBounds(area);
}
