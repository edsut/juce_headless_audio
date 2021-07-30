/*
  ==============================================================================

    justaudio.h
    Created: 26 Jul 2021 3:58:25pm
    Author:  els

  ==============================================================================
*/

#pragma once

#include<JuceHeader.h>

struct justaudioinfo {
    juce::String devname;   // Audio device name
    int mictot;             // Number of mics
    int spkrtot;            // Number of speakers
    int bufsz;              // Size of buffer
    int sampfreq;           // Sample frequency
    bool verbose;           // Verbosity on/off
};

class justaudio:   public juce::AudioIODeviceCallback
{
public:
    justaudio();
    ~justaudio();

    bool init(struct justaudioinfo *justaudioip);
    void shutdown();

    void audioDeviceAboutToStart (juce::AudioIODevice*) override; 
    void audioDeviceStopped() override;
    void audioDeviceError(const juce::String &errorMessage) override;
    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels,
                                int numberOfSamples) override;

    juce::AudioDeviceManager audMgr;
    juce::AudioDeviceManager::AudioDeviceSetup audSetup;
    int connect_to_server(const char *ip, int pt);

private:
    int cbtot;
    int NMIC = 0;
    int NSPKR = 0;
    int FSAMP;
    int HWBUFFER_SIZE;
    juce::String AudioDevice;
};

