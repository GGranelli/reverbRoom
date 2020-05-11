/*
  ==============================================================================

    RoomRect.h
    Created: 14 Nov 2019 5:14:42pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class TextEditorLookAndFeel     : public LookAndFeel_V4
{
public:
    TextEditorLookAndFeel()
    {
        setColour(TextEditor::backgroundColourId, Colours::transparentBlack);
        setColour(TextEditor::outlineColourId, Colours::transparentBlack);
    }
};
class RoomRect    : public Component,
                    public DragAndDropContainer,
                    public AudioProcessorValueTreeState::Listener,
                    public Slider::Listener
{
public:
    RoomRect(ReverbRoomAudioProcessor&,AudioProcessorValueTreeState&);
    ~RoomRect();

    void paint (Graphics&) override;
    void resized() override;
    
    void paintContent(Graphics& g,int w,int h) ;
    
    //DragAndDropEvents
    void mouseDrag (const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;
    
    void parameterChanged (const String &parameterID, float newValue) override;
    void sliderValueChanged (Slider *slider) override;
    
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sourceXAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> sourceYAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> micXAttachment;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> micYAttachment;
    
    //public per essere utilizzate dal padre
    TextEditor sourceBarW,sourceBarL,micBarW,micBarL;
    Image sourceImg,micImg;
    TextEditorLookAndFeel textEditorLookAndFeel;
    
private:
    ImageButton source,mic;
    Slider sourceXSlider,sourceYSlider,micXSlider,micYSlider;
    
    float rectWidth,rectHeight,roomWidth,roomLength,roomHeight,sourceHeight,micHeight;
    int sourceSize,micSize;
    bool sourceFocus={false};
    bool micFocus={false};
    float sx,sy;

    ReverbRoomAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoomRect)
};
