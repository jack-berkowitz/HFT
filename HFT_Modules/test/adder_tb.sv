`timescale 1ns/1ps

module adder_tb;

  logic        clk;
  logic        rst;
  logic [31:0] input_a;
  logic [31:0] input_b;
  logic        input_a_stb;
  logic        input_b_stb;
  logic        output_z_ack;
  logic        input_a_ack;
  logic        input_b_ack;
  logic        output_z_stb;
  logic [31:0] output_z;

  adder DUT (
    .input_a      (input_a),
    .input_b      (input_b),
    .input_a_stb  (input_a_stb),
    .input_b_stb  (input_b_stb),
    .output_z_ack (output_z_ack),
    .clk          (clk),
    .rst          (rst),
    .output_z     (output_z),
    .output_z_stb (output_z_stb),
    .input_a_ack  (input_a_ack),
    .input_b_ack  (input_b_ack)
  );

  task automatic test_add(input [31:0] a, b, input [31:0] expected);
      input_a = a;
      input_b = b;

      @(posedge clk iff output_z_stb);
      if (output_z === expected)
          $display("@@@ Passed: 0x%h + 0x%h = 0x%h", a, b, output_z);
      else
          $display("@@@ Failed: got 0x%h, expected 0x%h", output_z, expected);
  endtask

  initial clk = 1'b1;
  always #1 clk = ~clk;

  initial begin
    rst          = 1'b1;
    input_a      = '0;
    input_b      = '0;
    input_a_stb  = 1'b0;
    input_b_stb  = 1'b0;
    output_z_ack = 1'b0;

    #10;
    rst          = 1'b0;
    input_a_stb  = 1'b1;
    input_b_stb  = 1'b1;
    output_z_ack = 1'b1;

  test_add(32'h3f800000, 32'h3f800000, 32'h40000000);
  test_add(32'h3f000000, 32'h3f000000, 32'h3f800000);
  test_add(32'h3f000000, 32'hbf000000, 32'h00000000);

    $finish;
  end

endmodule
