/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Eigen/Dense"
#include "Eigen/Sparse"

//==============================================================================
/**
*/
class ReverbRoomAudioProcessor  : public AudioProcessor,
                                  public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ReverbRoomAudioProcessor();
    ~ReverbRoomAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //rir
    void rir(double sampleRate, int virtualS, float reflectionCoeff, float roomDimensions[3], float sourcePosition[3], float micPosition[3]);
    
    //update
    bool mNeedsUpdate={false};
    void updateParameters();
    
    //tree
    void parameterChanged (const String &parameterID, float newValue) override;
    AudioProcessorValueTreeState audioTree;
    
private:
    AudioBuffer<float> mImpulseBuffer;
    dsp::Convolution reverb;
    
    float roomDimensions[3]={5,5,2};
    float micPosition[3]={4,2.5,1.6};
    float sourcePosition[3]={1,2.5,1};
    float reflectionCoeff={0};
    
    float lastSampleRate={44100};
    
    //Gain
    float gainValue,gainSmoothed;
    
    //Ricalcolo RIR
    int waitCounter,waitTime;
    
    //init RIR
    int virtualSources={4};
    int N=2*virtualSources+1;
    int N2=N*N;
    int N3=N2*N;
    int counter=0;
    //int ntemp=0;
    int m=0;

    std::unique_ptr<Eigen::ArrayXd> nn;
    std::unique_ptr<Eigen::ArrayXd> rms;
    std::unique_ptr<Eigen::ArrayXd> srcs;
    std::unique_ptr<Eigen::ArrayXd> xi;
    std::unique_ptr<Eigen::ArrayXd> yj;
    std::unique_ptr<Eigen::ArrayXd> zk;
    std::unique_ptr<Eigen::ArrayXd> x2y2;
    std::unique_ptr<Eigen::ArrayXd> d;
    std::unique_ptr<Eigen::ArrayXd> time;
    std::unique_ptr<Eigen::ArrayXd> ef;
    std::unique_ptr<Eigen::ArrayXd> refl;
    std::unique_ptr<Eigen::ArrayXd> e;
    std::unique_ptr<Eigen::ArrayXd> h;
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbRoomAudioProcessor)
};
