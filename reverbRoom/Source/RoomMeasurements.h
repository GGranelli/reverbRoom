/*
  ==============================================================================

    RoomMeasurements.h
    Created: 30 Oct 2019 3:01:31pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class RoomMeasurements    : public Component
{
public:
    RoomMeasurements(ReverbRoomAudioProcessor&, AudioProcessorValueTreeState&);
    ~RoomMeasurements();
    
    void paint (Graphics&) override;
    void resized() override;
    
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> widthSliderAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> lengthSliderAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> heightSliderAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> reflectionSliderAttachment;
    
private:
    Slider widthSlider, lengthSlider, heightSlider, reflectionSlider;
    Label widthLabel, lengthLabel, heightLabel;

    ReverbRoomAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoomMeasurements)
};
