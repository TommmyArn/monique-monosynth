/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

AudioSourcePlayer::AudioSourcePlayer()
    : source (nullptr),
      sampleRate (0),
      bufferSize (0),
      lastGain (1.0f),
      gain (1.0f)
{
}

AudioSourcePlayer::~AudioSourcePlayer()
{
    setSource (nullptr);
}

void AudioSourcePlayer::setSource (AudioSource* newSource)
{
    if (source != newSource)
    {
        AudioSource* const oldSource = source;

        if (newSource != nullptr && bufferSize > 0 && sampleRate > 0)
            newSource->prepareToPlay (bufferSize, sampleRate);

        {
            const ScopedLock sl (readLock);
            source = newSource;
        }

        if (oldSource != nullptr)
            oldSource->releaseResources();
    }
}

void AudioSourcePlayer::setGain (const float newGain) noexcept
{
    gain = newGain;
}

void AudioSourcePlayer::audioDeviceIOCallback (const float** inputChannelData,
        int totalNumInputChannels,
        float** outputChannelData,
        int totalNumOutputChannels,
        int numSamples)
{
    // these should have been prepared by audioDeviceAboutToStart()...
    jassert (sampleRate > 0 && bufferSize > 0);

    const ScopedLock sl (readLock);

    if (source != nullptr)
    {
        int numActiveChans = 0, numInputs = 0, numOutputs = 0;

        // messy stuff needed to compact the channels down into an array
        // of non-zero pointers..
        for (int i = 0; i < totalNumInputChannels; ++i)
        {
            if (inputChannelData[i] != nullptr)
            {
                inputChans [numInputs++] = inputChannelData[i];
                if (numInputs >= numElementsInArray (inputChans))
                    break;
            }
        }

        for (int i = 0; i < totalNumOutputChannels; ++i)
        {
            if (outputChannelData[i] != nullptr)
            {
                outputChans [numOutputs++] = outputChannelData[i];
                if (numOutputs >= numElementsInArray (outputChans))
                    break;
            }
        }

        if (numInputs > numOutputs)
        {
            // if there aren't enough output channels for the number of
            // inputs, we need to create some temporary extra ones (can't
            // use the input data in case it gets written to)
            tempBuffer.setSize (numInputs - numOutputs, numSamples,
                                false, false, true);

            for (int i = 0; i < numOutputs; ++i)
            {
                channels[numActiveChans] = outputChans[i];
                memcpy (channels[numActiveChans], inputChans[i], sizeof (float) * (size_t) numSamples);
                ++numActiveChans;
            }

            for (int i = numOutputs; i < numInputs; ++i)
            {
                channels[numActiveChans] = tempBuffer.getWritePointer (i - numOutputs);
                memcpy (channels[numActiveChans], inputChans[i], sizeof (float) * (size_t) numSamples);
                ++numActiveChans;
            }
        }
        else
        {
            for (int i = 0; i < numInputs; ++i)
            {
                channels[numActiveChans] = outputChans[i];
                memcpy (channels[numActiveChans], inputChans[i], sizeof (float) * (size_t) numSamples);
                ++numActiveChans;
            }

            for (int i = numInputs; i < numOutputs; ++i)
            {
                channels[numActiveChans] = outputChans[i];
                zeromem (channels[numActiveChans], sizeof (float) * (size_t) numSamples);
                ++numActiveChans;
            }
        }

        AudioSampleBuffer buffer (channels, numActiveChans, numSamples);

        AudioSourceChannelInfo info (&buffer, 0, numSamples);
        source->getNextAudioBlock (info);

        for (int i = info.buffer->getNumChannels(); --i >= 0;)
            buffer.applyGainRamp (i, info.startSample, info.numSamples, lastGain, gain);

        lastGain = gain;
    }
    else
    {
        for (int i = 0; i < totalNumOutputChannels; ++i)
            if (outputChannelData[i] != nullptr)
                zeromem (outputChannelData[i], sizeof (float) * (size_t) numSamples);
    }
}

void AudioSourcePlayer::audioDeviceAboutToStart (AudioIODevice* device)
{
    prepareToPlay (device->getCurrentSampleRate(),
                   device->getCurrentBufferSizeSamples());
}

void AudioSourcePlayer::prepareToPlay (double newSampleRate, int newBufferSize)
{
    sampleRate = newSampleRate;
    bufferSize = newBufferSize;
    zeromem (channels, sizeof (channels));

    if (source != nullptr)
        source->prepareToPlay (bufferSize, sampleRate);
}

void AudioSourcePlayer::audioDeviceStopped()
{
    if (source != nullptr)
        source->releaseResources();

    sampleRate = 0.0;
    bufferSize = 0;

    tempBuffer.setSize (2, 8);
}
