/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

// TODO: try shape buttons
// TODO: add toggle functionality, by making the sequencer only changing the color of the active toggled buttons

#include "MainComponent.h"

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
    
//    addAndMakeVisible (test);
//    juce::Path path;
//    path.addEllipse(0, 0, 100, 100);
//    test.setShape(path, false, false, false);
    
    for (int corner = 0; corner < maxCorners; corner++)
    {
        addAndMakeVisible(sequencerPointsArray[corner]);
        sequencerPointsArray[corner].setClickingTogglesState(true);
        sequencerPointsArray[corner].setToggleState(true, false);
    }
    
    pointsAmountInputSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    pointsAmountInputSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
    pointsAmountInputSlider.setRange(3, maxCorners, 1);
    pointsAmountInputSlider.setValue(pointsAmount);
    addAndMakeVisible(pointsAmountInputSlider);
    pointsAmountInputSlider.onValueChange = [this] { sliderValueChanged (&pointsAmountInputSlider);   };
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
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
    //    float circumference = 2 * radius * MathConstants<float>::pi;
    
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
        sequencerPointsArray[point].setVisible(true);
        sequencerPointsArray[point].setBounds(0, 0, 20, 20);
        sequencerPointsArray[point].setCentrePosition(xValue, yValue);
        
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
        sequencerPointsArray[point].setVisible(false);
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
//    test.setBounds(0, 0, 10, 50);
}

void MainComponent::updateToggleState (Button* button)
{
    fillPath = !fillPath;
    repaint();
}

void MainComponent::sliderValueChanged(Slider *slider)
{
    pointsAmount = slider->getValue();
    repaint();
}

void MainComponent::timerCallback()
{
    // walk through sequencer
    currentBeat = (currentBeat + 1) % (int)pointsAmount;
    for (int point = 0; point < pointsAmount; point++)
    {
        if (sequencerPointsArray[point].getToggleState() == true)
        {
            if (currentBeat == point)
            {
                // set color
                sequencerPointsArray[point].setColour(TextButton::buttonOnColourId, red);
            }
            else {
                // set color
                sequencerPointsArray[point].setColour(TextButton::buttonOnColourId, aqua);
            }
        }
    }
    repaint();
}
