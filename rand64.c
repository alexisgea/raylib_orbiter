#include <stdint.h>
// https://nullprogram.com/blog/2017/09/21/
// https://en.m.wikipedia.org/wiki/Xorshift

// #include <time.h>
// int main(void)
// {
//     clock_t begin = clock();


//     clock_t end = clock();
//     double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

//     return 0;
// }

uint64_t rol64(uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}


struct xorshiftr128p_state {
    uint64_t s[2]; // seeds
};

/* The state must be seeded so that it is not all zero */
uint64_t xorshiftr128p(struct xorshiftr128p_state *state)
{ 
	uint64_t x = state->s[0];
	uint64_t const y = state->s[1];
	state->s[0] = y;
	x ^= x << 23; // shift & xor
	x ^= x >> 17; // shift & xor
	x ^= y; // xor
	state->s[1] = x + y;
	return x;
}



struct xoshiro256pp_state {
    uint64_t s[4];
};

/* Adapted from the code included on Sebastiano Vigna's website */
uint64_t xoshiro256pp(struct xoshiro256pp_state *state) {
	uint64_t *s = state->s;
	uint64_t const result = rol64(s[0] + s[3], 23) + s[0];
	uint64_t const t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;
	s[3] = rol64(s[3], 45);

	return result;
}

struct xoshiro256ss_state {
	uint64_t s[4];
};

// might be faster than PP above?
uint64_t xoshiro256ss(struct xoshiro256ss_state *state) {
	uint64_t *s = state->s;
	uint64_t const result = rol64(s[1] * 5, 7) * 9;
	uint64_t const t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;
	s[3] = rol64(s[3], 45);

	return result;
}


// INITIALISATORS

struct splitmix64_state {
	uint64_t s;
};

uint64_t splitmix64(struct splitmix64_state *state) {
	uint64_t result = (state->s += 0x9E3779B97f4A7C15);
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}


void xoshiro256ss_init(struct xoshiro256ss_state *state, uint64_t seed) {
	struct splitmix64_state smstate = {seed};
	state->s[0] = splitmix64(&smstate);
	state->s[1] = splitmix64(&smstate);
    state->s[2] = splitmix64(&smstate);
	state->s[3] = splitmix64(&smstate);
}

void xoshiro256pp_init(struct xoshiro256pp_state *state, uint64_t seed) {
	struct splitmix64_state smstate = {seed};
	state->s[0] = splitmix64(&smstate);
	state->s[1] = splitmix64(&smstate);
    state->s[2] = splitmix64(&smstate);
	state->s[3] = splitmix64(&smstate);
}

void xorshiftr128plus_init(struct xorshiftr128p_state *state, uint64_t seed) {
	struct splitmix64_state smstate = {seed};
	state->s[0] = splitmix64(&smstate);
	state->s[1] = splitmix64(&smstate);
}



