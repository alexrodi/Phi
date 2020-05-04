/*
  ==============================================================================

    DSPUtils.h
    Created: 25 Apr 2020 1:16:41pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once


template <typename Type>
class DelayLine
{
public:
    void clear() noexcept
    {
        std::fill (rawData.begin(), rawData.end(), Type (0));
    }

    size_t size() const noexcept
    {
        return rawData.size();
    }

    void resize (size_t newValue)
    {
        rawData.resize (newValue);
        leastRecentIndex = 0;
    }

    Type back() const noexcept
    {
        return rawData[leastRecentIndex];
    }

    Type get (size_t delayInSamples) const noexcept
    {
        jassert (delayInSamples >= 0 && delayInSamples < size());

        return rawData[(leastRecentIndex + 1 + delayInSamples) % size()];
    }

    /** Set the specified sample in the delay line */
    void set (size_t delayInSamples, Type newValue) noexcept
    {
        jassert (delayInSamples >= 0 && delayInSamples < size());

        rawData[(leastRecentIndex + 1 + delayInSamples) % size()] = newValue;
    }

    /** Adds a new value to the delay line, overwriting the least recently added sample */
    void push (Type valueToAdd) noexcept
    {
        rawData[leastRecentIndex] = valueToAdd;
        leastRecentIndex = leastRecentIndex == 0 ? size() - 1 : leastRecentIndex - 1;
    }

private:
    std::vector<Type> rawData;
    size_t leastRecentIndex = 0;
};

inline float mixf(float outputWhenInterp0, float outputWhenInterp1, float interp)
{
    // Clip at 0-1
    interp = fmax(fmin(interp, 1.0f), 0.0f);
    
    return outputWhenInterp1 * interp + outputWhenInterp0 * ( 1.0f - interp );
};

template <typename Type>
class DCBlock
{
public:
    Type process(Type input)
    {
        previousOutput = input - previousInput + 0.995 * previousOutput;
        previousInput = input;
        return previousOutput;
    }
    void reset()
    {
        previousInput = 0.0f;
        previousOutput = 0.0f;
    }
private:
    Type previousInput = 0.0f;
    Type previousOutput = 0.0f;
};

template <typename Type>
class OnePole
{
public:
    Type process(Type input, float cutoff)
    {
        previous = mixf(input, previous, cutoff);
        return dcBlock.process(previous);
    }
    
    void reset()
    {
        previous = 0.0f;
        dcBlock.reset();
    }
private:
    Type previous = 0.0f;
    DCBlock<Type> dcBlock;
};

template <typename Type>
class Accum
{
public:
    Type process(Type input)
    {
        return input + previous;
    }
    
    void reset()
    {
        previous = 0.0f;
    }
private:
    Type previous = 0.0f;
};