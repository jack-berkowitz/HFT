/////////////////////////////////////////////////////////////////////////
//                                                                     //
//   Modulename :  sys_defs.svh                                        //
//                                                                     //
//  Description :  This file defines macros and data structures used   //
//                 throughout the processor.                           //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

`ifndef __SYS_DEFS_SVH__
`define __SYS_DEFS_SVH__

`define DATA_W    64
`define KEEP_W    (`DATA_W / 8)
`define MDP3_PORT 16'd14310
`define HDR_BEATS 6

typedef logic [`DATA_W-1:0] AXI_TDATA;
typedef logic [`KEEP_W-1:0] AXI_TKEEP;

`endif // __SYS_DEFS_SVH__