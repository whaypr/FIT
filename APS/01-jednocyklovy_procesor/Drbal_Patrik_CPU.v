`default_nettype none


module processor( input         clk, reset,
                  output [31:0] PC,
                  input  [31:0] instruction,
                  output        WE,
                  output [31:0] address_to_mem,
                  output [31:0] data_to_mem,
                  input  [31:0] data_from_mem
                );
    /* WIRES */
    wire [2:0] immControl; // Control Unit
    wire [3:0] ALUControl; // Control Unit
    wire MemWrite, RegWrite, ALUSrc, MemToReg, BranchBeq, BranchBlt, BranchJal, BranchJalr, BranchAuipc; // Control Unit
    wire BranchOutcome, BranchJalx; // Control Unit extra
    
    wire [31:0] regData, WriteData; // Register Set

    wire [31:0] ImmOp;// Imm Decode

    wire Zero; // ALU
    wire [31:0] ALUOut; // ALU

    wire [31:0] PCn, SrcB, BranchTarget, res;// misc from muxes
    wire [31:0] PCPlus4, res_mem, res_alupc, brtar_tmp, auipc_tmp, res_final; // misc
    wire brout_tmp, brout_tmp2; // misc

    /* MEMORY MAPPING */
    assign WE = MemWrite;
    assign address_to_mem = ALUOut;
    assign data_to_mem = WriteData;
    assign res_mem = data_from_mem;

    /* COMPONENTS */
    ControlUnit cu(instruction, immControl, ALUControl, MemWrite, RegWrite, ALUSrc, MemToReg, BranchBeq, BranchBlt, BranchJal, BranchJalr, BranchAuipc);
    register PC_reg(clk, PCn, PC);
    RegisterSet reg_set(clk, RegWrite, instruction[19:15], instruction[24:20], instruction[11:7], res_final, regData, WriteData);
    imm_decoder imm_dec(instruction, immControl, ImmOp);
    ALU alu(ALUControl, regData, SrcB, Zero, ALUOut);

    assign PCPlus4 = PC + 4;
    adder adder_brtar_tmp(ImmOp, PC, brtar_tmp); // assign brtar_tmp = ImmOp + PC;
    adder adder_auipc_tmp(res, PC, auipc_tmp);

    or (BranchJalx, BranchJal, BranchJalr); // assign BranchJalx = BranchJal || BranchJalr;
    and (brout_tmp,Zero,BranchBeq); //assign brout_tmp = Zero && BranchBeq;
    and (brout_tmp2, BranchBlt, ALUOut[0]);
    or (BranchOutcome, brout_tmp, BranchJalx, brout_tmp2); // assign BranchOutcome = brout_tmp || BranchJalx;

    mux2 mux_PCn(BranchOutcome, PCPlus4, BranchTarget, PCn);
    mux2 mux_SrcB(ALUSrc, WriteData, ImmOp, SrcB);
    mux2 mux_BranchTarget(BranchJalr, brtar_tmp, ALUOut, BranchTarget);
    mux2 mux_res_alupc(BranchJalx, ALUOut, PCPlus4, res_alupc);
    mux2 mux_res(MemToReg, res_alupc, res_mem, res);
    mux2 mux_res_final(BranchAuipc, res, auipc_tmp, res_final);

    /* PC INIT AND RESET */
    initial PC_reg.out = 0;
    always@(reset) begin
        PC_reg.out = 0;
    end
endmodule


// Arithmetic logic unit 
module ALU (
             input [3:0] ALUControl,
             input signed [31:0] srcA, srcB,
             output reg zero,
             output reg [31:0] ALUResult
           );
    always@(*) begin
        case (ALUControl)
            0: ALUResult = srcA + srcB;
            1: ALUResult = srcA - srcB;
            2: ALUResult = srcA * srcB;
            3: ALUResult = srcA / srcB;
            4: ALUResult = srcA % srcB;
            5: ALUResult = srcA < srcB;
            6: ALUResult = srcA & srcB;
            7: ALUResult = {srcB[31:12], {12{1'sd0}}};
            8: ALUResult = srcA << srcB;
            9: ALUResult = srcA >> srcB;
           10: ALUResult = srcA >>> srcB;
            default: ; // illegal value -> do nothing
        endcase

        if (ALUResult == 0) zero = 1;
        else zero = 0;
    end
endmodule


// Register set
module RegisterSet (
                     input clk, we,
                     input [4:0] addrA, addrB, addrW,
                     input [31:0] wd,
                     output [31:0] rdA, rdB
                    );
    reg [31:0] rf[31:0];
    initial rf[0] = 0;

    assign rdA = rf[addrA];
    assign rdB = rf[addrB];

    always@(posedge clk)
        if (we && addrW)
            rf[addrW] = wd;

endmodule


// Control unit
module ControlUnit (
            input [31:0] inst,
            output reg [2:0] immControl,
            output reg [3:0] ALUControl,
            output reg memWrite, regWrite, ALUSrc, MemToReg, brBeq, brBlt, brJal, brJalr, brAuipc
          );

    /* HELPERS */
    reg [6:0] opcode, funct7;
    reg [2:0] funct3;
    reg [6:0] out1;
    reg [8:0] out2;
    
    always@(*) begin
        opcode = inst[6:0];
        funct3 = inst[14:12];
        funct7 = inst[31:25];
        {immControl,ALUControl} = out1;
        {memWrite,regWrite,ALUSrc,MemToReg,brBeq,brBlt,brJal,brJalr,brAuipc} = out2;
    end

    /* SWITCH */
    always@(*)
        case (opcode)
            'b0110011: case ({funct7, funct3})
                'b0000000_000: begin // add
                    out1 = 'bxxx_0000;
                    out2 = 'b010000000;
                    end
                'b0000000_111: begin // and
                    out1 = 'bxxx_0110;
                    out2 = 'b010000000;
                    end
                'b0100000_000: begin // sub
                    out1 = 'bxxx_0001;
                    out2 = 'b010000000;
                    end
                'b0000000_010: begin // slt
                    out1 = 'bxxx_0101;
                    out2 = 'b010000000;
                    end
                'b0000001_100: begin // div
                    out1 = 'bxxx_0011;
                    out2 = 'b010000000;
                    end
                'b0000001_110: begin // rem
                    out1 = 'bxxx_0100;
                    out2 = 'b010000000;
                    end
                'b0000000_001: begin // sll
                    out1 = 'bxxx_1000;
                    out2 = 'b010000000;
                    end
                'b0000000_101: begin // srl
                    out1 = 'bxxx_1001;
                    out2 = 'b010000000;
                    end
                'b0100000_101: begin // sra
                    out1 = 'bxxx_1010;
                    out2 = 'b010000000;
                    end
                default: ; // illegal value -> do nothing
                endcase
            'b0010011: begin // addi
                out1 = 'b000_0000;
                out2 = 'b011000000;
                end
            'b1100011: case (funct3)
                'b000: begin // beq 
                    out1 = 'b010_0001;
                    out2 = 'b000x10000;
                    end
                'b100: begin // blt
                    out1 = 'b010_0101;
                    out2 = 'b000x01000;
                    end
                default: ; // illegal value -> do nothing
                endcase
            'b0000011: begin // lw
                out1 = 'b000_0000;
                out2 = 'b011100000;
                end
            'b0100011: begin // sw
                out1 = 'b001_0000;
                out2 = 'b101x00000;
                end
            'b0110111: begin // lui
                out1 = 'b011_0111;
                out2 = 'b011000000;
                end
            'b1101111: begin // jal
                out1 = 'b100_xxxx;
                out2 = 'b01x000100;
                end
            'b1100111: begin // jalr
                out1 = 'b000_0000;
                out2 = 'b011000010;
                end
            'b0010111: begin // auipc
                out1 = 'b011_0111;
                out2 = 'b011000001;
                end
            default: ; // illegal value -> do nothing
        endcase
endmodule


module imm_decoder (
                     input [31:0] inst,
                     input [2:0] immControl,
                     output reg [31:0] imm
                   );
    always@(*)
        case (immControl)
            0: imm = { {21{inst[31]}}, inst[30:20] };                               // I
            1: imm = { {21{inst[31]}}, inst[30:25], inst[11:7] };                   // S
            2: imm = { {20{inst[31]}}, inst[7], inst[30:25], inst[11:8], 1'sd0 };   // B
            3: imm = { inst[31:12], {12{1'sd0}} };                                  // U
            4: imm = { {12{inst[31]}}, inst[19:12], inst[20], inst[30:21], 1'sd0 }; // J
            default: ; // illegal value -> do nothing
        endcase
endmodule

// --------------------------------------------------------------

module register (
                  input clk,
                  input [31:0] in,
                  output reg [31:0] out
                );
    always@(posedge clk)
        out <= in;
endmodule


module mux2 (
              input select,
              input [31:0] in0, in1,
              output reg [31:0] out
            );
    always@(*)
        if(select)
            out = in1;
        else
            out = in0;
endmodule


module adder(
             input [31:0] inA, inB,
             output signed [31:0] out
           );
    assign out = inA + inB;
endmodule


`default_nettype wire
