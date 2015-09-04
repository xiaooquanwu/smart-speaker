#include "speaker.asr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
    speaker_waiter_init();
    speaker_cooker_init();
    asr_init();

    speaker_waiter_run();
}
