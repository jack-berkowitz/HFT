`timescale 1ns / 1ps

// Parameters replacing `define macros
package softmax_pkg;
  parameter int DATALENGTH = 32;
  parameter int INPUTMAX   = 5;
endpackage

// FSM states as an enum
typedef enum logic [1:0] {
  IDLE         = 2'b00,
  INPUTSTREAM  = 2'b01,
  OP           = 2'b10,
  OUTPUTSTREAM = 2'b11
} state_t;

parameter logic [2:0] NUMBER = 3'b101;

module softmax_tb;
  import softmax_pkg::*;

  // SV uses logic instead of reg/wire
  logic                    Clock;
  logic                    Reset;
  logic                    Start;
  logic [DATALENGTH-1:0]   Datain;
  logic [INPUTMAX-1:0]     N;
  logic [DATALENGTH-1:0]   Dataout;

  // Named port connection (best practice in SV)
  softmax DUT (
    .Clock   (Clock),
    .Reset   (Reset),
    .Start   (Start),
    .Datain  (Datain),
    .N       (N),
    .Dataout (Dataout)
  );

  // Clock generation using SV always_ff style
  initial Clock = 1'b1;
  always #1 Clock = ~Clock;

  // Stimulus block
  initial begin
    Reset  = 1'b1;
    Start  = 1'b0;
    N      = 5'h3;
    Datain = '0;

    #10;
    Start  = 1'b1;
    Reset  = 1'b0;
    Datain = 32'h3f800000;

    #2 Datain = 32'h3f800000;
    #2 Datain = 32'h3f800000;
    #2 Datain = 32'h3f800000;

    Start = 1'b0;

    #2000;
    $finish;
  end

endmodule