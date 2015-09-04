#include "speaker.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define MAX_KEYWORD_LEN 4096

#define GRAMMAR_ID_LEN  128

/* the default grammar id */
static char s_grammar_id [GRAMMAR_ID_LEN] = "e7eb1a443ee143d5e7ac52cb794810fe";

const char*
get_grammar_id ()
{
    return s_grammar_id;
}

int
set_grammar_id ()
{
	int ret = MSP_SUCCESS;
	const char *sessionID = NULL;
	const char* testID = NULL;
	FILE *fp = NULL;

    /* open the key word file */
	fp = fopen("conf/asr_keywords_utf8.txt", "rb"); /* binary mode */
	if (fp == NULL)
	{
		fprintf (stderr, "keyword file cannot open\n");
		return -1;
	}
    
    /* read the data in file */
	char UserData[MAX_KEYWORD_LEN];
	memset (UserData, 0, MAX_KEYWORD_LEN);
	unsigned len = (unsigned) fread(UserData, 1, MAX_KEYWORD_LEN, fp);
	UserData[len] = 0;
	fclose(fp);

    /* upload the keywords, and ge the grammar id */
    testID = MSPUploadData("userword", UserData, len, "dtt=userword, sub=asr", &ret);
	if(ret != MSP_SUCCESS)
	{
		fprintf (stderr, "UploadData with errorCode: %d \n", ret);
		return -1;
	}
	memcpy(s_grammar_id, testID, strlen(testID));
	printf("*************************************************************\n");
	printf("GrammarID: \"%s\" \n", s_grammar_id);  //将获得的GrammarID输出到屏幕上
	printf("*************************************************************\n");
	return 0;
}

int
speaker_init ()
{
	int ret;
	char key;

    /* login on the ifly's remote service */
	const char* login_config = "appid=55bbb30a, work_dir=/tmp";
	ret = MSPLogin(NULL, NULL, login_config);
	if ( ret != MSP_SUCCESS )
	{
		fprintf (stderr, "MSPLogin failed , Error code %d.\n", ret);
		return -1;
	}

    /* set the Grammar ID */
	ret = set_grammar_id();    //0:不上传词表；1：上传词表
	if(ret != MSP_SUCCESS)
	{
		fprintf (stderr, "get_grammar_id with errorCode: %d \n", ret);
		return -1;
	}

	return 0;
}

void
speaker_destroy ()
{
    MSPLogout();
}
