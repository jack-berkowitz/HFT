`timescale 1ns/1ps

package exponential_pkg;
  parameter int DATALENGTH = 32;
  parameter int INPUTMAX   = 5;
endpackage

typedef enum logic [1:0] {
  IDLE        = 2'b00,
  INPUTSTREAM = 2'b01,
  OP          = 2'b10
} state_t;

module exponential_tb;
  import exponential_pkg::*;

  logic                  Clock;
  logic                  Reset;
  logic                  Str;
  logic [DATALENGTH-1:0] Datain;
  logic [DATALENGTH-1:0] DataOut;
  logic                  Ack;

  exponential DUT (
    .Clock   (Clock),
    .Reset   (Reset),
    .Str     (Str),
    .Datain  (Datain),
    .Ack     (Ack),
    .DataOut (DataOut)
  );

  initial Clock = 1'b0;
  always #1 Clock = ~Clock;

  initial begin
    Reset  = 1'b1;
    Str    = 1'b0;
    Datain = '0;

    #10;
    Reset  = 1'b0;
    Str    = 1'b1;
    Datain = 32'h3f800000;

    #50;
    $finish;
  end

endmodule