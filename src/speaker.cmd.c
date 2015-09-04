#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "speaker.cmd.h"
#include "speaker.asr.h"

request_t*
speaker_cmd_parse (const char *audio_command)
{
    char *str = asr_run (audio_command);
    char *text = strstr (str, "input=");
    if (text==NULL) {
        fprintf (stderr, "can not be here");
        return NULL;
    }
    text = text + 6;

    static const request_t requests[] = {
        {REQ_WHO_ARE_YOU, "你是谁"},
        {REQ_PLAY, "播放音乐"},
        {REQ_GET_TIME, "几点了"},
        {REQ_GET_WEATHER, "天气预报"}};
    
    unsigned i = 0;
    unsigned len = sizeof(requests)/sizeof(requests[0]);
    for (i=0; i<len; i++) {
        if (strncmp(text, requests[i].cmd, strlen(requests[i].cmd)) == 0)
            return request_new(requests[i].op, requests[i].cmd);
    }
	
    fprintf (stderr, "unknow command <%s>", text);
    return NULL;
}

request_t*
request_new(request_op_t op, const char *cmd)
{
    request_t *request = malloc(sizeof(request));
    request->op = op;
    if (cmd==NULL) {
        request->cmd = NULL;
    } else {
        request->cmd = strdup(cmd);
    }

    return request;
}

void
request_free(request_t *req)
{
    if (req==NULL)
        return;
    free (req->cmd);
    free (req);
}

int
request_send(io_t *io, request_t *request)
{

}

request_t*
request_recv(io_t *io)
{

}

reply_t*
reply_new(reply_op_t op)
{
    reply_t *reply = NULL;
    reply = malloc (sizeof(reply_t));
    reply->op = op;

    return reply;
}

void
reply_free(reply_t *reply)
{
    if (reply==NULL)
        return;
    free (reply);
}

int
reply_send(io_t *io, reply_t *reply)
{

}

reply_t*
reply_recv(io_t *io)
{

}

