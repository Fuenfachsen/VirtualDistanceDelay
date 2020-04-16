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

#pragma once

#include <JuceHeader.h>
#define GAIN_ID "gain01"
#define GAIN_NAME "Gain"
#define DISTANCE_ID "distance01"
#define DISTANCE_NAME "Distance"

//==============================================================================
/**
*/
class VirtualDistanceDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    VirtualDistanceDelayAudioProcessor();
    ~VirtualDistanceDelayAudioProcessor();

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
    void fillDelayBuffer (int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData, float gain);
    void getFromDelayBuffer (AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData, int delayTime);
    void feedbackDelay (int channel, const int bufferLength, const int delayBufferLength, int delayTime, float* dryBuffer, float gain);
    
    double gain { 0.5 };
    double mDelayTime { 300 };
    float mSpeedOfSound { 0.3435 };  // m/ms
    
    AudioProcessorValueTreeState parameters;

private:
    
    AudioBuffer<float> mDelayBuffer;
    int mWritePosition { 0 };
    int mSampleRate { 44100 };
    

//    float previousGain;
//
    std::atomic<float>* gainParam = nullptr;
    std::atomic<float>* distanceParam = nullptr;
//
//    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VirtualDistanceDelayAudioProcessor)
};
