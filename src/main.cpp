#include <iostream>
#include <windows.h>

#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#define DEVICE_FORMAT       ma_format_f32
#define DEVICE_CHANNELS     2
#define DEVICE_SAMPLE_RATE  48000

bool simpleGate = false;

std::string getAsset(std::string assetName) {
    return ASSET_DIR + std::string("/" + assetName);
}

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
    if (simpleGate) {
        ma_waveform* pSineWave;

        MA_ASSERT(pDevice->playback.channels == DEVICE_CHANNELS);

        pSineWave = (ma_waveform*)pDevice->pUserData;
        MA_ASSERT(pSineWave != NULL);

        ma_waveform_read_pcm_frames(pSineWave, pOutput, frameCount, NULL);

        (void)pInput;   /* Unused. */
    }
}

int main() {
    std::cout << "Test: " << getAsset("Testings") << std::endl;

    bool exitable = false, buttonGate = false;

    ma_waveform sineWave;
    ma_device_config deviceConfig;
    ma_device device;
    ma_waveform_config sineWaveConfig;

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = DEVICE_FORMAT;
    deviceConfig.playback.channels = DEVICE_CHANNELS;
    deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &sineWave;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        return -4;
    }

    printf("Device Name: %s\n", device.playback.name);

    sineWaveConfig = ma_waveform_config_init(device.playback.format, device.playback.channels, device.sampleRate, ma_waveform_type_sine, 0.2, 220);
    ma_waveform_init(&sineWaveConfig, &sineWave);

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        return -5;
    }

    while (!exitable) {
        isKeyPressed(VK_SHIFT, buttonGate);
    }
    ma_device_uninit(&device);
    return 0;
}
