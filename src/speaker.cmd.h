#ifndef SPEAKER_CMD_H
#define SPEAKER_CMD_H

typedef int io_t;

typedef enum {
    REQ_WHO_ARE_YOU,
    REQ_GET_TIME,
    REQ_FAIL,
    REQ_PLAY,
    REQ_TALK,
    REQ_QUERY,
    REQ_MAX
} request_op_t;

typedef struct {
    request_op_t op;
    char *cmd;
} request_t;

request_t*
request_new(request_op_t op, const char *cmd);

void
request_free(request_t *req);

int
request_send(io_t *io, request_t *request);

request_t*
request_recv(io_t *io);

typedef enum {
    REPLY_SUCC,
    REPLY_ERR,
    REPLY_MAX
} reply_op_t;

typedef struct {
    reply_op_t op;
} reply_t;

reply_t*
reply_new();

void
reply_free(reply_t *reply);

int
reply_send(io_t *io, reply_t *reply);

reply_t*
reply_recv(io_t *io);

request_t*
speaker_cmd_parse (const char *audio_command);

#endif /* SPEAKER_CMD_H */
