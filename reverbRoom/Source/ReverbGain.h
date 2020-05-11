/*
  ==============================================================================

    ReverbGain.h
    Created: 6 Dec 2019 3:55:41pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ReverbGain    : public Component
{
public:
    ReverbGain(ReverbRoomAudioProcessor&,AudioProcessorValueTreeState&);
    ~ReverbGain();

    void paint (Graphics&) override;
    void resized() override;
    
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;

private:
    Slider gainSlider;
    
    ReverbRoomAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbGain)
};
