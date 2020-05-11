/*
  ==============================================================================

    RoomSourceAndListener.cpp
    Created: 7 Nov 2019 3:04:53pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RoomSourceAndListener.h"

//==============================================================================
RoomSourceAndListener::RoomSourceAndListener(ReverbRoomAudioProcessor& p,AudioProcessorValueTreeState& vts) :
roomR(p,vts),processor(p),valueTreeState(vts)
{
    Font buttonFont("Montserrat","Regular",8.0f);
    Font boxFont("Montserrat","Regular",12.0f);
    
    //pointers
    sourceHeightAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"sourceHeight",sourceHeightSlider));
    micHeightAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"micHeight",micHeightSlider));
    
    
    setSize(400,200);
    
    addAndMakeVisible(&roomR);
    addAndMakeVisible(&roomR.sourceBarW);
    addAndMakeVisible(&roomR.sourceBarL);
    addAndMakeVisible(&roomR.micBarW);
    addAndMakeVisible(&roomR.micBarL);
    
    //============rect============//
    valueTreeState.addParameterListener("width", this);
    valueTreeState.addParameterListener("length", this);
    valueTreeState.addParameterListener("height", this);
    x=*vts.getRawParameterValue("width");
    y=*vts.getRawParameterValue("length");
    z=*vts.getRawParameterValue("height");
    
    if (x>y) {xR=1;yR=y/x;}
    else if (x<y) {yR=1;xR=x/y;}
    else if (x==y) {xR=yR=1;}
    
    //============sourceHeight============//
    sourceButton.setButtonText("Source Height");
    sourceButton.setLookAndFeel(&buttonLook);
    sourceButton.setColour(TextButton::buttonColourId,Colours::darkslategrey);
    sourceButton.setEnabled(false);
    addAndMakeVisible(&sourceButton);
    
    sourceHeightSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    sourceHeightSlider.setRange(0,1);
    sourceHeightSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 30, 25);
    sourceHeightSlider.setNumDecimalPlacesToDisplay(2);
    sourceHeightSlider.setSkewFactorFromMidPoint(0.5);
    sourceHeightSlider.setColour(Slider::thumbColourId, Colours::cyan);
    sourceHeightSlider.addListener(this);
    addAndMakeVisible(&sourceHeightSlider);
    
    //============micHeight============//
    micButton.setButtonText("Listener Height");
    micButton.setLookAndFeel(&buttonLook);
    micButton.setColour(TextButton::buttonColourId,Colours::darkslategrey);
    micButton.setEnabled(false);
    addAndMakeVisible(&micButton);
    
    micHeightSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    micHeightSlider.setRange(0,1);
    micHeightSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 30, 25);
    micHeightSlider.setNumDecimalPlacesToDisplay(2);
    micHeightSlider.setSkewFactorFromMidPoint(0.5);
    micHeightSlider.setColour(Slider::thumbColourId, Colours::cyan);
    micHeightSlider.addListener(this);
    addAndMakeVisible(&micHeightSlider);
    
    //============StatusBarH============//
    addAndMakeVisible(&sourceBarH);
    addAndMakeVisible(&micBarH);
    sourceBarH.setReadOnly(true);
    micBarH.setReadOnly(true);
    sourceBarH.setFont(boxFont);
    micBarH.setFont(boxFont);
    sourceBarH.setLookAndFeel(&roomR.textEditorLookAndFeel);
    micBarH.setLookAndFeel(&roomR.textEditorLookAndFeel);
    
    //============InitStatusBarH============//
    sh=z*sourceHeightSlider.getValue();
    mh=z*micHeightSlider.getValue();
    
    std::stringstream streamS;
    streamS << std::fixed << std::setprecision(2) << sh;
    std::stringstream streamM;
    streamM << std::fixed << std::setprecision(2) << mh;

    sourceBarH.setText("Z: "+streamS.str());
    micBarH.setText("Z: "+streamM.str());
    
    //============Images============//
    sourceLogo.setImages(false, true, true, roomR.sourceImg, 1.0f, Colour(0,204,204), roomR.sourceImg, 1.0f, Colours::cyan, roomR.sourceImg, 4.0f, Colours::cyan);
    sourceLogo.setEnabled(false);
    sourceLogo.setSize(12, 12);
    addAndMakeVisible(&sourceLogo);
    
    micLogo.setImages(false, true, true, roomR.micImg, 1.0f, Colour(0,204,204), roomR.micImg, 1.0f, Colours::cyan, roomR.micImg, 4.0f, Colours::cyan);
    micLogo.setEnabled(false);
    micLogo.setSize(12, 12);
    addAndMakeVisible(&micLogo);
}

RoomSourceAndListener::~RoomSourceAndListener()
{
    sourceButton.setLookAndFeel(nullptr);
    micButton.setLookAndFeel(nullptr);
    sourceBarH.setLookAndFeel(nullptr);
    micBarH.setLookAndFeel(nullptr);
    sourceHeightSlider.removeListener(this);
    micHeightSlider.removeListener(this);
    sourceHeightAttachment.reset();
    micHeightAttachment.reset();
}

void RoomSourceAndListener::paint (Graphics& g)
{
    Font titleFont("Montserrat","Regular",14.0f);
    Font widthlengthFont("Montserrat","Regular",12.0f);
    
    Rectangle<int> area=getLocalBounds();
    Rectangle<int> titleArea=area.removeFromTop(20);
       
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.setFont(titleFont);
    g.drawText("Source and Listener position", titleArea, Justification::centred);
    g.setColour(Colours::darkslategrey);
    g.drawRect(area);
    g.setColour(Colours::darkslategrey);
    g.drawRect(titleArea);
    
    Rectangle<int> roomRectArea;
    
    roomRectArea.setWidth(fmax(round(xR*280),50));
    roomRectArea.setHeight(fmax(round(yR*280),50));
    roomRectArea.setCentre(area.getWidth()/2, area.getHeight()/2);
    
    Rectangle<int> statusBarArea=area.removeFromBottom(30);
    g.setColour(Colours::darkslategrey);
    Line<float> barLine;
    barLine.setStart(0, statusBarArea.getY()+2);
    barLine.setEnd(getWidth(), statusBarArea.getY()+2);
    g.drawLine(barLine, 2);
    
    g.setColour(Colours::lightgrey);
    g.setFont(widthlengthFont);
    
    Line<float> widthLine;
    widthLine.setStart(roomRectArea.getX()-3, roomRectArea.getY()+roomRectArea.getHeight()+3);
    widthLine.setEnd(roomRectArea.getX()+50, roomRectArea.getY()+roomRectArea.getHeight()+3);
    g.drawArrow(widthLine, 1, 6, 4);
    
    g.drawText("width (X)", roomRectArea.getX(), roomRectArea.getY()+roomRectArea.getHeight()+3, 50, 3, Justification::topLeft);
    
    Line<float> lengthLine;
    lengthLine.setStart(roomRectArea.getX()-3, roomRectArea.getY()+roomRectArea.getHeight()+3);
    lengthLine.setEnd(roomRectArea.getX()-3, roomRectArea.getY()+roomRectArea.getHeight()+3-53);
    g.drawArrow(lengthLine, 1, 6, 4);
    
    //Graphics::ScopedSaveState state (g);
    g.addTransform (AffineTransform::rotation (-MathConstants<float>::halfPi,roomRectArea.getX()-16,roomRectArea.getY()+roomRectArea.getHeight()+3));
    g.drawText("length (Y)", roomRectArea.getX()-17, roomRectArea.getY()+roomRectArea.getHeight()+3, 50, 3, Justification::topLeft);
}

void RoomSourceAndListener::resized()
{
    Rectangle<int> area=getLocalBounds();
    area.removeFromTop(20);
    Rectangle<int> roomRectArea;
    
    roomRectArea.setWidth(fmax(round(xR*280),50));
    roomRectArea.setHeight(fmax(round(yR*280),50));
    roomRectArea.setCentre(area.getWidth()/2, area.getHeight()/2);
    
    roomR.setBounds(roomRectArea);
        
    sourceButton.setBounds(8, 35, 45, 30);
    sourceHeightSlider.setBounds(8, 70, 45, 160);
    
    micButton.setBounds(getWidth()-53, 35, 45, 30);
    micHeightSlider.setBounds(getWidth()-53, 70, 45, 160);
    
    Rectangle<int> statusBarArea=area.removeFromBottom(30);
    statusBarArea.reduce(5, 5);
    Rectangle<int> sourceBarArea=statusBarArea.removeFromLeft(statusBarArea.getWidth()/2);
    Rectangle<int> micBarArea=statusBarArea;
    Rectangle<int> sourceLogoArea=sourceBarArea.removeFromLeft(15);
    Rectangle<int> micLogoArea=micBarArea.removeFromLeft(15);

    
    sourceBarArea.reduce(2, 0);
    micBarArea.reduce(2, 0);
    
    sourceLogo.setCentrePosition(sourceLogoArea.getCentre());
    sourceLogo.setSize(12, 12);
    roomR.sourceBarW.setBounds(sourceBarArea.removeFromLeft(sourceBarArea.getWidth()/3));
    roomR.sourceBarL.setBounds(sourceBarArea.removeFromLeft(sourceBarArea.getWidth()/2));
    sourceBarH.setBounds(sourceBarArea);
    
    micLogo.setCentrePosition(micLogoArea.getCentre());
    micLogo.setSize(12, 12);
    roomR.micBarW.setBounds(micBarArea.removeFromLeft(micBarArea.getWidth()/3));
    roomR.micBarL.setBounds(micBarArea.removeFromLeft(micBarArea.getWidth()/2));
    micBarH.setBounds(micBarArea);
    
}

void RoomSourceAndListener::parameterChanged (const String &parameterID, float newValue)
{
    if (parameterID == "width")
    {
        x = newValue;
        //cambio rapporto rettangolo
        if (newValue>y)
        {
            xR=1;
            yR=y/newValue;
        }
        else if (newValue<y)
        {
            yR=1;
            xR=newValue/y;
        }
        else if (newValue==y)
        {
            xR=yR=1;
        }
        repaint();
        resized();
    }
    else if (parameterID == "length")
    {
        y = newValue;
        //cambio rapporto rettangolo
        if (newValue>x)
        {
            yR=1;
            xR=x/newValue;
        }
        else if (newValue<x)
        {
            xR=1;
            yR=newValue/x;
        }
        else if (newValue==x)
        {
            xR=yR=1;
        }
        repaint();
        resized();
    }
    else if (parameterID == "height")
    {
        z = newValue;
        
        sh=z*sourceHeightSlider.getValue();
        mh=z*micHeightSlider.getValue();
        
        std::stringstream streamS;
        streamS << std::fixed << std::setprecision(2) << sh;
        std::stringstream streamM;
        streamM << std::fixed << std::setprecision(2) << mh;

        sourceBarH.setText("Z: "+streamS.str());
        micBarH.setText("Z: "+streamM.str());
        resized();
    }
}

void RoomSourceAndListener::sliderValueChanged(Slider *slider)
{
    if (slider == &sourceHeightSlider)
    {
        sh=z*sourceHeightSlider.getValue();
        std::stringstream streamS;
        streamS << std::fixed << std::setprecision(2) << sh;
        sourceBarH.setText("Z: "+streamS.str());
        resized();
    }
    if (slider == &micHeightSlider)
    {
        mh=z*micHeightSlider.getValue();
        std::stringstream streamM;
        streamM << std::fixed << std::setprecision(2) << mh;
        micBarH.setText("Z: "+streamM.str());
        resized();
    }
}
