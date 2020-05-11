/*
  ==============================================================================

    RoomMeasurements.cpp
    Created: 30 Oct 2019 3:01:31pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RoomMeasurements.h"

//==============================================================================
RoomMeasurements::RoomMeasurements(ReverbRoomAudioProcessor& p,AudioProcessorValueTreeState& vts) :
processor(p),valueTreeState(vts)
{
    Font labelFont("Montserrat","Regular",14.0f);

    //pointers
    widthSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"width",widthSlider));
    lengthSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"length",lengthSlider));
    heightSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"height",heightSlider));
    reflectionSliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"reflection",reflectionSlider));
    
    
    setSize (400, 200);
    
    addAndMakeVisible(&widthSlider);
    widthSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    widthSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 60, 25);
    widthSlider.setNumDecimalPlacesToDisplay(2);
    widthSlider.setTextValueSuffix(" m");
    widthSlider.setColour(Slider::thumbColourId, Colours::cyan);
    
    addAndMakeVisible(widthLabel);
    widthLabel.setText("Width", dontSendNotification);
    widthLabel.setFont(labelFont);
    
    addAndMakeVisible(&lengthSlider);
    lengthSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    lengthSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 60, 25);
    lengthSlider.setNumDecimalPlacesToDisplay(2);
    lengthSlider.setTextValueSuffix(" m");
    lengthSlider.setColour(Slider::thumbColourId, Colours::cyan);
    
    addAndMakeVisible(lengthLabel);
    lengthLabel.setText("Length", dontSendNotification);
    lengthLabel.setFont(labelFont);
    
    addAndMakeVisible(&heightSlider);
    heightSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    heightSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 60, 25);
    heightSlider.setNumDecimalPlacesToDisplay(2);
    heightSlider.setTextValueSuffix(" m");
    heightSlider.setColour(Slider::thumbColourId, Colours::cyan);
    
    addAndMakeVisible(heightLabel);
    heightLabel.setText("Height", dontSendNotification);
    heightLabel.setFont(labelFont);
    
    addAndMakeVisible(&reflectionSlider);
    reflectionSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    reflectionSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 25);
    reflectionSlider.setNumDecimalPlacesToDisplay(2);
    reflectionSlider.setColour(Slider::thumbColourId, Colours::cyan);
}

RoomMeasurements::~RoomMeasurements()
{
    widthSliderAttachment.reset();
    lengthSliderAttachment.reset();
    heightSliderAttachment.reset();
    reflectionSliderAttachment.reset();
}

void RoomMeasurements::paint (Graphics& g)
{
    Font titleFont("Montserrat","Regular",14.0f);
    
    Rectangle<int> area=getLocalBounds();
    Rectangle<int> reflectionArea=area.removeFromRight(area.getWidth()/3+10);
    Rectangle<int> titleArea1=area.removeFromTop(20);
    Rectangle<int> titleArea2=reflectionArea.removeFromTop(20);
       
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.setFont(titleFont);
    g.drawText("Room Measurements", titleArea1, Justification::centred);
    g.drawText("Reflection Coefficient", titleArea2, Justification::centred);
    g.setColour(Colours::darkslategrey);
    g.drawRect(area);
    g.setColour(Colours::darkslategrey);
    g.drawRect(titleArea1);
    g.setColour(Colours::darkslategrey);
    g.drawRect(titleArea2);
    g.setColour(Colours::darkslategrey);
    g.drawRect(reflectionArea);
}

void RoomMeasurements::resized()
{
    Rectangle<int> area=getLocalBounds();
    area.removeFromTop(20);
    Rectangle<int> reflectionArea=area.removeFromRight(area.getWidth()/3+10);
    area.removeFromLeft(20);
    area.removeFromTop(10);
    reflectionArea.reduce(20, 20);
    
    const float border = 10.0f;
    
    float sliderHeight = area.getHeight()/4;
    float labelsWidth = area.getWidth()/4;
    float sliderWidth = area.getWidth()/4*3-10;
    
    reflectionSlider.setBounds(reflectionArea);
    
    //======================================================//
    
    FlexBox widthFlexbox;
    widthFlexbox.flexDirection=FlexBox::Direction::row;
    widthFlexbox.flexWrap=FlexBox::Wrap::wrap;
    widthFlexbox.alignContent=FlexBox::AlignContent::center;
    widthFlexbox.alignItems=FlexBox::AlignItems::center;
    
    Array<FlexItem> widthArray;
    widthArray.add(FlexItem(labelsWidth, sliderHeight, widthLabel));
    widthArray.add(FlexItem(sliderWidth, sliderHeight, widthSlider));
    
    widthFlexbox.items=widthArray;
    widthFlexbox.performLayout(area.removeFromTop(sliderHeight+border));
    
    //======================================================//

    FlexBox lengthFlexbox;
    lengthFlexbox.flexDirection=FlexBox::Direction::row;
    lengthFlexbox.flexWrap=FlexBox::Wrap::wrap;
    lengthFlexbox.alignContent=FlexBox::AlignContent::center;
    lengthFlexbox.alignItems=FlexBox::AlignItems::center;
    
    Array<FlexItem> lengthArray;
    lengthArray.add(FlexItem(labelsWidth, sliderHeight, lengthLabel));
    lengthArray.add(FlexItem(sliderWidth, sliderHeight, lengthSlider));
    
    lengthFlexbox.items=lengthArray;
    lengthFlexbox.performLayout(area.removeFromTop(sliderHeight+border));
    
    //======================================================//

    FlexBox heightFlexbox;
    heightFlexbox.flexDirection=FlexBox::Direction::row;
    heightFlexbox.flexWrap=FlexBox::Wrap::wrap;
    heightFlexbox.alignContent=FlexBox::AlignContent::center;
    heightFlexbox.alignItems=FlexBox::AlignItems::center;
    
    Array<FlexItem> heightArray;
    heightArray.add(FlexItem(labelsWidth, sliderHeight, heightLabel));
    heightArray.add(FlexItem(sliderWidth, sliderHeight, heightSlider));
    
    heightFlexbox.items=heightArray;
    heightFlexbox.performLayout(area.removeFromTop(sliderHeight+border));
}
