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

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VirtualDistanceDelayAudioProcessorEditor::VirtualDistanceDelayAudioProcessorEditor (VirtualDistanceDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    gainSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, GAIN_ID, gain);
    
        distanceSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, DISTANCE_ID, distance);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //setResizable(true, true);
    //setResizeLimits(800, 100, 1000, 1000);
    setSize(1000, 400);
    
    
    // these define the parameters of our slider object
    distance.setSliderStyle (Slider::LinearHorizontal);
    distance.setRange(0.0, 21000, 1.0);
    distance.setSkewFactorFromMidPoint(1000);
    distance.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    distance.setPopupDisplayEnabled (false, false, this);
    distance.setTextValueSuffix (" Distance (m)");
    distance.setValue(*processor.parameters.getRawParameterValue(DISTANCE_ID));
    
    distance.addListener(this);
    
    gain.setSliderStyle (Slider::RotaryVerticalDrag);
    gain.setRange(0.0, 1.0, 0.01);
    gain.setTextBoxStyle (Slider::NoTextBox, false, 90, 0);
    gain.setPopupDisplayEnabled (true, true, this);
    gain.setTextValueSuffix (" Gain");
    gain.setValue(*processor.parameters.getRawParameterValue(GAIN_ID));
    
    gain.addListener(this);
    
    milliSeconds.setColour(Label::textColourId, Colours::lightgrey);
    milliSeconds.setJustificationType (Justification::centred);
    milliSeconds.setText(static_cast<String>(processor.mDelayTime), dontSendNotification);
    
    snapInfo.setColour(Label::textColourId, Colours::lightgrey);
    snapInfo.setJustificationType (Justification::centred);
    setDistanceInfo(*processor.parameters.getRawParameterValue(DISTANCE_ID));
    //snapInfo.setText("-", dontSendNotification);
    
    gainInfo.setColour(Label::textColourId, Colours::lightgrey);
    gainInfo.setJustificationType (Justification::centred);
    gainInfo.setText(static_cast<String>(processor.gain), dontSendNotification);
    
    
    snapBtn.setButtonText("Snap to markers");
    snapBtn.setToggleState(snapBtnState, dontSendNotification);
    snapBtn.addListener(this);
    
    infoBtn.setButtonText("info");
    infoBtn.addListener(this);
    
    infobox.setMultiLine(true);
    infobox.setReadOnly(true);

    infobox.setColour(TextEditor::backgroundColourId,Colours::grey);
    String infoText;
    infoText << "Simple Delay Plugin developped by: ";
    infoText << newLine;
    infoText << "Johannes Schmitt - johannes.schmitt@hotmail.de";
    infoText << newLine;
    infoText << "> for .strandberg* Spark Topic 50 <";
    infoText << newLine;
    infoText << newLine;
    infoText << "#################################################################################################";
    infoText << newLine;
     infoText << "  If you like this work and want to support me you can gift me a beer or two via Paypal: https://www.paypal.me/derguitarjo  ";
    infoText << newLine;
      infoText << "#################################################################################################";
    infoText << newLine;
    infoText << newLine;
    infoText << "The code is based on 'Building a Delay Plugin by The Audio Programmer'";
    infoText << newLine;
    infoText << newLine;
    infoText << "Icon credits:";
    infoText << newLine;
    infoText << "www.flaticon.com with following creators:";
    infoText << newLine;
    infoText << "Freepik: Distancing, Golden Gate Bridge, Mt. Everest, Tropopause";
    infoText << newLine;
    infoText << "Nikita Golubev: Statue of Liberty";
    infoText << newLine;
    infoText << "ultimatearm: Burj Khalifa";
    infoText << newLine;
    infoText << "monkik: Eiffel Tower";
    infoText << newLine;
    infoText << newLine;
    infoText << newLine;
    infoText << "Also please check out www.strandbergguitars.com";
    infoText << newLine;
    infoText << "And follow me on Instagram: derguitarjo";
    infoText << newLine;
    infoText << newLine;
    infoText << newLine;
    infoText << "#######################";
    infoText << newLine;
    infoText << "## LICENSE INFORMATION ##";
    infoText << newLine;
    infoText << "#######################";
    infoText << newLine;
    infoText <<"This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.";
    infoText << newLine;
    infoText << newLine;
    infoText << "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.";
    infoText << newLine;
    infoText << newLine;
    infoText << "You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.";
    infoText << newLine;
    infoText << newLine;
    infoText << "Dieses Programm ist Freie Software: Sie können es unter den Bedingungen der GNU General Public License, wie von der Free Software Foundation, Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren veröffentlichten Version, weiter verteilen und/oder modifizieren.";
    infoText << newLine;
    infoText << newLine;
    infoText << "Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich sein wird, jedoch OHNE JEDE GEWÄHR,; sogar ohne die implizite Gewähr der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Siehe die GNU General Public License für weitere Einzelheiten.";
    infoText << newLine;
    infoText << newLine;
    infoText << "Sie sollten eine Kopie der GNU General Public License zusammen mit diesem Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.";
    
    
    infobox.setText (infoText, dontSendNotification);
    
    
 
    // this function adds the slider to the editor
    addAndMakeVisible (&distance);
    addAndMakeVisible (&gain);
    addAndMakeVisible(&milliSeconds);
    addAndMakeVisible (&snapInfo);
    addAndMakeVisible (&snapBtn);
//    addAndMakeVisible(&gainInfo);
    addAndMakeVisible(&infoBtn);
    
    addAndMakeVisible(&infobox);
    infobox.setVisible(false);
    

}

VirtualDistanceDelayAudioProcessorEditor::~VirtualDistanceDelayAudioProcessorEditor()
{
}

//==============================================================================
void VirtualDistanceDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    
    Image background = ImageCache::getFromMemory (BinaryData::Background_png, BinaryData::Background_pngSize);
    g.drawImageAt (background, 0, 0);
    
}

void VirtualDistanceDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    distance.setBounds(20, 200, getWidth()-40, 30);
    gain.setBounds(getWidth()/2- 50, 300, 100, 100);
    milliSeconds.setBounds(getWidth()/2-25, 25, 50, 10);
    snapBtn.setBounds(10, getHeight()-30, 200, 20);
    snapInfo.setBounds(getWidth()/2-100, 255, 200, 20);
//    gainInfo.setBounds(getWidth()/2-15, 370, 30, 20);
    
    infoBtn.setBounds(getWidth()-50, getHeight()-30, 40, 20);
    
    infobox.setBounds(30, 30, getWidth()-60, getHeight()-60);
    //gain.setBounds(<#int x#>, <#int y#>, <#int width#>, <#int height#>)
}

void VirtualDistanceDelayAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if(slider == &gain)
    {
        gainInfo.setText(static_cast<String>(processor.gain), dontSendNotification);
    }
    else {}
    
    if(slider == &distance)
    {
        double oldSliderValue = distance.getValue();
        double newSliderValue = snapToObjects(oldSliderValue);
        
        //The following calculation should be done within the processor
//        processor.mDelayTime = roundToInt(newSliderValue/processor.mSpeedOfSound);
        
        distance.setValue(newSliderValue);
        
        milliSeconds.setText(static_cast<String>(processor.mDelayTime), dontSendNotification);
        setDistanceInfo(static_cast<int>(newSliderValue));
        
        
    }
    else {}

}

void VirtualDistanceDelayAudioProcessorEditor
::setDistanceInfo(int sliderValue)
{
    switch (sliderValue) {
        case 2:
            snapInfo.setText("Social Distancing - 2 m", dontSendNotification);
            break;
        case 93:
            snapInfo.setText("Statue of Liberty - 93 m", dontSendNotification);
            break;
        case 300:
            snapInfo.setText("Eiffel Tower - 300 m", dontSendNotification);
            break;
        case 828:
            snapInfo.setText("Burj Khalifa - 828 m", dontSendNotification);
            break;
        case 2737:
            snapInfo.setText("Golden Gate Bridge - 2737 m", dontSendNotification);
            break;
        case 8848:
            snapInfo.setText("Mount Everest - 8848 m", dontSendNotification);
            break;
        case 15000:
            snapInfo.setText("Tropopause - 15000 m", dontSendNotification);
            break;
        default:
                snapInfo.setText(static_cast<String>(sliderValue) + " m", dontSendNotification);
            break;
    }
}

void VirtualDistanceDelayAudioProcessorEditor::buttonClicked (Button* button)
{
    if(button == &snapBtn){
        snapBtnState = !snapBtnState;
        snapBtn.setToggleState(snapBtnState, dontSendNotification);
    }
    if(button == &infoBtn){
        infobox.setVisible(!infobox.isVisible());
    }
    
}

double VirtualDistanceDelayAudioProcessorEditor::snapToObjects (double SliderValue)
{
    if (snapBtnState == true)
    {
        switch (static_cast<int>(SliderValue)) {
            case 1 ... 10:
                return 2;
                break;
            case 75 ... 110:
                return 93;
                break;
            case 250 ... 350:
                return 300;
                break;
            case 700 ... 949:
                return 828;
                break;
            case 2200 ... 3199:
                return 2737;
                break;
            case 7800 ... 9999:
                return 8848;
                break;
            case 14000 ... 16000:
                return 15000;
                break;
            default:
                return SliderValue;
                break;
        }
    }
    else{
        return SliderValue;
    }
}
