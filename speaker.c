#include "speaker.h"
#include <stdlib.h>

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

