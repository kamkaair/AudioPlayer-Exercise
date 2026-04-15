#include <iostream>
#include <windows.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

//#define DEVICE_FORMAT       ma_format_f32
//#define DEVICE_CHANNELS     2
//#define DEVICE_SAMPLE_RATE  48000

bool simpleGate = false;

std::string getAsset(std::string assetName) {
    return ASSET_DIR + std::string("/" + assetName);
}
std::string asset_path = getAsset("GETOUT.mp3");

void isKeyPressed(int key, bool& buttonGate) {
    if (GetAsyncKeyState(key) < 0 && buttonGate == false)
    {
        std::cout << key << " key is down." << std::endl;
        buttonGate = true;
    }
    if (GetAsyncKeyState(key) == 0 && buttonGate == true)
    {
        std::cout << key << " key is RELEASED!" << std::endl;
        buttonGate = false;
    }
    simpleGate = buttonGate;
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    // In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
    // pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
    // frameCount frames.
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (simpleGate) {
        if (pDecoder == NULL) {
            return;
        }
        
        /* Reading PCM frames will loop based on what we specified when called ma_data_source_set_looping(). */
        ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
        //ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

        (void)pInput;
    }
    else {
        ma_decoder_seek_to_pcm_frame(pDecoder, 0);
    }
}

int main() {
    bool exitable = false, buttonGate = false;
    //std::string asset_path = getAsset("GETOUT.mp3");
    std::cout << "Test path: " << asset_path << std::endl;

    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;
    //ma_waveform sineWave;
    //ma_waveform_config sineWaveConfig;

    result = ma_decoder_init_file(asset_path.c_str(), NULL, &decoder);
    if (result != MA_SUCCESS) {
        printf("Could not load file: %s\n", asset_path.c_str());
        return -1;
    }
    ma_data_source_set_looping(&decoder, MA_TRUE);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -2;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -3;
    }

    printf("Device Name: %s\n", device.playback.name);

    while (!exitable) {
        isKeyPressed(VK_SHIFT, buttonGate);
    }

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    return 0;
}
