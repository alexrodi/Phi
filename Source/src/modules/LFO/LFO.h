/*
  ==============================================================================

    LFO.h
    Created: 20 Aug 2025 3:48:48pm
    Author:  Alexandre Rodrigues

  ==============================================================================
*/

#pragma once

struct LFO {
    static inline const juce::StringArray NOTE_STRINGS = {
        "8",
        "4",
        "3",
        "2",
        "3 / 2",
        "1",
        "3 / 4",
        "1 / 2",
        "1 / 4",
        "1 / 6",
        "1 / 8",
        "1 / 12",
        "1 / 16",
        "1 / 32",
        "1 / 64"
    };
    
    static constexpr std::array<double, 15> NOTE_VALUES = {
        32.0,
        16.0,
        12.0,
        8.0,
        6.0,
        4.0,
        3.0,
        2.0,
        1.0,
        2.0 / 3.0,
        1.0 / 2.0,
        1.0 / 3.0,
        1.0 / 4.0,
        1.0 / 8.0,
        1.0 / 16.0
    };
    
    static inline const juce::StringArray WAVE_STRINGS = {"Sine", "Triangle", "Square", "Random"};
    
    enum class Wave { Sine, Triangle, Square, Random };
    
    static int rate_to_note_index(double rate) {
        return std::min((int)floor(rate * (double)NOTE_VALUES.size()), (int)NOTE_VALUES.size() - 1);
    }
    
    static juce::String rate_to_note_string(double rate) {
        return NOTE_STRINGS[rate_to_note_index(rate)];
    }
    
    static double rate_to_hz(double rate) { rate += 0.1; return (rate*rate*rate*rate) * 68.3013455365; }
    
    static double note_rate_to_hz(double rate, double bpm) { return bpm / (NOTE_VALUES[rate_to_note_index(rate)] * 60.0); }
    
    void prepare (double sample_rate) {
        increment = 1.0 / sample_rate;
    }
    
    void set_note_mode(bool is_note_mode) { note_mode = is_note_mode; update_freq(); }
    
    void set_bpm(double new_bpm) { bpm = new_bpm > 1.0 ? new_bpm : 1.0; update_freq(); }
    
    /// [0 - 1]
    void set_rate(double new_rate) { rate = new_rate; update_freq(); }
    
    void set_wave(Wave new_wave) { wave = new_wave; }
    
    /// [-1 - 1]
    void set_shape(float new_shape) { shape = new_shape; }
    
    /// [0 - 1]
    void set_phase(float new_phase) { phase = new_phase; }
    
    /// Sets the LFO position in phase units
    /// Call on audio thread only!
    void set_position(double new_position) {
        position = mod(new_position, 1.0);
    }
    
    /// Sets the LFO position in quarter-note units
    /// Call on audio thread only!
    void set_ppq_position(double ppq_position) {
        set_position((ppq_position / 4.0 / 60.0) * bpm * freq);
    }
    
    float next() {
        float value = 0.0f;
        float pos = position + phase;
        double next_position = position + freq * increment;
        
        if (wave == Wave::Sine)
            value = get_sine(pos, shape);
        else if (wave == Wave::Triangle)
            value = get_triangle(pos, shape);
        else if (wave == Wave::Square)
            value = get_square(pos, shape);
        else /* (wave == Wave::Random) */ {
            value = get_random(random_value, next_random_value, pos, shape);
            
            if (next_position >= 1.0) {
                random_value = next_random_value;
                next_random_value = rng.nextFloat() * 2.0f - 1.0f;
            }
        }
        
        position = mod(next_position, 1.0);
        
        return value;
    }
    
    void skip(int num_samples) {
        double next_position = position + freq * increment * (double)num_samples;
        
        if (wave == Wave::Random && next_position >= 1.0) {
            random_value = next_random_value;
            next_random_value = rng.nextFloat() * 2.0f - 1.0f;
        }
        
        position = mod(next_position, 1.0);
    }
    
    static float get_sine (float pos, float shape) {
        float sine = sin(pos * two_pi);
        float warped;
        
        if (shape > 0.0f) {
            warped = tanh(sine * (pow(shape, 10.0f) + 0.2f) * 20.0f);
            return sine * (1.0f - abs(shape)) + warped * abs(shape);
        } else {
            float sign = sine >= 0.0f ? 1.0f : -1.0f;
            float exponent = shape*shape*shape*shape * 100.0f + 1.0f;
            return sign * pow(abs(sine), exponent);
        }
    }
    
    static float get_triangle (float pos, float shape) {
        pos = frac(pos);
        
        float a = 0.5f + shape * 0.5f;
        
        float y = pos < a ? pos / a : (1.0f - pos) / (1.0f - a);
        
        return y * 2.0f - 1.0f;
    }
    
    static float get_square (float pos, float shape) {
        return mod(pos, 1.0f) < (shape * 0.49f + 0.5f) ? 1.0f : -1.0f;
    }
    
    static float get_random (float value, float next_value, float pos, float shape) {
        pos = frac(pos);
        shape = bipow(shape, 2.0f);
        
        float mix = (shape >= 0.0f) ? 1.0f - pow(1.0f - pos, 1.0f/shape) : pow(pos, -shape);
        
        return value * (1.0f - mix) + next_value * mix;
    }
    
private:
    static constexpr float two_pi = 2.0f * 3.141592653589793238f;
    
    double increment = 0.0, position = 0.0, rate = 0.0, bpm = 1.0;
    float random_value = 0.0f, next_random_value = 0.0f;
    std::atomic<float> shape = 0.0f, phase = 0.0f;
    std::atomic<bool> note_mode = false;
    std::atomic<double> freq = 0.0;
    std::atomic<Wave> wave;
    juce::Random rng;
    
    void update_freq() {
        if (note_mode)
            freq = note_rate_to_hz(rate, bpm);
        else
            freq = rate_to_hz(rate);
    }
};
