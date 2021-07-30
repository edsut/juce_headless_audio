/*
  ==============================================================================

    justaudio.cpp
    Created: 26 Jul 2021 3:58:25pm
    Author:  els

  ==============================================================================
*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "justaudio.h"

justaudio::justaudio()
{
    std::cout << __func__ << std::endl;
}

justaudio::~justaudio()
{
    std::cout << __func__ << std::endl;
}

bool
justaudio::init(struct justaudioinfo *justaudioip)
{
    juce::String resp;

    NMIC = justaudioip->mictot;
    NSPKR = justaudioip->spkrtot;
    FSAMP = justaudioip->sampfreq;
    HWBUFFER_SIZE = justaudioip->bufsz;
    AudioDevice = justaudioip->devname;

    std::cout << "justaudio::init(" << AudioDevice << ")..." << std::endl;
    std::cout << "  nmic: " << NMIC << ", nspkr: " << NSPKR << std::endl;
    std::cout << "  sample-freq: " << FSAMP << ", bufsz: " << HWBUFFER_SIZE << std::endl;

    resp = audMgr.initialise(NMIC,NSPKR,nullptr,false);
    if (resp != "") {
        std::cout << "audMgr.initialize() failed: " << resp << std::endl;
        return(false);
    }

    audSetup = audMgr.getAudioDeviceSetup();
    audSetup.inputDeviceName = (NMIC > 0) ? AudioDevice : "";
    audSetup.useDefaultInputChannels = false;
    audSetup.outputDeviceName = (NSPKR > 0) ? AudioDevice : "";
    audSetup.useDefaultOutputChannels = false;
    audSetup.bufferSize = HWBUFFER_SIZE;
    audSetup.sampleRate = FSAMP;
    
    resp = audMgr.setAudioDeviceSetup(audSetup, true);
    if (resp != "") {
        std::cout << "audMgr.setAudioDeviceSetup() failed: " << resp << std::endl;
        return(false);
    }

    // This gets things rolling with our callbacks...
    audMgr.addAudioCallback(this);

    return(true);
}

void
justaudio::shutdown()
{
    std::cout << __func__ << std::endl;
    
    audMgr.closeAudioDevice();
    audMgr.removeAudioCallback(this);
}

void
justaudio::audioDeviceAboutToStart (juce::AudioIODevice*)
{
    std::cout << __func__ << std::endl;
}

void 
justaudio::audioDeviceStopped()
{
    std::cout << __func__ << std::endl;
    std::cout << "audioDeviceIOCallback was called " << cbtot << " times" << std::endl;
}

void 
justaudio::audioDeviceError(const juce::String &errmsg)
{
    std::cout << __func__ << ": " << errmsg << std::endl;
}

void 
justaudio::audioDeviceIOCallback (const float** inData, int inChTot, float** outData, int outChTot, int sampleTot)
{
    float *lOut, *rOut;
    const float *lIn, *rIn;

    cbtot++;

    lOut = outData[0];
    rOut = outData[1];
    lIn = inData[0];
    rIn = inData[1];

    for(int i=0;i<sampleTot;i++) {
        *lOut++ = *lIn++;
        *rOut++ = *rIn++;
    }
}
