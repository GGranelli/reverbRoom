/*
  ==============================================================================

    RoomSourceAndListener.h
    Created: 7 Nov 2019 3:04:53pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "RoomRect.h"

//==============================================================================
/*
*/
class ButtonLookAndFeel     : public LookAndFeel_V4
{
public:
    Font getTextButtonFont(TextButton& button, int buttonHeight)
    {
        Font buttonFont("Montserrat","Regular",10.0f);
        return buttonFont;
    }
};

class RoomSourceAndListener    : public Component,
                                 public AudioProcessorValueTreeState::Listener,
                                 public Slider::Listener
{
public:
    RoomSourceAndListener(ReverbRoomAudioProcessor&,AudioProcessorValueTreeState&);
    ~RoomSourceAndListener();

    void paint (Graphics&) override;
    void resized() override;
    
    void parameterChanged (const String &parameterID, float newValue) override;
    void sliderValueChanged (Slider *slider) override;
    
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sourceHeightAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> micHeightAttachment;
    
    
private:
    TextButton sourceButton,micButton;
    Slider sourceHeightSlider,micHeightSlider;
    bool sourceFocus,micFocus={false};
    float z,sh,mh;
    
    TextEditor sourceBarH,micBarH;
    ButtonLookAndFeel buttonLook;
    ImageButton sourceLogo,micLogo;
    
    RoomRect roomR;
    
    //float to resize the rect
    float x,y,xR,yR,sx,sy,mx,my;
    
    ReverbRoomAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoomSourceAndListener)
};
