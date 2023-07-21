`include "helpers.v"
`include "../Drbal_Patrik_CPU.v"

module testbench();
	reg         clk;
	reg         reset;
	wire [31:0] data_to_mem, address_to_mem;
	wire        write_enable;

	integer i;

	top simulated_system (clk, reset, data_to_mem, address_to_mem, write_enable);

	initial	begin
		$dumpfile("test");
		$dumpvars;
		reset<=1; # 2; reset<=0;
		#600;
		$writememh ("memfile_data_after_simulation.hex",simulated_system.dmem.RAM,0,63);
		
		// print registers
		$display("REGISTERS: ");
		for(i = 0; i < 10; i += 1)
			$display ("%d = %x", i, simulated_system.CPU.reg_set.rf[i]);
		$display ("PC: %x", simulated_system.CPU.PC);

		$finish;
	end

	// generate clock
	always	begin
		clk<=1; # 1; clk<=0; # 1;
	end
endmodule