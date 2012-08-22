//
// Author: Wolfgang Spraul
//
// This is free and unencumbered software released into the public domain.
// For details see the UNLICENSE file at the root of the source tree.
//

int fpga_find_iob(struct fpga_model* model, const char* sitename,
	int* y, int* x, int* idx);

const char* fpga_iob_sitename(struct fpga_model* model, int y, int x,
	int idx);

//
// When dealing with devices, there are two indices:
// 1. The index of the device in the device array for that tile.
// 2. The index of the device within devices of the same type in the tile.
//

// Looks up a device pointer based on the type index.
struct fpga_device* fpga_dev(struct fpga_model* model,
	int y, int x, enum fpgadev_type type, int type_idx);

// Counts how many devices of type 'type' are in the device
// array up to devidx.
int fpga_dev_typecount(struct fpga_model* model, int y, int x,
	enum fpgadev_type type, int dev_idx);

enum { A6_LUT, B6_LUT, C6_LUT, D6_LUT };
// lut_len can be -1 (ZTERM)
int fpga_set_lut(struct fpga_model* model, struct fpga_device* dev,
	int which_lut, const char* lut_str, int lut_len);

// returns the number of outgoing connections for the
// connection point given with 'name', and the connection
// point's first dest offset in connpt_dests_o.
int fpga_connpt_lookup(struct fpga_model* model, int y, int x,
	const char* name, int* connpt_dests_o);

const char* fpga_conn_dest(struct fpga_model* model, int y, int x,
	int connpt_dest_idx, int* dest_y, int* dest_x);

typedef int swidx_t;

// SW_FROM and SW_TO values are chosen such that ! inverts them.
#define SW_FROM	0
#define SW_TO	1

// returns a switch index, or -1 (NO_SWITCH) if no switch was found
swidx_t fpga_switch_first(struct fpga_model* model, int y, int x,
	const char* name, int from_to);
swidx_t fpga_switch_next(struct fpga_model* model, int y, int x,
	swidx_t last, int from_to);
swidx_t fpga_switch_backtofirst(struct fpga_model* model, int y, int x,
	swidx_t last, int from_to);

const char* fpga_switch_str(struct fpga_model* model, int y, int x,
	swidx_t swidx, int from_to);
int fpga_switch_is_bidir(struct fpga_model* model, int y, int x,
	swidx_t swidx);
int fpga_switch_is_enabled(struct fpga_model* model, int y, int x,
	swidx_t swidx);
void fpga_switch_enable(struct fpga_model* model, int y, int x,
	swidx_t swidx);
void fpga_switch_disable(struct fpga_model* model, int y, int x,
	swidx_t swidx);

#define SW_CHAIN_NEXT		0 // use for name
#define MAX_SW_CHAIN_SIZE	32

struct sw_chain
{
	// start and recurring values:
	struct fpga_model* model;
	int y;
	int x;
	// start_switch will be set to SW_CHAIN_NEXT (0) after the first call
	const char* start_switch;
	int from_to;

	// return values:
	swidx_t chain[MAX_SW_CHAIN_SIZE];
	int chain_size;

	// internal:
	int first_round;	
};

// Returns 0 if another switch is returned in chain, or
// NO_SWITCH (-1) if there is no other switch.
// chain_size set to 0 when there are no more switches in the tree
int fpga_switch_chain_enum(struct sw_chain* chain);

struct swchain_conns
{
	// start and recurring values:
   	struct fpga_model* model;
	int y;
	int x;
	// start_switch will be set to SW_CHAIN_NEXT (0) after first call
	const char* start_switch;

	// return values:
	struct sw_chain chain;
	int connpt_dest_start;
	int num_dests;
	int dest_i;
	int dest_y;
	int dest_x;
	const char* dest_str;
};

// Returns 0 if another connection is returned in conns, or
// NO_CONN (-1) if there is no other connection.
int fpga_switch_conns_enum(struct swchain_conns* conns);
