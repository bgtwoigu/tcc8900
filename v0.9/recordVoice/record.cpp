#include "record.h"


snd_pcm_t *capture_handle;
snd_pcm_uframes_t frames;
snd_pcm_stream_t stream;
snd_pcm_hw_params_t *hw_params = NULL;

unsigned int period_time;

int getSampleRate(int m_sample){
  
    switch (m_sample){
        case 0:return 8000;
        case 1:return 11000;
        case 2:return 23000;
        case 3:return 32000;
        default:
               return 0;
    }
}

//��ʼ��Pcm�豸
int InitPcm(int m_sample){

    int err = 0;
    unsigned int val;
    int dir = 0;

    snd_pcm_hw_params_alloca(&hw_params);
    stream = SND_PCM_STREAM_CAPTURE;
    //��һ��pcm�豸������1���豸�ṹ�壬����2���豸���ƣ�����3�Ǵ�ģʽ���ա����ŵȣ�����4�Ǵ򿪷�ʽ����׼�����������첽
    if((err = snd_pcm_open(&capture_handle,"plug:tcc",SND_PCM_STREAM_CAPTURE,0)) < 0){
   
        printf("cannot open audio device(%d)\n",err);
        return -1;
    }
    //��ʹ��Ĭ��ֵ��ʼ��pcm�豸Ӳ�������ṹ��
    if((err = snd_pcm_hw_params_any(capture_handle,hw_params)) < 0){
    
        printf("fail to initialize hardware parameter structure(%d)\n",err);
        return -1;
    }
    //��ȡ���ڳ���(֡��)
    snd_pcm_hw_params_get_period_size(hw_params, &frames,0);
 
  //  printf("frames = %d.............\n",(int)frames);

    //�����豸�����ڻ������ڵĴ洢��ʽ������ͷǽ���
    if((err = snd_pcm_hw_params_set_access(capture_handle,hw_params,SND_PCM_ACCESS_RW_INTERLEAVED)) < 0){
        printf("fail to set access type (%d)\n",err);
        return -1;
    }
    //������Ƶ������ʽ��8λ��16λ��32λ
    if((err = snd_pcm_hw_params_set_format(capture_handle,hw_params,SND_PCM_FORMAT_S16_LE)) < 0){
    
        printf("fail to set sample format (%d)\n",err);
        return -1;
    }
    //���ò���Ƶ��
    if((val = getSampleRate(m_sample)) == 0){
    
        printf("Error: getSampleRate(m_sample), m_sample is error.\n");
        return -1;
    }
    //printf("samplerate:%d\n",(int)val);
    if((err = snd_pcm_hw_params_set_rate_near(capture_handle,hw_params,&val,0)) < 0){

        printf("fail to set sample rate(%d)\n",err);
        return -1;
    }
    //�����豸ͨ����1Ϊ��������2Ϊ������
    val = CH;//Ƶ��1(������)
    if((err = snd_pcm_hw_params_set_channels(capture_handle, hw_params, val)) < 0){
    
        printf("fail to set channel count (%d)\n", err);
        return -1;
    }
    //��������д���úõ�Ӳ������
    if((err = snd_pcm_hw_params(capture_handle,hw_params)) < 0){
       
        printf("fail to set parameters(%d)\n",err);
        return -1;
    }
    //Prepare PCM for use
    if((err = snd_pcm_prepare(capture_handle)) < 0){

        printf("fail to prepare audio interface for use (%d)\n",err);
        return -1;
    }

    frames = 6144;
    //���δָ��pcm�����ڳ���(֡��)��ʹ��һ����ӽ������ڳ���
    if((err = snd_pcm_hw_params_set_period_size_near(capture_handle, hw_params, &frames, &dir)) < 0){
    
        printf("fail to snd_pcm_hw_params_set_period_size_near failed! err = %d\n",err);
        return -1;
    }
    //��ȡ��Ƶ�豸����һ�����ݵ�����
    int status = snd_pcm_hw_params_get_period_time(hw_params, &val, &dir);
    if(status < 0){
    
        printf("fail to get pcm_period_time\n");
        return -1;
    }
    period_time = val;
    //printf("period time = %d us...................\n", val);
    system("amixer cset numid=16 on");//���������޷�¼��
    system("amixer cset numid=7 on");//���������޷�¼��
    system("amixer cset numid=8 on");//�������кܴ�����
   
    return 0;
}

int Recording()
{
    char mono[PCMBUFFSIZE];
    int iRet, iLen;
    extern int wavFd;

    memset(mono, 0, sizeof(char) * PCMBUFFSIZE);
    iLen = snd_pcm_readi(capture_handle, mono, frames);
    if(iLen != (int)frames){
        printf("read from audio interface failed (%d)\n", iLen);
        if((iRet = snd_pcm_prepare(capture_handle)) < 0){
            printf("snd_pcm_prepare failed! err : %d\n", iRet);
            return -1;
        }
    }else if(iLen < 0){
        printf("error from read: %s\n", snd_strerror(iLen));
        return -1;
    }
    /*printf("Get sound success, iLen is %d, frames is %d, strlen(mono) is %d\n,"*,
            iLen, (int)frames, strlen(mono));
    */

    iRet = write(wavFd, mono, sizeof(char) * frames *2);
    if(iRet < 0){
        printf("Fail to write pcm file!\n");
        return -1;
    }
    return iRet;
}


