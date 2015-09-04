#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "speaker.h"

//注意sub=asr,16k音频aue=speex-wb, 8k音频识别aue=speex，
#define ASR_PARAM "rst=plain,rse=utf8,sub=asr,aue=speex-wb,auf=audio/L16;rate=16000,ent=sms16k"
#define MSP_REC_STATUS_SPEECH_COMPLETE 5
char *
asr_run (const char* asrfile)
{
	char rec_result[1024*4] = {0};
	FILE *f_pcm = NULL;
	char *pPCM = NULL;
	int epStatus = 0;
	int recStatus = 0 ;
	int ret = 0 ;
	long pcmSize = 0;

    /* create a session */
	const char *sessionID = QISRSessionBegin(get_grammar_id(), ASR_PARAM, &ret);
	if(ret !=0)
	{
		fprintf(stderr, "QISRSessionBegin Failed,ret=%d\n", ret);
        return NULL;
	}
    
    /* the format of @asrfile can also be WAV format */
	f_pcm = fopen(asrfile, "rb");
    if (f_pcm==NULL) {
        printf ("can not find file <%s>\n", asrfile);
        QISRSessionEnd (sessionID, NULL);
        return NULL;
    }
    
    /* read the content in sound file */
    fseek(f_pcm, 0, SEEK_END);
    pcmSize = ftell(f_pcm);
    fseek(f_pcm, 0, SEEK_SET);
    pPCM = (char *)malloc(pcmSize);
    fread((void *)pPCM, pcmSize, 1, f_pcm);
    fclose(f_pcm);
    f_pcm = NULL;

	long pcmCount = 0;  /* the number of bytes sent to server */
	int audStat;
    while (1) {
		unsigned int len = 6400;
		if (pcmSize < 6400*2) {
			len = pcmSize;
		}

        if (pcmCount > 0) {
            audStat = MSP_AUDIO_SAMPLE_CONTINUE;
        } else if (pcmCount == 0) {
            audStat = MSP_AUDIO_SAMPLE_FIRST;
        }

		if (len <= 0)   /* there is no data to be sent */
		{
			break;
		}

        /* send the data in file to server */
		ret = QISRAudioWrite(sessionID, (const void *)&pPCM[pcmCount], len, audStat, &epStatus, &recStatus);
		if (ret != 0) {
            fprintf(stderr, "QISRAudioWrite failed, error code is: %d", ret );
			break;
        } else if (MSP_EP_AFTER_SPEECH==epStatus) {
            fprintf(stderr, "end point of speech has been detected!");
            break;
        }

		pcmCount += (long)len;
		pcmSize -= (long)len;
		if (recStatus == MSP_REC_STATUS_SUCCESS) {
			const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &ret);
			if (ret !=0)
			{
				fprintf(stderr, "QISRGetResult Failed,ret=%d\n",ret);
				break;
			}
			if (NULL != rslt)
				strcat(rec_result, rslt);
		}
	}

    /* tell server the input is over */
	ret=QISRAudioWrite(sessionID, (const void *)NULL, 0, MSP_AUDIO_SAMPLE_LAST, &epStatus, &recStatus);
	if (ret !=0)
	{
		fprintf(stderr, "QISRAudioWrite Failed,ret=%d\n",ret);
        QISRSessionEnd(sessionID, NULL);
        return NULL;
	}

    printf ("QISRAudioWrite SUCC\n");

	free(pPCM);
	pPCM = NULL;
	while (recStatus != MSP_REC_STATUS_SPEECH_COMPLETE && ret == 0) {
		const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &ret);
		if (rslt)
		{
			strcat(rec_result,rslt);
		}
	}

	ret=QISRSessionEnd(sessionID, NULL);

    return strdup(rec_result);
} 
