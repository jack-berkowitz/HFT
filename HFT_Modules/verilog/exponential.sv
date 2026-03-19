`timescale 1ns / 1ps
`define DATALENGTH 32
`define OP 				2'b10
module exponential (
		input  logic			           	Clock,
		input  logic           				Reset,
        input  logic                        Str,
		input  logic [`DATALENGTH-1:0] 	Datain,
        output logic                        Ack,
		output logic [`DATALENGTH-1:0]		DataOut
	
);
logic [`DATALENGTH-1:0] one, oversix, half;
//reg [`DATALENGTH-1:0]	T0R,T1R,T2R,TsR,TdR,TcR, DoR;
logic [`DATALENGTH-1:0] Count;
logic [`DATALENGTH-1:0]	T0;
logic [`DATALENGTH-1:0] T1,T2,Ts,Td,Tc,To,Tsh;
logic wa_add_1,wb_add_1,wz_add_1;
logic wa_add_2,wb_add_2,wz_add_2;
logic wa_add_3,wb_add_3,wz_add_3;
logic wa_mul_1,wb_mul_1,wz_mul_1;
logic wa_mul_2,wb_mul_2,wz_mul_2;
logic wa_sh_1,wb_sh_1,wz_sh_1;
logic wa_div_1,wb_div_1,wz_div_1;

assign one = 32'h3f800000;
assign oversix = 32'h3e2aaaab ;
assign half = 32'h3f000000;

adder A1 (
        .clk          (Clock),
        .rst          (Reset),
        .input_a      (one),
        .input_a_stb  (Str),
        .input_a_ack  (wa_add_1),
        .input_b      (T0),
        .input_b_stb  (Str),
        .input_b_ack  (wb_add_1),
        .output_z     (T1),
        .output_z_stb (wz_add_1),
        .output_z_ack (Str)
        );
multiplier Ms (
        .clk          (Clock),
        .rst          (Reset),
        .input_a      (T0),
        .input_a_stb  (wa_add_1),
        .input_a_ack  (wa_mul_1),
        .input_b      (T0),
        .input_b_stb  (wb_add_1),
        .input_b_ack  (wb_mul_1),
        .output_z     (Ts),
        .output_z_stb (wz_mul_1),
        .output_z_ack (wz_add_1)
        );
multiplier Msh (
        .clk          (Clock),
        .rst          (Reset),
        .input_a      (half),
        .input_a_stb  (wa_mul_1),
        .input_a_ack  (wa_sh_1),
        .input_b      (Ts),
        .input_b_stb  (wb_mul_1),
        .input_b_ack  (wb_sh_1),
        .output_z     (Tsh),
        .output_z_stb (wz_sh_1),
        .output_z_ack (wz_mul_1)
        );
adder A2 (
        .clk          (Clock),
        .rst          (Reset),
        .input_a      (T1),
        .input_a_stb  (wa_sh_1),
        .input_a_ack  (wa_add_2),
        .input_b      (Tsh),
        .input_b_stb  (wb_sh_1),
        .input_b_ack  (wb_add_2),
        .output_z     (T2),
        .output_z_stb (wz_add_2),
        .output_z_ack (wz_sh_1)
        );

multiplier Mc (
        .clk          (Clock),
        .rst          (Reset),
        .input_a      (Ts),
        .input_a_stb  (wa_add_2),
        .input_a_ack  (wa_mul_2),
        .input_b      (To),
        .input_b_stb  (wb_add_2),
        .input_b_ack  (wb_mul_2),
        .output_z     (Tc),
        .output_z_stb (wz_mul_2),
        .output_z_ack (wz_add_2)
        );

multiplier Dm (
        .clk          (Clock),
        .rst          (Reset),
        .input_a      (Tc),
        .input_a_stb  (wa_mul_2),
        .input_a_ack  (wa_div_1),
        .input_b      (oversix),
        .input_b_stb  (wb_mul_2),
        .input_b_ack  (wb_div_1),
        .output_z     (Td),
        .output_z_stb (wz_div_1),
        .output_z_ack (wz_mul_2)
        );

adder A3 (
        .clk          (Clock),
        .rst          (Reset),
        .input_a      (Td),
        .input_a_stb  (wa_div_1),
        .input_a_ack  (wa_add_3),
        .input_b      (T2),
        .input_b_stb  (wb_div_1),
        .input_b_ack  (wb_add_3),
        .output_z     (To),
        .output_z_stb (wz_add_3),
        .output_z_ack (wz_div_1)
        );
integer i;
always @(posedge Clock or posedge Reset) begin 
	if(Reset) begin
                T0 <= 0;
                DataOut <= 0;
                Ack <= 0;
                Count <= 0;
//                T1 <= 0;
//                T2 <= 0;
//                Ts <= 0;
//                Tc <= 0;
//                Td <= 0;
	end else begin
        if (Str) begin
    		T0 <= Datain;
            DataOut <= To;
            if (Count == 140)
                Ack <= 1;
            Count <= Count +1;
        end
        else 
            T0 <= 32'hz;

		// T1R <= T1;
		// TsR <= Ts;
		// T2R <= T2;
		// TcR <= Tc;
		// T1 <= T0 + 1 ;
		// Ts <= T0 * T0;
		// T2 <= T1 + (Ts >> 2);
		// Tc <= Ts* T0; 
		// DataOut <= T2 + (Tc /6);

	end
end

endmodule
