/*
  ==============================================================================

    About.h
    Created: 14 Mar 2020 5:24:59pm
    Author:  Gabriele Granelli

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class About    : public Component
{
public:
    About();
    ~About();

    void paint (Graphics&) override;
    void resized() override;

private:
    DrawableImage logoImg;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (About)
};
