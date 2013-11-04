node metronome
  (reset: bool;
  delay: int)
returns
  (tic: bool;
  tac: bool);

var
  V6_hz: int;
  V7_n: int;
  V8_first: bool;
  V9_state: bool;

let
  assert (delay >= 0);
  tic = ((V7_n = 0) and V9_state);
  tac = ((V7_n = 0) and (not V9_state));
  V6_hz = (if reset then delay else (0 -> (pre V6_hz)));
  V7_n = (if (not V8_first) then 1 else (if (reset or ((pre V7_n) = 0)) then 
  V6_hz else ((pre V7_n) - 1)));
  V8_first = ((false -> (pre V8_first)) or reset);
  V9_state = (true -> (if ((pre V7_n) = 0) then (not (pre V9_state)) else (pre 
  V9_state)));
tel

