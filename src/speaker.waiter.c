#include <stdio.h>
#include <assert.h>

#include "speaker.waiter.h"
#include "speaker.cmd.h"

const char*
get_audio_file ()
{
    printf ("please input the command:\n");
    printf ("  1: who are you ?\n");
    printf ("  2: play music?\n");
    printf ("  3: what is the time?\n");

    char command[64];
    scanf("%s", command); 
    switch (command[0]) {
        case '1':
            return "test/sound/who-are-you.wav";
        case '2':
            return "test/sound/play-music.wav";
        case '3':
            return "test/sound/what-is-the-time.wav";
        default:
            return NULL;
    }

    return NULL;
}

int
speaker_waiter_init ()
{

}

int
speaker_waiter_run ()
{
    while (1) {
        const char *audio_command = get_audio_file ();
        request_t *req = speaker_cmd_parse (audio_command);
        assert (req);
        speaker_cooker_run (req);
        request_free (req);
    }
}
