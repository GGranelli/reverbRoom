/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Eigen/Dense"
#include "Eigen/Sparse"

//==============================================================================
ReverbRoomAudioProcessor::ReverbRoomAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
audioTree(*this, nullptr, Identifier("PARAMETERS"),
    {std::make_unique<AudioParameterFloat>("gain","Gain",NormalisableRange<float>(-48,12,0.01),0),
     std::make_unique<AudioParameterFloat>("width","Width",NormalisableRange<float>(1,50.0,0.01),5),
     std::make_unique<AudioParameterFloat>("length","Length",NormalisableRange<float>(1,50,0.01),5),
     std::make_unique<AudioParameterFloat>("height","Height",NormalisableRange<float>(1,20,0.01),2),
     std::make_unique<AudioParameterFloat>("reflection","Reflection",NormalisableRange<float>(0,1,0.01),0),
     std::make_unique<AudioParameterFloat>("sourceHeight","SourceHeight",NormalisableRange<float> (0,1,0.01),0.3),
     std::make_unique<AudioParameterFloat>("micHeight","MicHeight",NormalisableRange<float>(0,1,0.01),0.3),
     std::make_unique<AudioParameterFloat>("sourcePosX","SourcePosX",NormalisableRange<float>(0,1,0.0001),0.2),
     std::make_unique<AudioParameterFloat>("sourcePosY","SourcePosY",NormalisableRange<float>(0,1,0.0001),0.5),
     std::make_unique<AudioParameterFloat>("micPosX","MicPosX",NormalisableRange<float>(0,1,0.0001),0.8),
     std::make_unique<AudioParameterFloat>("micPosY","MicPosY",NormalisableRange<float>(0,1,0.0001),0.5)
    })
#endif
{
    audioTree.addParameterListener("gain", this);
    audioTree.addParameterListener("width", this);
    audioTree.addParameterListener("length", this);
    audioTree.addParameterListener("height", this);
    audioTree.addParameterListener("reflection", this);
    audioTree.addParameterListener("sourceHeight", this);
    audioTree.addParameterListener("micHeight", this);
    audioTree.addParameterListener("sourcePosX", this);
    audioTree.addParameterListener("sourcePosY", this);
    audioTree.addParameterListener("micPosX", this);
    audioTree.addParameterListener("micPosY", this);
    
    //gain
    gainValue = *audioTree.getRawParameterValue("gain");
    gainValue = gainSmoothed = Decibels::decibelsToGain(gainValue);
    
    //rir
    nn.reset(new Eigen::ArrayXd);
    rms.reset(new Eigen::ArrayXd);
    srcs.reset(new Eigen::ArrayXd);
    xi.reset(new Eigen::ArrayXd);
    yj.reset(new Eigen::ArrayXd);
    zk.reset(new Eigen::ArrayXd);
    x2y2.reset(new Eigen::ArrayXd);
    d.reset(new Eigen::ArrayXd);
    time.reset(new Eigen::ArrayXd);
    ef.reset(new Eigen::ArrayXd);
    refl.reset(new Eigen::ArrayXd);
    e.reset(new Eigen::ArrayXd);
    h.reset(new Eigen::ArrayXd);
    
    nn->resize(N);
    rms->resize(N);
    srcs->resize(N);
    xi->resize(N);
    yj->resize(N);
    zk->resize(N);
    x2y2->resize(N2, 1);
    d->resize(N3, 1);
    time->resize(N3, 1);
    ef->resize(N2, 1);
    refl->resize(N3, 1);
    e->resize(N3, 1);
    
    //init nn
    for(int i = -virtualSources; i <= virtualSources; ++i)
    {
        nn->coeffRef(counter) = i;
        counter++;
    }

    //init rms and srcs
    for(int i = 0; i <= 2*virtualSources; ++i)
    {
        srcs->coeffRef(i) = pow(-1.0,nn->coeffRef(i));
        rms->coeffRef(i) = nn->coeffRef(i) + (1 - srcs->coeffRef(i))/2;
    }
    
}

ReverbRoomAudioProcessor::~ReverbRoomAudioProcessor()
{
    nn->resize(0);
    rms->resize(0);
    srcs->resize(0);
    xi->resize(0);
    yj->resize(0);
    zk->resize(0);
    x2y2->resize(0);
    d->resize(0);
    time->resize(0);
    ef->resize(0);
    refl->resize(0);
    e->resize(0);
    h->resize(0);
    
    nn.reset();
    rms.reset();
    srcs.reset();
    xi.reset();
    yj.reset();
    zk.reset();
    x2y2.reset();
    d.reset();
    time.reset();
    ef.reset();
    refl.reset();
    e.reset();
    h.reset();
}

//==============================================================================
const String ReverbRoomAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbRoomAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbRoomAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbRoomAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbRoomAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbRoomAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbRoomAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbRoomAudioProcessor::setCurrentProgram (int index)
{
}

const String ReverbRoomAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbRoomAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ReverbRoomAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate=sampleRate;
    
    //buffer
    mImpulseBuffer.clear();
    
    //update
    mNeedsUpdate=false;
    
    //counter
    waitTime=sampleRate*0.100; //ms
    waitCounter=waitTime;
    
    //convolution
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    rir(sampleRate, virtualSources, reflectionCoeff, roomDimensions, sourcePosition, micPosition);
    reverb.copyAndLoadImpulseResponseFromBuffer(mImpulseBuffer, sampleRate, true, false, true, 0);
    reverb.prepare(spec);
    reverb.reset();
}

void ReverbRoomAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbRoomAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void ReverbRoomAudioProcessor::parameterChanged (const String &parameterID, float newValue)
{
    if (parameterID == "gain")
    {
        gainValue = Decibels::decibelsToGain(newValue);
    }
    else if (parameterID == "width")
    {
        roomDimensions[0] = newValue;
        mNeedsUpdate=true;
    }
    else if (parameterID == "length")
    {
        roomDimensions[1] = newValue;
        mNeedsUpdate=true;
    }
    else if (parameterID == "height")
    {
        roomDimensions[2] = newValue;
        mNeedsUpdate=true;
    }
    else if (parameterID == "reflection")
    {
        reflectionCoeff = newValue;
        mNeedsUpdate=true;
    }
    else if (parameterID == "sourceHeight")
    {
        sourcePosition[2] = newValue*roomDimensions[2];
        mNeedsUpdate=true;
    }
    else if (parameterID == "micHeight")
    {
        micPosition[2] = newValue*roomDimensions[2];
        mNeedsUpdate=true;
    }
    else if (parameterID == "sourcePosX")
    {
        sourcePosition[0] = newValue*roomDimensions[0];
        mNeedsUpdate=true;
    }
    else if (parameterID == "sourcePosY")
    {
        sourcePosition[1] = newValue*roomDimensions[1];
        mNeedsUpdate=true;
    }
    else if (parameterID == "micPosX")
    {
        micPosition[0] = newValue*roomDimensions[0];
        mNeedsUpdate=true;
    }
    else if (parameterID == "micPosY")
    {
        micPosition[1] = newValue*roomDimensions[1];
        mNeedsUpdate=true;
    }
}

void ReverbRoomAudioProcessor::updateParameters()
{
    //ricalcolo rir e convolution
    rir(lastSampleRate, virtualSources, reflectionCoeff, roomDimensions, sourcePosition, micPosition);
    reverb.copyAndLoadImpulseResponseFromBuffer(mImpulseBuffer, lastSampleRate, true, false, true, 0);
    mNeedsUpdate=false;
    waitCounter=waitTime;
}

void ReverbRoomAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //data structure that points to the buffer
    dsp::AudioBlock<float> block (buffer);
    
    //update any parameters for your processes
    if (waitCounter<=0 && mNeedsUpdate) updateParameters();
    
    
    //JUCE dsp processes each have their own process method
    reverb.process (dsp::ProcessContextReplacing<float> (block));
    
    /*
     for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
     {
         gainSmoothed=gainSmoothed-0.004*(gainSmoothed-gainValue);
         for (int channel = 0; channel < totalNumInputChannels; ++channel)
         {
             auto* channelData = buffer.getWritePointer (channel);
             channelData[sample]*=gainSmoothed*5;
         }
         waitCounter--;
     }
     */
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            gainSmoothed=gainSmoothed-0.004*(gainSmoothed-gainValue);
            channelData[sample]*=gainSmoothed*5;
            waitCounter--;
        }
    }
}

//==============================================================================
bool ReverbRoomAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ReverbRoomAudioProcessor::createEditor()
{
    return new ReverbRoomAudioProcessorEditor (*this,audioTree);
}

//==============================================================================
void ReverbRoomAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state=audioTree.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ReverbRoomAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr)
        if (xml->hasTagName (audioTree.state.getType()))
            audioTree.replaceState (ValueTree::fromXml (*xml));
}


//Room Impulse Response
void ReverbRoomAudioProcessor::rir(double sampleRate, int virtualS, float reflectionC, float roomMeasurements[3], float sourcePosition[3], float micPosition[3])
{
    //counter=0;
    //ntemp=0;
    //m=0;
    
    //init nn
    /*
    for(ntemp = -virtualS; ntemp <= virtualS; ++ntemp)
    {
        nn->coeffRef(counter) = ntemp;
        counter++;
    }

    //init rms and srcs
    for(ntemp = 0; ntemp <= 2*virtualS; ++ntemp)
    {
        srcs->coeffRef(ntemp) = pow(-1.0,nn->coeffRef(ntemp));
        rms->coeffRef(ntemp) = nn->coeffRef(ntemp) + (1 - srcs->coeffRef(ntemp))/2;
    }
     
    for(int i = -virtualS; i <= virtualS; ++i)
    {
        nn->coeffRef(counter) = i;
        counter++;
    }

    //init rms and srcs
    for(int i = 0; i <= 2*virtualS; ++i)
    {
        srcs->coeffRef(i) = pow(-1.0,nn->coeffRef(i));
        rms->coeffRef(i) = nn->coeffRef(i) + (1 - srcs->coeffRef(i))/2;
    }
     */
    //distance between each i,j,k'th virtual source and mic position in the x,y and z axis
    *xi = *srcs*sourcePosition[0]+(*rms*roomMeasurements[0])-micPosition[0];
    *yj = *srcs*sourcePosition[1]+(*rms*roomMeasurements[1])-micPosition[1];
    *zk = *srcs*sourcePosition[2]+(*rms*roomMeasurements[2])-micPosition[2];
    
    //calculate d
    Eigen::ArrayXXd::Map(x2y2->data(), N, N) = xi->square().transpose().replicate(N,1) + yj->square().replicate(1,N);
    Eigen::ArrayXXd::Map(d->data(), N2, N) = x2y2->replicate(1,N) + zk->square().transpose().replicate(N2,1);

    //d=Straight line distance calculation to each virtual source using Pythagora theorem
    *d=d->sqrt();
  
    //time domain IR function of each virtual source
    *time=floor(sampleRate*(*d/343))+1;
    
    //refl=total reflection coefficient
    refl->setZero();
    
    Eigen::ArrayXXd::Map(ef->data(), N, N) = nn->abs().transpose().replicate(N,1) + nn->abs().replicate(1,N);
    Eigen::ArrayXXd::Map(refl->data(), N2, N) = ef->replicate(1,N) + nn->abs().transpose().replicate(N2,1);
    
    //Implementation of phase inversion as per Lehmann & Johansson's ISM structure
    *refl=pow(-reflectionC,*refl);
    
    //e=total magnitude of each echo, as the distance the sound travels from source to receiver is proportional to 1/d the total magnitude of each echo is equal to the total reflection coefficient multiplied by 1/d
    *e=*refl/(*d);

    //h=impulse response: total magnitude multiplied by unit impulse response function and the sum over all indices
    m=time->maxCoeff()+1;

    h->resize(m);
    h->setZero();
    
    for (int i=0; i<N3; ++i)
    {
        h->coeffRef(time->coeffRef(i))=e->coeffRef(i);
    }
    
    //scale the output
    *h=*h/(h->abs().maxCoeff());

    //fill the buffer
    mImpulseBuffer.clear();
    
    
    if (mImpulseBuffer.getNumSamples()!=m || mImpulseBuffer.getNumChannels()!=getTotalNumInputChannels())
        mImpulseBuffer.setSize(getTotalNumInputChannels(), m);
    
    for (int j=0; j<getTotalNumInputChannels(); ++j){
        
        for (int i=0; i<mImpulseBuffer.getNumSamples(); ++i)
        {
            mImpulseBuffer.addSample(j, i, h->coeffRef(i));
        }
    }
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbRoomAudioProcessor();
}
