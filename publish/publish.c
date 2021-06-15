#include  "hiredis.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>


void pubCallback(redisAsyncContext *c, void *r, void *privdata) 
{
  redisReply *reply = (redisReply*)r;
  if (reply == NULL){
    printf( "Response not recev"); 
    return;
  }
  puts("message published");
  redisAsyncDisconnect(c);
}

int main(int argv, char** args)
{
  signal(SIGPIPE, SIG_IGN);
  struct event_base* base = event_base_new();

  redisAsyncContext* _redisContext = redisAsyncConnect("127.0.0.1", 6379);

  if (_redisContext->err) {
    /* Let context leak for now... */
    puts("Error: ");
    return 1;
  }

  redisLibeventAttach(_redisContext,base);

  int status;
  //Act as publisher
  const char* command = "publish foo bar";

  status = redisAsyncCommand(_redisContext, pubCallback, (char*)"pub", command);
  event_base_dispatch(base);
  return 0;
}


/*
    unsigned int j, isunix = 0;
    redisContext *c;
    redisReply *reply;
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";

    if (argc > 2) {
        if (*argv[2] == 'u' || *argv[2] == 'U') {
            isunix = 1;
            // in this case, host is the path to the unix socket 
            printf("Will connect to unix socket @%s\n", hostname);
        }
    }

    int port = (argc > 2) ? atoi(argv[2]) : 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    if (isunix) {
        c = redisConnectUnixWithTimeout(hostname, timeout);
    } else {
        c = redisConnectWithTimeout(hostname, port, timeout);
    }
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    // PING server 
    reply = redisCommand(c,"PING");
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);
    // Set a key 
    reply = redisCommand(c,"SET %s %s", "foo", "hello world");
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    // Set a key using binary safe API 
    reply = redisCommand(c,"SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5);
    printf("SET (binary API): %s\n", reply->str);
    freeReplyObject(reply);

    // Try a GET and two INCR 
    reply = redisCommand(c,"GET foo");
    printf("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);
    // again ... 
    reply = redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);

    // Create a list of numbers, from 0 to 9 
    reply = redisCommand(c,"DEL mylist");
    freeReplyObject(reply);
    for (j = 0; j < 10; j++) {
        char buf[64];

        snprintf(buf,64,"%u",j);
        reply = redisCommand(c,"LPUSH mylist element-%s", buf);
        freeReplyObject(reply);
    }

    // Let's check what we have inside the list 
    reply = redisCommand(c,"LRANGE mylist 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < reply->elements; j++) {
            printf("%u) %s\n", j, reply->element[j]->str);
        }
    }
    freeReplyObject(reply);
    // Disconnects and frees the context 
    
    redisFree(c);

    return 0;
}

*/
