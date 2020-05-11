/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "RoomMeasurements.h"
#include "RoomSourceAndListener.h"
#include "RoomRect.h"
#include "ReverbGain.h"
#include "About.h"

//==============================================================================
/**
*/
class ReverbRoomAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ReverbRoomAudioProcessorEditor (ReverbRoomAudioProcessor&,
                                    AudioProcessorValueTreeState&);
    ~ReverbRoomAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
private:
    RoomMeasurements roomValuesGui;
    RoomSourceAndListener roomSnLGui;
    ReverbGain gainGui;
    About aboutGui;
    
    ReverbRoomAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    float x,y;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbRoomAudioProcessorEditor)
};
