#include <stdlib.h>
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
    }
}
