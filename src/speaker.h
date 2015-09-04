#ifndef SPEAKER_H
#define SPEAKER_H

#include <ifly/qisr.h>
#include <ifly/msp_cmn.h>
#include <ifly/msp_errors.h>

int
speaker_init ();

const char*
get_grammar_id ();

int
set_grammar_id ();

void
speaker_destroy ();

#endif /* SPEAKER_H */
