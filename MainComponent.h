/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    void updateToggleState (Button* button);
    void sliderValueChanged (Slider* slider);
    void timerCallback() override;

private:
    //==============================================================================
    TextButton fillButton;
    Slider pointsAmountInputSlider;

    double pointsAmount = 5;
    
    bool fillPath = true;
    
    int bpmInMs = 600; // 60.000 / bpm(100)
    
    int maxCorners = 14;
    TextButton sequencerPointsArray[14];
    
    int currentBeat = 0;
        
//    ShapeButton test {"test", Colours::white, Colours::white, Colours::white};
    
    Colour darkBack = Colour(10, 13, 39);
    Colour lightText = Colour(255, 255, 255);
    Colour pink = Colour(204, 76, 121);
    Colour lightBlue = Colour(98, 184, 208);
    Colour aqua = Colour(73, 170, 197);
    Colour blue = Colour(57, 105, 160);
    Colour darkBlue = Colour(40, 73, 127);
    Colour red = Colour(196, 74, 78);
    
    //------------------------------------------------------------

    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
    void updateAngleDelta();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
