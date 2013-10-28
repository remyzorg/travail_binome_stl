/********
* poc version 0.80
* c file generated from : metronome.oc 
********/
#include <stdlib.h>
#define _metronome_POC_SRC_FILE
#include "metronome.h"
/*--------
Internal structure for the call
--------*/
typedef struct  {
   void* client_data;
   int current_state;
   _boolean _V0;
   _boolean _V1;
   _integer _V2;
   _boolean _V3;
   _integer _V4;
   _boolean _V5;
   _integer _V6;
   _boolean _V7;
   _boolean _V8;
} metronome_ctx;
/*--------
Output procedures must be defined,
Input procedures must be used:
--------*/
void metronome_I_reset(metronome_ctx* ctx, _boolean __V){
   ctx->_V1 = __V;
   ctx->_V0 = _true;
}
void metronome_I_delay(metronome_ctx* ctx, _integer __V){
   ctx->_V2 = __V;
   ctx->_V0 = _true;
}
extern void metronome_O_tic(void*, _boolean);
extern void metronome_O_tac(void*, _boolean);
/*--------
Internal reset input procedure
--------*/
static void metronome_reset_input(metronome_ctx* ctx){
   ctx->_V0 = _false;
   ctx->_V0 = _false;
}
/*--------
Reset procedure
--------*/
void metronome_reset(metronome_ctx* ctx){
   ctx->current_state = 0;
   metronome_reset_input(ctx);
}
/*--------
Dynamic allocation of an internal structure
--------*/
metronome_ctx* metronome_new_ctx(void* cdata){
   metronome_ctx* ctx = (metronome_ctx*)calloc(1, sizeof(metronome_ctx));
   ctx->client_data = cdata;
   metronome_reset(ctx);
   return ctx;
}
/*--------
Copy the value of an internal structure
--------*/
void metronome_copy_ctx(metronome_ctx* dest, metronome_ctx* src){
   memcpy((void*)dest, (void*)src, sizeof(metronome_ctx));
}
/*--------
Step procedure
--------*/
void metronome_step(metronome_ctx* ctx){
   switch(ctx->current_state){
   case 0:
      ctx->_V5 = (ctx->_V4 == 0);
      if(ctx->_V1){
         ctx->_V6 = ctx->_V2;
         ctx->_V4 = ctx->_V6;
      } else {
         ctx->_V6 = 0;
         ctx->_V4 = 1;
      }
      ctx->_V3 = (ctx->_V4 == 0);
      if(ctx->_V3){
         ctx->_V7 = _true;
         metronome_O_tic(ctx->client_data, ctx->_V7);
      } else {
         ctx->_V7 = _false;
         metronome_O_tic(ctx->client_data, ctx->_V7);
      }
      ctx->_V8 = _false;
      metronome_O_tac(ctx->client_data, ctx->_V8);
      if(ctx->_V1){
         ctx->current_state = 1; break;
      } else {
         ctx->current_state = 2; break;
      }
   break;
   
   case 1:
      ctx->_V5 = (ctx->_V4 == 0);
      if(ctx->_V1){
         ctx->_V6 = ctx->_V2;
         ctx->_V4 = ctx->_V6;
      } else {
         if(ctx->_V5){
            ctx->_V4 = ctx->_V6;
         } else {
            ctx->_V4 = (ctx->_V4 - 1);
         }
      }
      ctx->_V3 = (ctx->_V4 == 0);
      if(ctx->_V3){
         if(ctx->_V5){
            ctx->_V7 = _false;
            metronome_O_tic(ctx->client_data, ctx->_V7);
            ctx->_V8 = _true;
            metronome_O_tac(ctx->client_data, ctx->_V8);
         } else {
            ctx->_V7 = _true;
            metronome_O_tic(ctx->client_data, ctx->_V7);
            ctx->_V8 = _false;
            metronome_O_tac(ctx->client_data, ctx->_V8);
         }
      } else {
         ctx->_V7 = _false;
         metronome_O_tic(ctx->client_data, ctx->_V7);
         ctx->_V8 = _false;
         metronome_O_tac(ctx->client_data, ctx->_V8);
      }
      if(ctx->_V5){
         ctx->current_state = 3; break;
      } else {
         ctx->current_state = 1; break;
      }
   break;
   
   case 2:
      ctx->_V5 = (ctx->_V4 == 0);
      if(ctx->_V1){
         ctx->_V6 = ctx->_V2;
         ctx->_V4 = ctx->_V6;
      } else {
         ctx->_V4 = 1;
      }
      ctx->_V3 = (ctx->_V4 == 0);
      if(ctx->_V3){
         if(ctx->_V5){
            ctx->_V7 = _false;
            metronome_O_tic(ctx->client_data, ctx->_V7);
            ctx->_V8 = _true;
            metronome_O_tac(ctx->client_data, ctx->_V8);
         } else {
            ctx->_V7 = _true;
            metronome_O_tic(ctx->client_data, ctx->_V7);
            ctx->_V8 = _false;
            metronome_O_tac(ctx->client_data, ctx->_V8);
         }
      } else {
         ctx->_V7 = _false;
         metronome_O_tic(ctx->client_data, ctx->_V7);
         ctx->_V8 = _false;
         metronome_O_tac(ctx->client_data, ctx->_V8);
      }
      if(ctx->_V1){
         if(ctx->_V5){
            ctx->current_state = 3; break;
         } else {
            ctx->current_state = 1; break;
         }
      } else {
         if(ctx->_V5){
            ctx->current_state = 4; break;
         } else {
            ctx->current_state = 2; break;
         }
      }
   break;
   
   case 3:
      ctx->_V5 = (ctx->_V4 == 0);
      if(ctx->_V1){
         ctx->_V6 = ctx->_V2;
         ctx->_V4 = ctx->_V6;
      } else {
         if(ctx->_V5){
            ctx->_V4 = ctx->_V6;
         } else {
            ctx->_V4 = (ctx->_V4 - 1);
         }
      }
      ctx->_V3 = (ctx->_V4 == 0);
      if(ctx->_V3){
         if(ctx->_V5){
            ctx->_V7 = _true;
            metronome_O_tic(ctx->client_data, ctx->_V7);
            ctx->_V8 = _false;
            metronome_O_tac(ctx->client_data, ctx->_V8);
         } else {
            ctx->_V7 = _false;
            metronome_O_tic(ctx->client_data, ctx->_V7);
            ctx->_V8 = _true;
            metronome_O_tac(ctx->client_data, ctx->_V8);
         }
      } else {
         ctx->_V7 = _false;
         metronome_O_tic(ctx->client_data, ctx->_V7);
         ctx->_V8 = _false;
         metronome_O_tac(ctx->client_data, ctx->_V8);
      }
      if(ctx->_V5){
         ctx->current_state = 1; break;
      } else {
         ctx->current_state = 3; break;
      }
   break;
   
   case 4:
      ctx->_V5 = (ctx->_V4 == 0);
      if(ctx->_V1){
         ctx->_V6 = ctx->_V2;
         ctx->_V4 = ctx->_V6;
      } else {
         ctx->_V4 = 1;
      }
      ctx->_V3 = (ctx->_V4 == 0);
      if(ctx->_V3){
         if(ctx->_V5){
            ctx->_V7 = _true;
            metronome_O_tic(ctx->client_data, ctx->_V7);
            ctx->_V8 = _false;
            metronome_O_tac(ctx->client_data, ctx->_V8);
         } else {
            ctx->_V7 = _false;
            metronome_O_tic(ctx->client_data, ctx->_V7);
            ctx->_V8 = _true;
            metronome_O_tac(ctx->client_data, ctx->_V8);
         }
      } else {
         ctx->_V7 = _false;
         metronome_O_tic(ctx->client_data, ctx->_V7);
         ctx->_V8 = _false;
         metronome_O_tac(ctx->client_data, ctx->_V8);
      }
      if(ctx->_V1){
         if(ctx->_V5){
            ctx->current_state = 1; break;
         } else {
            ctx->current_state = 3; break;
         }
      } else {
         if(ctx->_V5){
            ctx->current_state = 2; break;
         } else {
            ctx->current_state = 4; break;
         }
      }
   break;
   } /* END SWITCH */
   metronome_reset_input(ctx);
}
