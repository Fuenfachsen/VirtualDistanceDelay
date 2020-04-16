/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.
 
    VirtualDistanceDelay - a simple delay audio plugin that lets you select
    distance instead of time for the delay time.
    Copyright (C) 2020  Johannes Schmitt
 
 This file is part of VirtualDistanceDelay.

    VirtualDistanceDelay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VirtualDistanceDelay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

    Diese Datei ist Teil von VirtualDistanceDelay.

    VirtualDistanceDelay ist Freie Software: Sie können es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
    veröffentlichten Version, weiter verteilen und/oder modifizieren.

    VirtualDistanceDelay wird in der Hoffnung, dass es nützlich sein wird, aber
    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License für weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VirtualDistanceDelayAudioProcessor::VirtualDistanceDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif

                       ),
    parameters(*this, nullptr, Identifier("myParams"),
               {
        std::make_unique<AudioParameterFloat>(GAIN_ID,GAIN_NAME, 0.0f,1.0f,0.5f), std::make_unique<AudioParameterFloat>(DISTANCE_ID, DISTANCE_NAME, 0.0f, 21000.0f,93.0f)   })

#endif
{

}


VirtualDistanceDelayAudioProcessor::~VirtualDistanceDelayAudioProcessor()
{
}

//==============================================================================
const String VirtualDistanceDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VirtualDistanceDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VirtualDistanceDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VirtualDistanceDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VirtualDistanceDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VirtualDistanceDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VirtualDistanceDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VirtualDistanceDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String VirtualDistanceDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void VirtualDistanceDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void VirtualDistanceDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    const int numInputchannels = getTotalNumInputChannels();
    const int delayBufferSize = 62*(sampleRate + samplesPerBlock);
    mSampleRate = sampleRate;
    
    
    mDelayBuffer.setSize(numInputchannels, delayBufferSize);
}

void VirtualDistanceDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VirtualDistanceDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void VirtualDistanceDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //Get gain value and calculate the delay time from the distance
    gain = *parameters.getRawParameterValue(GAIN_ID);
    mDelayTime = roundToInt((*parameters.getRawParameterValue(DISTANCE_ID)/mSpeedOfSound));
    

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    const int bufferLength = buffer.getNumSamples();
    const int delayBufferLength = mDelayBuffer.getNumSamples();
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        const float* bufferData = buffer.getReadPointer(channel);
        const float* delayBufferData = mDelayBuffer.getReadPointer(channel);
        float* dryBuffer = buffer.getWritePointer(channel);
        
        fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData, gain);
        getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData, mDelayTime);
        feedbackDelay(channel, bufferLength, delayBufferLength, mDelayTime, dryBuffer, gain);
        
        
        ///GAIN PARAM FROM GUI NEEDS TO BE CONNECTED
        

    }
    
    mWritePosition += bufferLength;
    mWritePosition %= delayBufferLength;
}

void VirtualDistanceDelayAudioProcessor::fillDelayBuffer (int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData, float gain)
{
    if (delayBufferLength > bufferLength + mWritePosition)
           {
               mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, gain, gain);
           }
           else {
               const int bufferRemaining = delayBufferLength -mWritePosition;
               mDelayBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, gain, gain);
               mDelayBuffer.copyFromWithRamp(channel, 0, bufferData + bufferRemaining, bufferLength - bufferRemaining, gain, gain);
           }
    
}

void VirtualDistanceDelayAudioProcessor::getFromDelayBuffer (AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData, int delayTime)
{
    //int delayTime = 500;
    const int readPosition = static_cast<int>(delayBufferLength + mWritePosition - (mSampleRate * mDelayTime/1000)) % delayBufferLength;
    
    if (delayBufferLength > bufferLength + readPosition){
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferLength);
    }
    else{
        const int bufferRemaining = delayBufferLength - readPosition;
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
        buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining);
    }
    
}

void VirtualDistanceDelayAudioProcessor::feedbackDelay (int channel, const int bufferLength, const int delayBufferLength, int delayTime, float* dryBuffer, float gain)
{
    if (delayBufferLength > bufferLength + mWritePosition)
    {
        mDelayBuffer.addFromWithRamp(channel, mWritePosition, dryBuffer, bufferLength, gain, gain);
    }
    else
    {
        const int bufferRemaining = delayBufferLength - mWritePosition;
        
        mDelayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuffer, bufferRemaining, gain, gain);
        mDelayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining, gain, gain);
    }
}

//==============================================================================
bool VirtualDistanceDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VirtualDistanceDelayAudioProcessor::createEditor()
{
    return new VirtualDistanceDelayAudioProcessorEditor (*this);
}

//==============================================================================
void VirtualDistanceDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VirtualDistanceDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VirtualDistanceDelayAudioProcessor();
}
