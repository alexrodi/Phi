/*
  ==============================================================================

    DSPUtils.h
    Created: 25 Apr 2020 1:16:41pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once


/// A mathematically accurate and waaaay faster implementation of `fmod()`
template <typename T>
inline T mod(T a, T b) noexcept {
    return a - b * floor(a / b);
}

/// Returns the fractional part of `x`
template <typename T>
inline T frac(T x) noexcept {
    return x - floor(x);
}

/// Clips `value` between `min` and `max` (both bounds inclusive)
template<typename T>
inline T clip(T value, T min, T max) noexcept {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

/// A mirrored version of the power function, where pow(-x) is equal to -pow(x)
template <typename T>
inline T bipow(T base, T exponent) noexcept
{
    T sign = (base<0.0) ? -1 : 1;
    return pow(base * sign, exponent) * sign;
}

/// Converts a MIDI note number into a frequency value in Hz (midi 69 = 440Hz)
inline float midi_to_freq(float noteNumber) noexcept
{
    return pow(2.0f, (noteNumber - 69.0f) * 0.08333333333f) * 440.0f;
}

/// Converts dB into an amplitude factor
template <std::floating_point T>
inline T db_to_a(T db) noexcept
{
    return pow(static_cast<T>(10.0), db * static_cast<T>(0.05));
}

/// Converts an amplitude factor into dB
template <std::floating_point T>
inline T a_to_db(T a) noexcept
{
    return static_cast<T>(20.0) * log10(a);
}

template <std::floating_point T>
inline T mix(T outputWhenInterp0, T outputWhenInterp1, T interp) noexcept {
    // Clip at 0-1
    interp = clip(interp, (T)0.0, (T)1.0);
    
    return outputWhenInterp1 * interp + outputWhenInterp0 * ( (T)1.0 - interp );
};

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
    
    Type getInterpolated (float delayInSamples) const noexcept
    {
        jassert (delayInSamples >= 0.0f && delayInSamples < static_cast<float>(size()));
        
        float position = static_cast<float>(leastRecentIndex + 1) + delayInSamples;
        float lowIndex = floor(position);
        size_t index = static_cast<size_t>(lowIndex);
        
        return mixf(rawData[index % size()], rawData[(index + 1) % size()], position - lowIndex);
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
        previousInput = 0.0;
        previousOutput = 0.0;
    }
private:
    Type previousInput = 0.0;
    Type previousOutput = 0.0;
};

template <typename Type>
class OnePole
{
public:
    Type process(Type input, float cutoff)
    {
        previous = mix(input, previous, cutoff);
        return dcBlock.process(previous);
    }
    
    Type processInverse(Type input, float cutoff)
    {
        return input - process(input, cutoff);
    }
    
    void reset()
    {
        previous = 0.0;
        dcBlock.reset();
    }
private:
    Type previous = 0.0;
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
        previous = 0.0;
    }
private:
    Type previous = 0.0;
};
