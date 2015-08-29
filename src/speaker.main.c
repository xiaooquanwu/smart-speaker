#include "speaker.asr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* 
get_audio_file(void)
{
	char key = 0;
	while(key != 27)//按Esc则退出
	{
		system("cls");//清屏
		printf("请选择音频文件：\n");
		printf("1.科大讯飞\n");
		printf("2.阿里山龙胆\n");
		printf("3.齐鲁石化\n");
		printf("4.一二三四五六七八九十\n");
		printf("注意：第三条的音频故意结巴说出来的，用于展示效果。\n      关键字列表中没有第四条，展示如果用户说的词语不在列表中，会得到什么结果。\n");
		key = getchar();
		switch(key)
		{
		case '1':
			printf("1.科大讯飞\n");
			return "../test/sound/iflytek01.wav";                               //iflytek01对应的音频内容“科大讯飞”
		case '2':
			printf("2.阿里山龙胆\n");
			return "../test/sound/iflytek02.wav";                               //iflytek02对应的音频内容“阿里山龙胆”
		case '3':
			printf("3.齐鲁石化\n");
			return "../test/sound/iflytek03.wav";                               //iflytek03对应的音频内容“齐鲁石化”
		case '4':
			printf("4.一二三四五六七八九十\n");
			return "../test/sound/iflytek04.wav";                               //iflytek04对应的音频内容"一二三四五六七八九十"
		default:
			continue;
		}
	}
	exit(0);
	return NULL;
}


int main ()
{
    asr_init();
	const char* asrfile = get_audio_file(); //选择
    char *result = asr_run(asrfile);
    printf ("result: <%s>\n", result);
    tts_run ("科大讯飞");
}
