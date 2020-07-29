/*
  ==============================================================================

    Envelopes.cpp
    Created: 29 Jul 2020 11:37:29pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "Envelopes.h"

double Envelopes::arExp(double input, int trigger)
{
    switch(currentEnvState) {
        case ATTACK:
            amplitude *= attackExp;
            if (amplitude >= 1) {
                amplitude = 1;
                currentEnvState = HOLD;
            }
            break;
        case HOLD:
            amplitude = 1;
            if (trigger != 1) {
                currentEnvState = RELEASE;
            }
            break;
        case RELEASE:
            amplitude *= releaseExp;
            if (amplitude <= amplitudeStartValue) {
                amplitude = 0;
                currentEnvState = STOP;
            }
            break;
        case STOP:
            amplitude = 0.0;
            if (trigger == 1) {
                amplitude = amplitudeStartValue;
                currentEnvState = ATTACK;
            }
            break;
    }

    output = input * amplitude;
    return output;
}

void Envelopes::setTimeExp(double attackMs, double releaseMs)
{
    attackExp = pow((1.0/amplitudeStartValue), 1.0 / (samplerate * (attackMs / 1000.0)));
    releaseExp = pow((amplitudeStartValue/1), 1.0/ (samplerate * (releaseMs / 1000.0)));
}
