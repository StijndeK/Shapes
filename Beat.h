/*
  ==============================================================================

    Beat.h
    Created: 30 Jul 2020 7:26:46pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelopes.h"

class Beat {
    
public:
    Envelopes env;
    bool trigger;
    TextButton sequencerPoint;
};
