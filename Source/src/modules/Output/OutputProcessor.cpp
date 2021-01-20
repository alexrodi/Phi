/*
  ==============================================================================

    OutputProcessor.cpp
    Created: 25 Apr 2020 12:10:44am
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#include "OutputProcessor.h"
#include "OutputUI.h"

OutputProcessor::OutputProcessor() :
ModuleProcessor( 2, 2 ) // Output cheats on the number of output channels, the two outs are only used to patch it to the actual output device
{
}

OutputProcessor::~OutputProcessor()
{
}

void OutputProcessor::prepareToPlay (double, int)
{
}
void OutputProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{
}
void OutputProcessor::releaseResources()
{
}

std::unique_ptr<ModuleUI> OutputProcessor::createUI()
{
    return std::make_unique<OutputUI>(*this);
}

