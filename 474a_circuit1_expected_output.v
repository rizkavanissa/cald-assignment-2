`timescale 1ns / 1ps

module Circuit1 (a,b,c,z,x,Clk,Rst);
  input Clk, Rst;
  input [7:0] a, b, c;

  output [7:0] z;
  output [15:0] x;

  wire [7:0] d, e;
  wire [15:0] f, g;
  wire [15:0] xwire;

  ADD #(8) ADD_0(a, b, d);
  ADD #(8) ADD_1(a, c, e);
  COMP #(8) COMP_0(d, e, g,,);
  MUX2x1 #(8) MUX2x1_0(d, e, g, z);
  MUL #(16) MUL_0(a, c, f);
  SUB #(16) SUB_0(f, {8'b0,d}, xwire);
  REG #(16) REG_0(xwire, Clk, Rst, x);
endmodule