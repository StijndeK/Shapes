/*
  ==============================================================================

    Envelopes.h
    Created: 29 Jul 2020 11:36:37pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class Envelopes {
public:
    
    double arExp(double input, int trigger);    // exponential curve ar envelope
    void setTimeExp(double attackMs, double releaseMs);

private:
    
    enum EnvState {ATTACK, HOLD, RELEASE, STOP};    // envelope states
    EnvState currentEnvState = STOP;     // initial state

    double attackExp;
    double releaseExp;
    double amplitudeStartValue = 0.001;     // define the startvalue for the exponential curve
    double amplitude = 0.001;
    double output;
    double samplerate = 44100.0;
};
