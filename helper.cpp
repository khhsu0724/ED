#include "helper.hpp"

size_t ed::choose(size_t n, size_t k) {
	if (k > n) return 0;
	if (!k) return 1;
    return (n*choose(n-1, k-1))/k;
}

bool ed::is_pw2(ulli x) {return !(x == 0) && !(x & (x - 1));}

ulli ed::next_perm(ulli v) {
	ulli t = v | (v - 1);
	return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzll(v) + 1));
}

// Enumarate states that include bits for "inc" variable using recursion
void ed::enum_states(std::vector<ulli>& states, ulli n, ulli k, ulli inc, ulli s) {
	if (k > n) return;
	if (k == 0 && n < (__builtin_ctzll(inc)+1)) {
		states.emplace_back(s);
		return;
	}
	enum_states(states,n-1,k-1,inc,s+(1<<(n-1))); // bit is 1
	if (!(inc & (1<<(n-1)))) enum_states(states,n-1,k,inc,s); // bit is 0
}

// Add valence state and core state 
ulli ed::add_bits(ulli b1, ulli b2, int b1size, int b2size) {
	ulli b1d = b1 & ((BIG1 << (b1size/2)) - 1);
	ulli b2d = b2 & ((BIG1 << (b2size/2)) - 1);
	return (b1^b1d) << b2size | (b2^b2d) << (b1size/2) | b1d << (b2size/2) | b2d;
}

int ed::count_bits(ulli b) {
	int c;
	for (c = 0; b; c++) b &= b - 1;
	return c;
}

vecc ed::ctranspose(const vecc& mat, size_t m, size_t n) {
	// Conjugate Transpose of a matrix
	vecc trans(m*n,0);
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < m; j++) {
			trans.at(j*n+i) = std::conj(mat.at(i*m+j));
		}
	}
	return trans;
}


std::vector<int> ed::distribute(int num_h, int num_at) {
	int base = num_h / num_at;
	int mod = num_h % num_at;
	std::vector<int> dist(num_at,base);
	for (int i = 0; i < mod; ++i) ++dist[i];
	return dist;
}

std::string ed::format_duration(std::chrono::milliseconds ms) {
    using namespace std::chrono;
    auto secs = duration_cast<seconds>(ms);
    ms -= duration_cast<milliseconds>(secs);
    auto mins = duration_cast<minutes>(secs);
    secs -= duration_cast<seconds>(mins);
    auto hour = duration_cast<hours>(mins);
    mins -= duration_cast<minutes>(hour);

    std::stringstream ss;
    ss << hour.count() << " Hours : " << mins.count() << " Minutes : " << secs.count() << " Seconds : " << ms.count() << " Milliseconds";
    return ss.str();
}

void ed::print_progress(double frac, double all) {
	// Print at least every 2%
	if (((int)frac+1)%((int)all/50+1) && frac != all) return;
	double progress = frac/all;
    int barWidth = 40;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
	if (progress == 1) std::cout << std::endl;
	return;
}
