/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

// TODO:  morph functie, rotatie, kleuren veranderen door de slider heen, sequencer
// Sequencer: timer maken die nieuw geluid en lichtje op een rand aanroept. Toggles op de randjes zetten om aan en uit te zetten of er een geluid gespeeld moet worden

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (300, 350);

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);
    
    addAndMakeVisible (fillButton);
    fillButton.setButtonText ("Fill");
    fillButton.onClick = [this] { updateToggleState (&fillButton);   };
    
    addAndMakeVisible(pointsAmountInput);
    pointsAmountInput.setInputRestrictions(2, "0123456789");
    
    pointsAmountInputSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    pointsAmountInputSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
    pointsAmountInputSlider.setRange(3, 14, 1);
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
    g.fillAll (darkBack);
    
    getLookAndFeel().setColour(Slider::thumbColourId, aqua);
    getLookAndFeel().setColour(Slider::backgroundColourId, lightBlue);
    getLookAndFeel().setColour(Slider::trackColourId, blue);
    getLookAndFeel().setColour(Slider::textBoxTextColourId, lightText);
    
    getLookAndFeel().setColour(TextButton::buttonColourId, Colour(10, 13, 39));
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
        
        pointArea.setCentre(xValue, yValue);
        g.setColour (red);
        g.fillEllipse(pointArea);
        
        if (point == 0)
        {
            path.startNewSubPath (xValue, yValue);
        }
        else {
            path.lineTo(xValue, yValue);
        }
    }
    
//    Drawable* img = Drawable::createFromSVG(*svg);
//    img->setTransform(AffineTransform::rotation(angle, svg_width / 2, svg_height / 2)
//            .scaled(rw/svg_width, rw/svg_height, svg_width/2, svg_height/2));
//    img->drawAt(g, centreX - (svg_width / 2), centreY - (svg_height / 2), 1.0f);
    
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
//    pointsAmountInput.setBounds(0, 0, 50, 50);
    pointsAmountInputSlider.setBounds(0, getHeight() - 70, getWidth(), 50);
}

void MainComponent::mouseDown (const juce::MouseEvent&)
{
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
