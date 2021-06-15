#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>


void subCallback(redisAsyncContext *c, void *r, void *privdata) 
{
  redisReply *reply = (redisReply*)r;
  if (reply == NULL){
    puts("Response not recev");
    return;
  }

  if(reply->type == REDIS_REPLY_ARRAY & reply->elements == 3)
  {
    printf("reply->element[0]->str = %s \n", reply->element[0]->str);
    if(strcmp(reply->element[0]->str,"subscribe") != 0)
    {
      printf("Message received -> %s ( on channel : %s ",  reply->element[2]->str, reply->element[1]->str);
    }
  }
}

int main(int argv, char** args)
{
   signal(SIGPIPE, SIG_IGN);
   struct event_base* base = event_base_new();

   redisAsyncContext* _redisContext = redisAsyncConnect("127.0.0.1", 6379);

   if (_redisContext->err) {
     /* Let context leak for now... */
     printf("Error: ");
     return 1;
   }

   redisLibeventAttach(_redisContext,base);
   
   int status;
  //Act as subscriber
    const char* command = "subscribe foo";

     status = redisAsyncCommand(_redisContext, 
           subCallback, 
           (char*)"sub", command); 

  event_base_dispatch(base);
  return 0;
}


