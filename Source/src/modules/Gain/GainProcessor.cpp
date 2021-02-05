/*
  ==============================================================================

    GainProcessor.cpp
    Created: 25 Apr 2020 11:55:42pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "GainProcessor.h"
#include "GainUI.h"


GainProcessor::GainProcessor() :
ModuleProcessor( 1, 1,
    std::make_unique<AudioParameterFloat> (
                                           "gain",
                                           "Gain",
                                           NormalisableRange<float> (-70.0f, 12.0f),
                                           0.0f,
                                           "Gain",
                                           AudioParameterFloat::genericParameter,
                                           [](float value, int) { return String (value, 1); },
                                           [](const String& text) { return text.getFloatValue(); }
                                           )
                )
{
}

GainProcessor::~GainProcessor()
{
}

void GainProcessor::prepareToPlay (double, int)
{
   
}
void GainProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
   buffer.applyGain( *params.getRawParameterValue("gain") );
}
void GainProcessor::releaseResources()
{
}

std::unique_ptr<ModuleUI> GainProcessor::createUI()
{
    return std::make_unique<GainUI>(*this);
}
