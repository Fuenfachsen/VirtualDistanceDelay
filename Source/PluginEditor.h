/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.
 
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
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class VirtualDistanceDelayAudioProcessorEditor  : public AudioProcessorEditor,
                                                private Slider::Listener,
                                                private Button::Listener
{
public:
    VirtualDistanceDelayAudioProcessorEditor (VirtualDistanceDelayAudioProcessor&);
    ~VirtualDistanceDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VirtualDistanceDelayAudioProcessor& processor;
    
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked (Button* button) override;
    double snapToObjects (double SliderValue);
    
    Slider distance;
    Slider gain;
    Label milliSeconds;
    Label snapInfo;
    Label gainInfo;
    ToggleButton snapBtn;
    TextButton infoBtn;
    bool snapBtnState { true };
    
    TextEditor infobox;
    
    void setDistanceInfo(int sliderValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VirtualDistanceDelayAudioProcessorEditor)
    
    //Needed to prevent crashes because of order of destruction of objects
public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> gainSliderValue;
        std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> distanceSliderValue;
};
