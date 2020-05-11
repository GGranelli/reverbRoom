/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbRoomAudioProcessorEditor::ReverbRoomAudioProcessorEditor (ReverbRoomAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), roomValuesGui(p,vts), roomSnLGui(p,vts), gainGui(p,vts), aboutGui(), processor (p), valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(false, false);
    setSize(515, 635);
    
    addAndMakeVisible(&roomValuesGui);
    addAndMakeVisible(&roomSnLGui);
    addAndMakeVisible(&gainGui);
    addAndMakeVisible(&aboutGui);
}

ReverbRoomAudioProcessorEditor::~ReverbRoomAudioProcessorEditor()
{
}

//==============================================================================
void ReverbRoomAudioProcessorEditor::paint (Graphics& g)
{
    Rectangle<int> titleArea (0,0,getWidth(),50);
    
    Font titleFont("Montserrat","Medium",45.0f);
    
    g.fillAll(Colours::darkslategrey);
    g.setColour(Colours::darkslategrey);
    g.fillRect(titleArea);
    g.setColour(Colours::cyan);
    g.setFont(titleFont);
    g.drawText("REVERB ROOM", titleArea, Justification::centred);
}

void ReverbRoomAudioProcessorEditor::resized()
{
    auto area=getLocalBounds();
    Rectangle<int> titleArea;
    
    titleArea=area.removeFromTop(50);

    //aumento il roomRect di 30
    roomValuesGui.setBounds(5,55,400,205);
    roomSnLGui.setBounds(5,265,400,365);
    gainGui.setBounds(410,55,100,520);
    aboutGui.setBounds(410,580,100,50);
}


