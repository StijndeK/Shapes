/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

// TODO: Env en sound maken per ding (polyfoon)
// TODO: custom class for every corner element (trigger, env, buttons)

//==============================================================================
MainComponent::MainComponent()
{
    setSize (300, 350);
    
    // timer for sequencer
    Timer::startTimer(bpmInMs);

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
    
    addAndMakeVisible (fillButton);
    fillButton.setButtonText ("Fill");
    fillButton.onClick = [this] { updateToggleState (&fillButton);   };
    
    for (int corner = 0; corner < maxCorners; corner++)
    {
        addAndMakeVisible(beats[corner].sequencerPoint);
        beats[corner].sequencerPoint.setClickingTogglesState(true);
        beats[corner].sequencerPoint.setToggleState(true, false);
        
        beats[corner].env.setTimeExp(20, 200);
    }
    
    pointsAmountInputSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    pointsAmountInputSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
    pointsAmountInputSlider.setRange(3, maxCorners, 1);
    pointsAmountInputSlider.setValue(pointsAmount);
    addAndMakeVisible(pointsAmountInputSlider);
    pointsAmountInputSlider.onValueChange = [this] { sliderValueChanged (&pointsAmountInputSlider);   };
    
    bpmSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    bpmSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    bpmSlider.setRange(40, 500, 1);
    bpmSlider.setValue(120);
    addAndMakeVisible(bpmSlider);
    bpmSlider.onValueChange = [this] { sliderValueChanged (&bpmSlider);   };
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
        currentSampleRate = sampleRate;
        updateAngleDelta();
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    auto level = 0.125f;
    auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto currentSample = (float) std::sin (currentAngle);
        currentAngle += angleDelta;
        
        double theSound = 0;
        for (int point = 0; point < pointsAmount; point++)
        {
            theSound += beats[point].env.arExp(currentSample * level, beats[point].trigger); // pass sound through envelope
            beats[point].trigger = false; // reset trigger TODO: only do this when necessary
        }
                
        leftBuffer[sample]  = theSound;
        rightBuffer[sample] = theSound;
    }
}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // set colours
    g.fillAll (darkBack);
    
    getLookAndFeel().setColour(Slider::thumbColourId, aqua);
    getLookAndFeel().setColour(Slider::backgroundColourId, lightBlue);
    getLookAndFeel().setColour(Slider::trackColourId, blue);
    getLookAndFeel().setColour(Slider::textBoxTextColourId, lightText);
    
    getLookAndFeel().setColour(TextButton::buttonColourId, darkBack);
    getLookAndFeel().setColour(TextButton::buttonOnColourId, aqua);
    getLookAndFeel().setColour(TextButton::textColourOffId, lightText); // text colour
        
    // -------------------------------------------------------------------------
    
    double theta = MathConstants<double>::twoPi;
    int centrePoint = getWidth() / 2;
    
    // create and draw circle
    double radius = getWidth() / 2.5;
    juce::Rectangle<float> circle (radius * 2, radius * 2);
    circle.setCentre(centrePoint, centrePoint); // centre the circle around 100, 100
    g.setColour (lightBlue);
    g.setOpacity(0.5);
    g.drawEllipse(circle, 2);
    
    // area to draw points
    juce::Rectangle<float> pointArea (4, 4);
        
    // calculate points and draw the figure
    juce::Path path;
    
    for (int point = 0; point < pointsAmount; point++)
    {
        double angle = theta / pointsAmount * (double)point;
        double xValue = radius * cos(angle) + centrePoint;
        double yValue = radius * sin(angle) + centrePoint;
        
        g.setColour (red);
        pointArea.setCentre(xValue, yValue);
        g.setColour (red);
        g.fillEllipse(pointArea);
        
        // set toggles on corners
        beats[point].sequencerPoint.setVisible(true);
        beats[point].sequencerPoint.setBounds(0, 0, 20, 20);
        beats[point].sequencerPoint.setCentrePosition(xValue, yValue);
        
        // line or fill
        if (point == 0)
        {
            path.startNewSubPath (xValue, yValue);
        }
        else {
            path.lineTo(xValue, yValue);
        }
    }
    
    // remove unused toggles
    for (int point = pointsAmount; point < maxCorners; point ++)
    {
        beats[point].sequencerPoint.setVisible(false);
    }
    
    path.closeSubPath ();
    
    g.setColour (pink);
    if (fillPath) {
        g.strokePath(path, PathStrokeType(2));
    }
    else {
        g.fillPath(path);
    }
}

void MainComponent::resized()
{
    fillButton.setBounds(getWidth() - 50, 0, 50, 25);
    pointsAmountInputSlider.setBounds(0, getHeight() - 70, getWidth(), 50);
    bpmSlider.setBounds(0, 0, 50, 60);
}

void MainComponent::updateToggleState (Button* button)
{
    fillPath = !fillPath;
    trigger = !trigger;
    repaint();
}

void MainComponent::sliderValueChanged(Slider *slider)
{
    if (slider == &pointsAmountInputSlider)
    {
        pointsAmount = slider->getValue();
        repaint();
    }
    else {
        bpmInMs = 60000 / slider->getValue();
        Timer::startTimer(bpmInMs);
    }
}

void MainComponent::timerCallback()
{
    // walk through sequencer
    currentBeat = (currentBeat + 1) % (int)pointsAmount;
    for (int point = 0; point < pointsAmount; point++)
    {
        if (beats[point].sequencerPoint.getToggleState() == true)
        {
            if (currentBeat == point)
            {
                beats[point].sequencerPoint.setColour(TextButton::buttonOnColourId, red);
                beats[point].trigger = true;
            }
            else {
                beats[point].sequencerPoint.setColour(TextButton::buttonOnColourId, aqua);
            }
        }
    }
    repaint();
}

//==============================================================================

void MainComponent::updateAngleDelta()
{
    int frequency = 500;
    auto cyclesPerSample = frequency / currentSampleRate;         //  First we calculate the number of cycles that will need to complete for each output sample.
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;          // Then this is multiplied by the length of a whole sine wave cycle, which is 2pi radians.
}
