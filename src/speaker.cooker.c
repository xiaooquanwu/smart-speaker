#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "speaker.cooker.h"
#include "speaker.cmd.h"

int
speaker_cooker_init ()
{
    return 0;
}

int
speaker_cooker_run (const request_t *req)
{
    if (req==NULL)
        return -1;
    
    if (req->op == REQ_WHO_ARE_YOU) {
        tts_run("我是智能音箱");
        return 0;
    } else if (req->op == REQ_PLAY) {
        system("play test/music/one.wav");
    } else if (req->op == REQ_GET_TIME) {
        FILE *stream = popen ("date", "r");
        char buff[128];
        fgets (buff, 128, stream);
        tts_run(buff);
    } else if (req->op == REQ_GET_WEATHER) {
        system ("3rd-libs/lib/weather/weather.py >/tmp/data");
        FILE *stream = fopen ("/tmp/data", "r");
        char output[512] = "";
        char buff[512];
        size_t n;
        while ((n=fread(buff, 1, 512, stream)) != 0) {
            buff[n] = 0;
            strcat (output, buff);
        }
        printf ("output: %s\n", output);
        tts_run(output);
    }
}
