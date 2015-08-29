#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ifly/qisr.h>
#include <ifly/msp_cmn.h>
#include <ifly/msp_errors.h>

#define MAX_KEYWORD_LEN 4096
char GrammarID[128];

int get_grammar_id( int upload)
{
	int ret = MSP_SUCCESS;
	const char * sessionID = NULL;
	char UserData[MAX_KEYWORD_LEN];
	unsigned int len = 0;
	const char* testID = NULL;
	FILE *fp = NULL;
	memset(UserData, 0, MAX_KEYWORD_LEN);
	if (0 == upload)

	{
		strcpy(GrammarID, "e7eb1a443ee143d5e7ac52cb794810fe");
		                 //65bc94965a076ecaf6ea79899bebc534
		//这个ID是我上传之后记录下来的。语法上传之后永久保存在服务器上，所以不要反复上传同样的语法
		return 0;
	}	
	//如果想要重新上传语法，传入参数upload置为TRUE，就可以走下面的上传语法流程
	fp = fopen("../conf/asr_keywords_utf8.txt", "rb");  //关键字列表文件必须是utf8格式
	if (fp == NULL)
	{
		printf("keyword file cannot open\n");
		return -1;
	}
	len = (unsigned int)fread(UserData, 1, MAX_KEYWORD_LEN, fp);
	UserData[len] = 0;
	fclose(fp);
       testID = MSPUploadData("userword", UserData, len, "dtt = userword, sub = asr", &ret);// sub 参数必需，否则返回空串
	if(ret != MSP_SUCCESS)
	{
		printf("UploadData with errorCode: %d \n", ret);
		return ret;
	}
	memcpy((void*)GrammarID, testID, strlen(testID));
	printf("*************************************************************\n");
	printf("GrammarID: \"%s\" \n", GrammarID);//将获得的GrammarID输出到屏幕上
	printf("*************************************************************\n");
	return 0;
}


const char* g_param = "rst=plain,rse=utf8,sub=asr,aue=speex-wb,auf=audio/L16;rate=16000,ent=sms16k";    //注意sub=asr,16k音频aue=speex-wb，8k音频识别aue=speex，

char *
asr_run (const char* asrfile)
{
    const char *param = g_param;
	char rec_result[1024*4] = {0};
	const char *sessionID;
	FILE *f_pcm = NULL;
	char *pPCM = NULL;
	int lastAudio = 0 ;
	int audStat = 2 ;
	int epStatus = 0;
	int recStatus = 0 ;
	long pcmCount = 0;
	long pcmSize = 0;
	int ret = 0 ;
	sessionID = QISRSessionBegin(GrammarID, param, &ret); //asr
	if(ret !=0)
	{
		printf("QISRSessionBegin Failed,ret=%d\n",ret);
	}

	f_pcm = fopen(asrfile, "rb");
	if (NULL != f_pcm) {
		fseek(f_pcm, 0, SEEK_END);
		pcmSize = ftell(f_pcm);
		fseek(f_pcm, 0, SEEK_SET);
		pPCM = (char *)malloc(pcmSize);
		fread((void *)pPCM, pcmSize, 1, f_pcm);
		fclose(f_pcm);
		f_pcm = NULL;
	} else {
        printf ("can not find file <%s>\n", asrfile);
        exit(-1);
    }

	while (1) {
		unsigned int len = 6400;
              unsigned int audio_len = 6400;
		if (pcmSize < 12800) {
			len = pcmSize;
			lastAudio = 1;
		}
		audStat = 2;
		if (pcmCount == 0)
			audStat = 1;
		if (0) {
			if (audStat == 1)
				audStat = 5;
			else
				audStat = 4;
		}
		if (len<=0)
		{
			break;
		}
		printf("\ncsid=%s,count=%lu,aus=%d,",sessionID,pcmCount/audio_len,audStat);
		ret = QISRAudioWrite(sessionID, (const void *)&pPCM[pcmCount], len, audStat, &epStatus, &recStatus);
		printf("eps=%d,rss=%d,ret=%d",epStatus,recStatus,ret);
		if (ret != 0)
			break;
		pcmCount += (long)len;
		pcmSize -= (long)len;
		if (recStatus == 0) {
			const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &ret);
			if (ret !=0)
			{
				printf("QISRGetResult Failed,ret=%d\n",ret);
				break;
			}
			if (NULL != rslt)
				strcat(rec_result,rslt);
		}
		if (epStatus == MSP_EP_AFTER_SPEECH)
			break;
		usleep(150000);
	}
	ret=QISRAudioWrite(sessionID, (const void *)NULL, 0, 4, &epStatus, &recStatus);
	if (ret !=0)
	{
		printf("QISRAudioWrite Failed,ret=%d\n",ret);
	}

    printf ("QISRAudioWrite SUCC\n");

	free(pPCM);
	pPCM = NULL;
	while (recStatus != 5 && ret == 0) {
		const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &ret);
		if (NULL != rslt)
		{
			strcat(rec_result,rslt);
		}
		usleep(150000);
	}
	ret=QISRSessionEnd(sessionID, NULL);
	if(ret !=MSP_SUCCESS)
	{
		printf("QISRSessionEnd Failed, ret=%d\n",ret);
	}
    
    return strdup(rec_result);
}

int
asr_init ()
{
	const char* login_config = "appid = 55bbb30a,work_dir =   .  ";
	int ret = 0 ;
	char key = 0 ;
	int grammar_flag = 0;   //0:不上传词表；1：上传词表
	ret = MSPLogin(NULL, NULL, login_config);
	if ( ret != MSP_SUCCESS )
	{
		printf("MSPLogin failed , Error code %d.\n",ret);
		return 0 ;
	}
	memset(GrammarID, 0, sizeof(GrammarID));
	ret = get_grammar_id(grammar_flag);
	if(ret != MSP_SUCCESS)
	{
		printf("get_grammar_id with errorCode: %d \n", ret);
		return 0 ;
	}

	return 0;
}
