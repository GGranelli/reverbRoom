/*
  ==============================================================================

    RoomRect.cpp
    Created: 14 Nov 2019 5:14:42pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RoomRect.h"

//==============================================================================
RoomRect::RoomRect(ReverbRoomAudioProcessor& p,AudioProcessorValueTreeState& vts) :
processor(p),valueTreeState(vts)
{
    Font theFont("Montserrat","Regular",8.0f);
    Font labelFont("Montserrat","Regular",10.0f);
    Font boxFont("Montserrat","Regular",12.0f);

    //pointers
    sourceXAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"sourcePosX",sourceXSlider));
    sourceYAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"sourcePosY",sourceYSlider));
    micXAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"micPosX",micXSlider));
    micYAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(valueTreeState,"micPosY",micYSlider));
        
    setSize(250, 250);
    
    
    //button images
    sourceImg=ImageFileFormat::loadFrom(BinaryData::source_png, BinaryData::source_pngSize);
    micImg=ImageFileFormat::loadFrom(BinaryData::mic_png, BinaryData::mic_pngSize);
    
    //============sourceButton============//
    source.setImages(false, true, true, sourceImg, 1.0f, Colour(0,51,51), sourceImg, 1.0f, Colours::cyan, sourceImg, 4.0f, Colours::cyan);
    source.addMouseListener(this, false);
    addAndMakeVisible(&source);
    
    //============micButton============//
    mic.setImages(false, true, true, micImg, 1.0f, Colour(0,51,51), micImg, 1.0f, Colours::cyan, micImg, 4.0f, Colours::cyan);
    mic.addMouseListener(this, false);
    addAndMakeVisible(&mic);
    
    //============Sliders============//
    addAndMakeVisible(&sourceXSlider);
    addAndMakeVisible(&sourceYSlider);
    addAndMakeVisible(&micXSlider);
    addAndMakeVisible(&micYSlider);
    
    sourceXSlider.setVisible(false);
    sourceYSlider.setVisible(false);
    micXSlider.setVisible(false);
    micYSlider.setVisible(false);
    
    sourceXSlider.addListener(this);
    sourceYSlider.addListener(this);
    micXSlider.addListener(this);
    micYSlider.addListener(this);
    
    //============RectSize============//
    Rectangle<int> area=getLocalBounds();
    rectWidth=area.getWidth();
    rectHeight=area.getHeight();
    
    //============StatusBarWL============//
    addAndMakeVisible(&sourceBarW);
    addAndMakeVisible(&sourceBarL);
    sourceBarW.setReadOnly(true);
    sourceBarL.setReadOnly(true);
    sourceBarW.setFont(boxFont);
    sourceBarL.setFont(boxFont);
    sourceBarW.setLookAndFeel(&textEditorLookAndFeel);
    sourceBarL.setLookAndFeel(&textEditorLookAndFeel);
    
    addAndMakeVisible(&micBarW);
    addAndMakeVisible(&micBarL);
    micBarW.setReadOnly(true);
    micBarL.setReadOnly(true);
    micBarW.setFont(boxFont);
    micBarL.setFont(boxFont);
    micBarW.setLookAndFeel(&textEditorLookAndFeel);
    micBarL.setLookAndFeel(&textEditorLookAndFeel);
    
    //============RoomSize============//
    valueTreeState.addParameterListener("width", this);
    valueTreeState.addParameterListener("length", this);
    valueTreeState.addParameterListener("height", this);
    roomWidth=*vts.getRawParameterValue("width");
    roomLength=*vts.getRawParameterValue("length");
    roomHeight=*vts.getRawParameterValue("height");
    
    //============Source&MicHeights============//
    valueTreeState.addParameterListener("sourceHeight", this);
    valueTreeState.addParameterListener("micHeight", this);
    sourceHeight=*vts.getRawParameterValue("sourceHeight");
    micHeight=*vts.getRawParameterValue("micHeight");
    sourceSize=round(10*sourceHeight)+10;
    micSize=round(10*micHeight)+10;
    
    source.setSize(sourceSize, sourceSize);
    mic.setSize(micSize, micSize);
    
    //============InitStatusBarWL============//
    std::stringstream streamSX;
    std::stringstream streamSY;
    std::stringstream streamMX;
    std::stringstream streamMY;
    
    sx=(source.getX()+sourceSize/2)/rectWidth*roomWidth;
    sy=(abs(source.getY()-rectHeight)+sourceSize/2)/rectHeight*roomLength;
    streamSX << std::fixed << std::setprecision(2) << sx;
    streamSY << std::fixed << std::setprecision(2) << sy;
    sourceBarW.setText("X: "+streamSX.str());
    sourceBarL.setText("Y: "+streamSY.str());

    sx=(mic.getX()+micSize/2)/rectWidth*roomWidth;
    sy=(abs(mic.getY()-rectHeight)+micSize/2)/rectHeight*roomLength;
    streamMX << std::fixed << std::setprecision(2) << sx;
    streamMY << std::fixed << std::setprecision(2) << sy;
    micBarW.setText("X: "+streamMX.str());
    micBarL.setText("Y: "+streamMY.str());
}

RoomRect::~RoomRect()
{
    sourceXAttachment.reset();
    sourceYAttachment.reset();
    micXAttachment.reset();
    micYAttachment.reset();
    sourceBarW.setLookAndFeel(nullptr);
    sourceBarL.setLookAndFeel(nullptr);
    micBarW.setLookAndFeel(nullptr);
    micBarL.setLookAndFeel(nullptr);
}

void RoomRect::paint (Graphics& g)
{
    g.setColour(Colour(132,132,132));
    g.fillRect(getLocalBounds());
    g.setColour(Colour(190,190,190));
    g.fillRect(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight)),
                10+round(fmin(rectHeight,rectWidth)/280*(roomHeight)),
                getWidth()-2*(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight))),
                getHeight()-2*(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight))));
    g.setColour(Colour(90,90,90));
    g.drawRect(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight)),
               10+round(fmin(rectHeight,rectWidth)/280*(roomHeight)),
               getWidth()-2*(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight))),
               getHeight()-2*(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight))));
    g.drawLine(0, 0,
               10+round(fmin(rectHeight,rectWidth)/280*(roomHeight)),
               10+round(fmin(rectHeight,rectWidth)/280*(roomHeight)));
    g.drawLine(getWidth(), 0,
               getWidth()-(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight))),
               10+round(fmin(rectHeight,rectWidth)/280*(roomHeight)));
    g.drawLine(0, getHeight(),
               10+round(fmin(rectHeight,rectWidth)/280*(roomHeight)),
               getHeight()-(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight))));
    g.drawLine(getWidth(), getHeight(),
               getWidth()-(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight))),
               getHeight()-(10+round(fmin(rectHeight,rectWidth)/280*(roomHeight))));
}

void RoomRect::resized()
{
    Rectangle<int> area;
    area=getLocalBounds();
    source.setBounds(round(sourceXSlider.getValue()*area.getWidth()),
                     round(sourceYSlider.getValue()*area.getHeight()),
                     sourceSize, sourceSize);
    mic.setBounds(round(micXSlider.getValue()*area.getWidth()),
                  round(micYSlider.getValue()*area.getHeight()),
                  micSize, micSize);
    
    rectWidth=area.getWidth();
    rectHeight=area.getHeight();
}


void RoomRect::mouseDrag (const MouseEvent& event)
{
    if (source.isDown())
    {
        sourceFocus=true;
        startDragging("source", &source);
    }
    if (mic.isDown())
    {
        micFocus=true;
        startDragging("mic", &mic);
    }
    if (sourceFocus)
    {
        std::stringstream streamX;
        std::stringstream streamY;
        sx=(source.getX()+sourceSize/2+event.getDistanceFromDragStartX())/rectWidth*roomWidth;
        sy=(abs(source.getY()-rectHeight)-sourceSize/2+(event.getDistanceFromDragStartY()*-1)) /rectHeight*roomLength;
        streamX << std::fixed << std::setprecision(2) << sx;
        streamY << std::fixed << std::setprecision(2) << sy;
        sourceBarW.setText("X: "+streamX.str());
        sourceBarL.setText("Y: "+streamY.str());
        streamX.clear();
        streamY.clear();
    }
    if (micFocus)
    {
        std::stringstream streamX;
        std::stringstream streamY;
        sx=(mic.getX()+micSize/2+event.getDistanceFromDragStartX())/rectWidth*roomWidth;
        sy=(abs(mic.getY()-rectHeight)-micSize/2+(event.getDistanceFromDragStartY()*-1))/rectHeight*roomLength;
        streamX << std::fixed << std::setprecision(2) << sx;
        streamY << std::fixed << std::setprecision(2) << sy;
        micBarW.setText("X: "+streamX.str());
        micBarL.setText("Y: "+streamY.str());
        streamX.clear();
        streamY.clear();
    }
}

void RoomRect::mouseUp(const MouseEvent& event)
{
    if (sourceFocus)
    {
        if( (source.getX()+sourceSize/2+event.getDistanceFromDragStartX())>rectWidth || (source.getY()+sourceSize/2+event.getDistanceFromDragStartY())>rectHeight ||
            (source.getX()+sourceSize/2+event.getDistanceFromDragStartX())<1 ||
            (source.getY()+sourceSize/2+event.getDistanceFromDragStartY())<1 )
            source.setBounds(source.getX(), source.getY(),
                             sourceSize, sourceSize);
        
        else source.setBounds(source.getX()+event.getDistanceFromDragStartX(),
                              source.getY()+event.getDistanceFromDragStartY(),
                              sourceSize, sourceSize);
        
        sourceFocus=false;
        sourceXSlider.setValue(source.getX()/rectWidth);
        sourceYSlider.setValue(source.getY()/rectHeight);
    }
    
    if (micFocus)
    {
        if( (mic.getX()+micSize/2+event.getDistanceFromDragStartX())>rectWidth || (mic.getY()+micSize/2+event.getDistanceFromDragStartY())>rectHeight ||
            (mic.getX()+micSize/2+event.getDistanceFromDragStartX())<1 ||
            (mic.getY()+micSize/2+event.getDistanceFromDragStartY())<1 )
            mic.setBounds(mic.getX(), mic.getY(),
                          micSize, micSize);
        
        else mic.setBounds(mic.getX()+event.getDistanceFromDragStartX(),
                           mic.getY()+event.getDistanceFromDragStartY(),
                           micSize, micSize);

        micFocus=false;
        micXSlider.setValue(mic.getX()/rectWidth);
        micYSlider.setValue(mic.getY()/rectHeight);
    }
}


void RoomRect::parameterChanged (const String &parameterID, float newValue)
{
    if (parameterID == "width")
    {
        roomWidth=newValue;
        
        std::stringstream streamSX;
        std::stringstream streamSY;
        std::stringstream streamMX;
        std::stringstream streamMY;
        
        sx=(source.getX()+sourceSize/2)/rectWidth*roomWidth;
        sy=(abs(source.getY()-rectHeight)+sourceSize/2)/rectHeight*roomLength;
        streamSX << std::fixed << std::setprecision(2) << sx;
        streamSY << std::fixed << std::setprecision(2) << sy;
        sourceBarW.setText("X: "+streamSX.str());
        sourceBarL.setText("Y: "+streamSY.str());

        sx=(mic.getX()+micSize/2)/rectWidth*roomWidth;
        sy=(abs(mic.getY()-rectHeight)+micSize/2)/rectHeight*roomLength;
        streamMX << std::fixed << std::setprecision(2) << sx;
        streamMY << std::fixed << std::setprecision(2) << sy;
        micBarW.setText("X: "+streamMX.str());
        micBarL.setText("Y: "+streamMY.str());
    }
    if (parameterID == "length")
    {
        roomLength=newValue;
        
        std::stringstream streamSX;
        std::stringstream streamSY;
        std::stringstream streamMX;
        std::stringstream streamMY;
        
        sx=(source.getX()+sourceSize/2)/rectWidth*roomWidth;
        sy=(abs(source.getY()-rectHeight)+sourceSize/2)/rectHeight*roomLength;
        streamSX << std::fixed << std::setprecision(2) << sx;
        streamSY << std::fixed << std::setprecision(2) << sy;
        sourceBarW.setText("X: "+streamSX.str());
        sourceBarL.setText("Y: "+streamSY.str());

        sx=(mic.getX()+micSize/2)/rectWidth*roomWidth;
        sy=(abs(mic.getY()-rectHeight)+micSize/2)/rectHeight*roomLength;
        streamMX << std::fixed << std::setprecision(2) << sx;
        streamMY << std::fixed << std::setprecision(2) << sy;
        micBarW.setText("X: "+streamMX.str());
        micBarL.setText("Y: "+streamMY.str());
    }
    if (parameterID == "height")
    {
        roomHeight=newValue;
        repaint();
    }
    if (parameterID == "sourceHeight")
    {
        sourceHeight=newValue;
        sourceSize=round(10*sourceHeight)+10;
        resized();
    }
    if (parameterID == "micHeight")
    {
        micHeight=newValue;
        micSize=round(10*micHeight)+10;
        resized();
    }
}
void RoomRect::sliderValueChanged(Slider *slider)
{
    if (slider == &sourceXSlider)
    {
        resized();
    }
    if (slider == &sourceYSlider)
    {
        resized();
    }
    if (slider == &micXSlider)
    {
        resized();
    }
    if (slider == &micYSlider)
    {
        resized();
    }
}
 
