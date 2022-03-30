/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

void processBuffer(AudioBuffer<float> *buffer)
{
    int numberOfChannels = buffer->getNumChannels();
    int numberOfSamples = buffer->getNumSamples();
    
    for (int c=0; c<numberOfChannels; c++)
    {
        const float* input = buffer->getReadPointer(c);
        float* output = buffer->getWritePointer(c);
        for (int n=0; n<numberOfSamples; n++)
        {
            // Example of the functions
            output[n] = input[n]*0.5f;
        }
    }
}


int main ()
{
    String inputFileName("nobody.wav");
    String outputFileName("nobody-output.wav");

    // create an input file object
    File inputFile = File::getSpecialLocation(File::userDesktopDirectory);
    inputFile = inputFile.getChildFile(inputFileName);
    
    if (inputFile.existsAsFile() == false)
    {
        std::cout << "Sorry " << inputFile.getFullPathName() << " does not exist!" << std::endl;
        return 2;
    }
    
    // Initialise the decoders
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    // Read the audio file
    AudioFormatReader *reader = formatManager.createReaderFor(inputFile);
    
    // Get the basic information
    int64 numChannels = reader->numChannels;
    int64 numSamples = reader->lengthInSamples;
    double sampleRate = reader->sampleRate;
    
    // Allocate some memory as an AudioBuffer with type float
    AudioBuffer <float>buffer(numChannels, numSamples);
    
    //Read the audio into the buffer
    reader->read(&buffer, 0, numSamples, 0, true, true);
    
    // We can free the reader
    delete reader;
    
    
    // Do some processing (this is the function we will write ourselves)
    processBuffer(&buffer);
    
    
    // Begin the exporting (start writing to a file)
    File outputFile = File::getSpecialLocation(File::userDesktopDirectory);
    outputFile = outputFile.getChildFile(outputFileName);
    
    // if the file already exists, delete it...
    if (outputFile.existsAsFile())
        outputFile.deleteFile();
    
    // Create the output streamers to actually write the data
	FileOutputStream *fileStream = outputFile.createOutputStream();
    WavAudioFormat wavFormat;
    AudioFormatWriter *writer = wavFormat.createWriterFor(fileStream, sampleRate, numChannels, 16, StringPairArray(), 0);
    writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    writer->flush();
    delete writer;
    
    return 0;
}
