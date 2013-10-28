/********
* poc version 0.80
* c header file generated from : metronome.oc 
* to be used with : metronome.c 
********/
/*-------- Predefined types ---------*/
#ifndef _metronome_POC_PREDEF_TYPES
#define _metronome_POC_PREDEF_TYPES
typedef int _boolean;
typedef int _integer;
typedef char* _string;
typedef float _float;
typedef double _double;
#define _false 0
#define _true 1
#endif
#ifndef _metronome_POC_SRC_FILE
/*--------- Pragmas ----------------*/
//MODULE: metronome 2 2
//IN: _boolean reset
//IN: _integer delay
//OUT: _boolean tic
//OUT: _boolean tac
/*--------Context type -------------*/
struct metronome_ctx;
/*--------Context allocation --------*/
extern struct metronome_ctx* metronome_new_ctx(void* client_data);
/*--------Context copy -------------*/
extern void metronome_copy_ctx(struct metronome_ctx* dest, struct metronome_ctx
* src);
/*-------- Input procedures ---------*/
extern void metronome_I_reset(struct metronome_ctx* ctx, _boolean __V);
extern void metronome_I_delay(struct metronome_ctx* ctx, _integer __V);
/*-------- Reset procedure ----------*/
extern void  metronome_reset(struct metronome_ctx* ctx);
/*-------- Step procedure ----------*/
extern void  metronome_step(struct metronome_ctx* ctx);
#endif
