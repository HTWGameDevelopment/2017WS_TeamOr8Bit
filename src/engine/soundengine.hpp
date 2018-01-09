//
// Created by simon on 07.01.2018.
//

#ifndef QE_SOUNDENGINE_HPP
#define QE_SOUNDENGINE_HPP

#include<stdio.h>
#include <portaudio.h>

#define ERRCHECK(x) { if(!(x)) {exit(1); } }

namespace gamespace {
    class SoundEngine {
    private:
        PaStream* stream;
        FILE* wavfile;
        int channels;
        int sampleRate;
        PaSampleFormat sampleFormat;
        int bytesPerSample, bitsPerSample;
        int paStreamCallback(
                const void *input, void *output,
                unsigned long frameCount,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags,
                void *userData )
        {
            size_t numRead = fread(output, bytesPerSample * numChannels, frameCount, wavfile);
            output = (uint8_t*)output + numRead * numChannels * bytesPerSample;
            frameCount -= numRead;

            if(frameCount > 0) {
                memset(output, 0, frameCount * numChannels * bytesPerSample);
                return paComplete;
            }

            return paContinue;
        }

        bool portAudioOpen() {
            ERRCHECK(Pa_Initialize() == paNoError);

            PaStreamParameters outputParameters;

            outputParameters.device = Pa_GetDefaultOutputDevice();
            ERRCHECK(outputParameters.device != paNoDevice);

            outputParameters.channelCount = numChannels;
            outputParameters.sampleFormat = sampleFormat;
            outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;

            PaError ret = Pa_OpenStream(
                    &stream,
                    NULL, // no input
                    &outputParameters,
                    sampleRate,
                    paFramesPerBufferUnspecified, // framesPerBuffer
                    0, // flags
                    &paStreamCallback,
                    NULL //void *userData
            );

            if(ret != paNoError) {
                //Fehler!
                if(stream)
                    Pa_CloseStream(stream);
                return false;
            }
            ERRCHECK(Pa_StartStream(stream) == paNoError);
            return true;
        }

        std::string freadStr(FILE* f, size_t len) {
            std::string s(len, '\0');
            ERRCHECK(fread(&s[0], 1, len, f) == len);
            return s;
        }

        template<typename T>
        T freadNum(FILE* f) {
            T value;
            ERRCHECK(fread(&value, sizeof(value), 1, f) == 1);
            return value;
        }

        void readFmtChunk(uint32_t chunkLen) {
            uint16_t fmttag = freadNum<uint16_t>(wavfile);
            numChannels = freadNum<uint16_t>(wavfile);
            sampleRate = freadNum<uint32_t>(wavfile);
            uint32_t byteRate = freadNum<uint32_t>(wavfile);
            uint16_t blockAlign = freadNum<uint16_t>(wavfile);
            bitsPerSample = freadNum<uint16_t>(wavfile);
            bytesPerSample = bitsPerSample / 8;
            if(fmttag == 1 /*PCM*/) {
                switch(bitsPerSample) {
                    case 8: sampleFormat = paInt8; break;
                    case 16: sampleFormat = paInt16; break;
                    case 32: sampleFormat = paInt32; break;
                }
            } else {
                sampleFormat = paFloat32;
            }
            if(chunkLen > 16) {
                uint16_t extendedSize = freadNum<uint16_t>(wavfile);
                fseek(wavfile, extendedSize, SEEK_CUR);
            }
        }
    public:
        int playsound(char** path) {
            wavfile = fopen(path, "r");

            ERRCHECK(freadStr(wavfile, 4) == "RIFF");
            uint32_t wavechunksize = freadNum<uint32_t>(wavfile);
            ERRCHECK(freadStr(wavfile, 4) == "WAVE");
            while(true) {
                std::string chunkName = freadStr(wavfile, 4);
                uint32_t chunkLen = freadNum<uint32_t>(wavfile);
                if(chunkName == "fmt ")
                    readFmtChunk(chunkLen);
                else if(chunkName == "data") {
                    break; // start playing now
                } else {
                    // skip chunk
                    ERRCHECK(fseek(wavfile, chunkLen, SEEK_CUR) == 0);
                }
            }

            ERRCHECK(portAudioOpen());

            // wait until stream has finished playing
            while(Pa_IsStreamActive(stream) > 0)
                usleep(1000);

            fclose(wavfile);
            Pa_CloseStream(stream);
            Pa_Terminate();
        }

        int stopsound() {
           PA_StopStream(stream);
            Pa_Terminate();
        }
    };

}


#endif //QE_SOUNDENGINE_HPP
