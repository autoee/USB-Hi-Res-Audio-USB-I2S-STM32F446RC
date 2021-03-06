/*
********************************************************************************
* COPYRIGHT(c) ��� ���� � ��ϻ, 2019
* 
* ����������� ����������� ��������������� �� �������� ���� ����� (as is).
* ��� ��������������� �������� ������ �����������.
********************************************************************************
*/



#include "audio_configuration.h"
#include "usbd_audio.h"
#include "SAI.h"



static uint8_t AudioConfiguration = 0;
static uint8_t SyncSource = 0;


void SetAudioConfigDependedFuncs(AUDIO_SpeakerNode_t *speaker)
{
  switch(AudioConfiguration)
  {
    case AUDIO_CONFIG_2_0_STEREO_32_BIT:
    case AUDIO_CONFIG_2_0_STEREO:
    default:
      speaker->SpeakerPlay = Play_SAIMaster;
      speaker->SpeakerPrepareData = 0;
    break;
    
    case AUDIO_CONFIG_3_1:
    case AUDIO_CONFIG_4_0_QUADRO:
      speaker->SpeakerPlay = Play_SAIMasterAndSlave;
      speaker->SpeakerPrepareData = PrepareData_4_Chnls;
    break;
  }
}
//------------------------------------------------------------------------------
void Play_SAIMaster(uint16_t *Data, uint16_t Size, uint8_t ResByte)
{
  if (ResByte == 3)
    ResByte = 4;
  
  uint16_t TxSize = Size / CONFIG_2_0_SAI_COUNT / ResByte;

  SAI_MasterDMAPrepareTx(Data, TxSize);
  SAI_MasterDMAEnable();
}
//------------------------------------------------------------------------------
void Play_SAIMasterAndSlave(uint16_t *Data, uint16_t Size, uint8_t ResByte)
{
  if (ResByte == 3)
    ResByte = 4;
  
  uint16_t TxSize = Size / CONFIG_4_0_SAI_COUNT / ResByte;

  SAI_MasterDMAPrepareTx(&Data[0], TxSize);
  SAI_SlaveDMAPrepareTx(&Data[(ResByte / 2) * TxSize], TxSize);
  
  SAI_SlaveDMAEnable();
  SAI_MasterDMAEnable();
}
//------------------------------------------------------------------------------
void PrepareData_4_Chnls(uint8_t* AudioData, uint16_t Size, uint8_t ResInBytes)
{  
  uint8_t* SAISlaveData = malloc(Size / 2);
  if (!SAISlaveData)
  {
    Error_Handler();
  }

  if (ResInBytes == 3)
    ResInBytes = 4;
  
  uint8_t DataIncrement = ResInBytes * CONFIG_4_0_QUADRO_CHANNEL_COUNT;
  uint8_t BytesPerSAI = DataIncrement / CONFIG_4_0_SAI_COUNT;
  
  for (uint16_t i = 0; i < Size; i += DataIncrement)
  {
    uint16_t BaseAddr = i / CONFIG_4_0_SAI_COUNT;
    
    AudioData[BaseAddr] = AudioData[i];
    AudioData[BaseAddr + 1] = AudioData[i + 1];
    AudioData[BaseAddr + 2] = AudioData[i + 2];
    AudioData[BaseAddr + 3] = AudioData[i + 3];
    
    SAISlaveData[BaseAddr] = AudioData[i + BytesPerSAI];
    SAISlaveData[BaseAddr + 1] = AudioData[i + BytesPerSAI + 1];
    SAISlaveData[BaseAddr + 2] = AudioData[i + BytesPerSAI + 2];
    SAISlaveData[BaseAddr + 3] = AudioData[i + BytesPerSAI + 3];
    
    if (ResInBytes == 4)
    {
      AudioData[BaseAddr + 4] = AudioData[i + 4];
      AudioData[BaseAddr + 5] = AudioData[i + 5];
      AudioData[BaseAddr + 6] = AudioData[i + 6];
      AudioData[BaseAddr + 7] = AudioData[i + 7];

      SAISlaveData[BaseAddr + 4] = AudioData[i + BytesPerSAI + 4];
      SAISlaveData[BaseAddr + 5] = AudioData[i + BytesPerSAI + 5];
      SAISlaveData[BaseAddr + 6] = AudioData[i + BytesPerSAI + 6];
      SAISlaveData[BaseAddr + 7] = AudioData[i + BytesPerSAI + 7];
    }
  }

  memcpy(&AudioData[Size / CONFIG_4_0_SAI_COUNT], SAISlaveData, Size / CONFIG_4_0_SAI_COUNT);
  
  free(SAISlaveData);
}
//------------------------------------------------------------------------------
void AudioChangeFrequency(uint32_t AudioFrequency)
{
  SAI_MasterDisable();
  if (AudioConfiguration > AUDIO_CONFIG_2_0_STEREO)
    SAI_SlaveDisable();
  
  if (SyncSource == EXTERNAL_SYNC)
    ExtSyncSelectSource(AudioFrequency);
  else
  {
    SAI_PLLSAIConfig(AudioFrequency);
    SAI_ChangeFrequency(AudioFrequency);
  }
    
  if (AudioConfiguration > AUDIO_CONFIG_2_0_STEREO)
    SAI_SlaveEnable();
  
  SAI_MasterEnable();
}
//------------------------------------------------------------------------------
void ExtSyncSelectSource(uint32_t AudioFrequency)
{
  EXT_SYNC_SELECT_GPIO->ODR &= ~EXT_SYNC_SELECT_MASK;
    
  switch(AudioFrequency)
  {
    case USB_AUDIO_CONFIG_FREQ_44_1_K:
      EXT_SYNC_SELECT_GPIO->ODR |= EXT_SYNC_SELECT_44100;
    break;
      
    case USB_AUDIO_CONFIG_FREQ_48_K:
    default:
      EXT_SYNC_SELECT_GPIO->ODR |= EXT_SYNC_SELECT_48000;
    break;
      
    case USB_AUDIO_CONFIG_FREQ_96_K:
      EXT_SYNC_SELECT_GPIO->ODR |= EXT_SYNC_SELECT_96000;
    break;
      
    case USB_AUDIO_CONFIG_FREQ_192_K:
      EXT_SYNC_SELECT_GPIO->ODR |= EXT_SYNC_SELECT_192000;
    break;
  }
}
//------------------------------------------------------------------------------
void AudioChangeResolution(uint8_t AudioResolution)
{
  SAI_MasterDisable();
  if (AudioConfiguration > AUDIO_CONFIG_2_0_STEREO)
    SAI_SlaveDisable();
  
  SAI_Init(SAI_MASTER, AudioResolution);
  SAI_MasterDMAChangeDataSize(AudioResolution);
  
  if (AudioConfiguration > AUDIO_CONFIG_2_0_STEREO)
  {
    SAI_Init(SAI_SLAVE, AudioResolution);
    SAI_SlaveDMAChangeDataSize(AudioResolution);
  }
  
  if (AudioConfiguration > AUDIO_CONFIG_2_0_STEREO)
    SAI_SlaveEnable();
  
  SAI_MasterEnable();
}
//------------------------------------------------------------------------------
void AudioOutMute(uint8_t MuteFlag)
{
  SAI_MasterMute(MuteFlag);
  
  if (AudioConfiguration > AUDIO_CONFIG_2_0_STEREO)
    SAI_SlaveMute(MuteFlag);
}
//------------------------------------------------------------------------------
void AudioOutInit(uint32_t AudioFrequency, uint8_t AudioResolution)
{
  if (SyncSource == EXTERNAL_SYNC)
    SAI_ExternalSyncInit();
  else
    SAI_PLLSAIConfig(AudioFrequency);
  
  SAI_MasterGPIOInit();
  SAI_MasterDMAInit(AudioResolution);
  SAI_MasterInit(AudioResolution);
  
  if (AudioConfiguration > AUDIO_CONFIG_2_0_STEREO)
  {
    SAI_SlaveGPIOInit();
    SAI_SlaveDMAInit(AudioResolution);
    SAI_SlaveInit(AudioResolution);
    SAI_SlaveEnable();
  }
  
  if (SyncSource == EXTERNAL_SYNC)
    ExtSyncSelectSource(AudioFrequency);
  else
    SAI_ChangeFrequency(AudioFrequency);
  
  SAI_MasterEnable();
}
//------------------------------------------------------------------------------
void PlayDescriptionInit(AUDIO_Description_t *Description)
{
  switch(AudioConfiguration)
  {
    case AUDIO_CONFIG_2_0_STEREO_32_BIT:
    case AUDIO_CONFIG_2_0_STEREO:
    default:
      Description->channels_count = CONFIG_2_0_STEREO_CHANNEL_COUNT;
      Description->channels_map = CONFIG_2_0_STEREO_CHANNEL_MAP;
    break;
    
    case AUDIO_CONFIG_3_1:
      Description->channels_count = CONFIG_3_1_CHANNEL_COUNT;
      Description->channels_map = CONFIG_3_1_CHANNEL_MAP;
    break;
    
    case AUDIO_CONFIG_4_0_QUADRO:
      Description->channels_count = CONFIG_4_0_QUADRO_CHANNEL_COUNT;
      Description->channels_map = CONFIG_4_0_QUADRO_CHANNEL_MAP;
    break;
  }
    
  Description->resolution = CONFIG_RES_BYTE_24;//CONFIG_RES_BYTE_16;
  Description->frequency = CONFIG_2_0_FREQUENCY_DEFAULT;
  Description->audio_type = USBD_AUDIO_FORMAT_TYPE_PCM;
  Description->audio_volume_db_256 = VOLUME_SPEAKER_DEFAULT_DB_256;
  Description->audio_mute = 0;
  
  if (AudioConfiguration == AUDIO_CONFIG_2_0_STEREO_32_BIT)
  {
    Description->resolution = CONFIG_RES_BYTE_32;
    Description->frequency = CONFIG_2_0_32BIT_FREQUENCY_DEFAULT;
  }
}
//------------------------------------------------------------------------------
uint8_t GetAudioConfiguration(void)
{
  return AudioConfiguration;
}
//------------------------------------------------------------------------------
void MakeSerialNumber(uint32_t *Buffer)
{
  Buffer[0] = (*(uint32_t*)DEVICE_ID1) + (*(uint32_t*)DEVICE_ID3);
  Buffer[1] = (((*(uint32_t*)DEVICE_ID2) >> 16) + AudioConfiguration) << 16;
}
//------------------------------------------------------------------------------
void AudioConfig_Init(void)
{
  AudioConfiguration |= (CONFIG_1_GPIO->IDR & (1 << CONFIG_1_PIN)) >> CONFIG_1_PIN;
  AudioConfiguration |= ((CONFIG_2_GPIO->IDR & (1 << CONFIG_2_PIN)) >> CONFIG_2_PIN) << 1;
  
  switch(AudioConfiguration)
  {
    case CONFIG_PIN_2_MASK:
      AudioConfiguration = AUDIO_CONFIG_2_0_STEREO_32_BIT;
    break;
   
    case CONFIG_PINS_MASK:
    default:
      AudioConfiguration = AUDIO_CONFIG_2_0_STEREO;
    break;
    
    case CONFIG_PIN_1_MASK:
      AudioConfiguration = AUDIO_CONFIG_3_1;
    break;
    
    case 0:
      AudioConfiguration = AUDIO_CONFIG_4_0_QUADRO;
    break;
  }
  
  if ((CONFIG_3_GPIO->IDR & (1 << CONFIG_3_PIN)) != (1 << CONFIG_3_PIN))
  {
    EXT_SYNC_SELECT_GPIO->MODER |= (1 << (2 * EXT_SYNC_SELECT_1_PIN)) | (1 << (2 * EXT_SYNC_SELECT_2_PIN));
    SyncSource = EXTERNAL_SYNC;
  }
}
//------------------------------------------------------------------------------
void OUTClk_Init(void)
{
  uint32_t AudioFreq = CONFIG_2_0_FREQUENCY_DEFAULT;
  
  if (SyncSource != EXTERNAL_SYNC)
  {
    switch((OUT_CLK_CONFIG_1_GPIO->IDR & ((1 << OUT_CLK_CONFIG_1_PIN) | (1 << OUT_CLK_CONFIG_3_PIN))))
    {
      case OUT_CLK_CONFIG_1:
        AudioFreq = USB_AUDIO_CONFIG_FREQ_44_1_K;
      break;
      
      case OUT_CLK_CONFIG_2:
      default:
        AudioFreq = USB_AUDIO_CONFIG_FREQ_48_K;
      break;
      
      case OUT_CLK_CONFIG_3:
        AudioFreq = USB_AUDIO_CONFIG_FREQ_96_K;
      break;
      
      case OUT_CLK_CONFIG_4:
        AudioFreq = USB_AUDIO_CONFIG_FREQ_192_K;
      break;
    }
  }
  
  AudioOutInit(AudioFreq, CONFIG_RES_BIT_24);
}
//------------------------------------------------------------------------------
void ExtPowerDisable(void)
{
  POWER_DISABLE_GPIO->ODR |= 1 << POWER_DISABLE_PIN;
}


