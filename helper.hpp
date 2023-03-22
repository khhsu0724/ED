#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <complex>
#include <string>
#include <tuple>
#include <utility>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <memory>
#include <chrono>
#ifndef HELPER
#define HELPER

#define TOL 1E-7
#define PI 3.141592653589793238462643383279502884L
typedef std::complex<double> dcomp;
typedef std::vector<double> vecd;
typedef std::vector<std::complex<double>> vecc;
typedef unsigned long long int ulli;
typedef unsigned long int uli;
typedef std::pair<size_t,size_t> bindex;
typedef std::vector<std::pair<ulli,ulli>> vpulli;
#define BIG1 ulli(1)

namespace ed {
	bool is_pw2(ulli x);
	size_t choose(size_t n, size_t k);
	ulli next_perm(ulli v);
	void enum_states(std::vector<ulli>& states, ulli n, ulli k, ulli inc = 0, ulli s = 0);
	ulli add_bits(ulli b1, ulli b2, int b1size, int b2size);
	int count_bits(ulli b);
	vecc ctranspose(const vecc& mat, size_t m, size_t n);
	std::vector<int> distribute(int num_h, int num_at);
	std::string format_duration(std::chrono::milliseconds ms);
	void print_progress(double frac, double all);

	template <typename T> T dot(std::vector<T> a, std::vector<T> b) {
		try {
			if (a.size() != b.size()) std::invalid_argument("different vector size for dot product");
			T dp = 0;
			for (int i = 0; i < a.size(); ++i) dp += a[i]*b[i];
			return dp;
		} catch (const std::exception &ex) {
			std::cout << ex.what() << "\n";
			exit(0);
		}
	};

	template <typename T> double norm(std::vector<T>& vin) {
		double n = 0;
		for (auto& v : vin) n += pow(abs(v),2);
		return sqrt(n);
	};

	template <typename T> void norm_vec(std::vector<T>& invec) {
		double norm = 0;
		for (auto & i : invec) norm += pow(abs(i),2);
		norm = sqrt(norm);
		for (auto & i : invec) i = i / norm;
	};

	template <typename T> void norm_ev(T* ev, int n) {
		for (int i = 0; i < n; ++i) {
			T nm = 0;
			for (int j = 0; j < n; j++) {
				if (abs(ev[i*n+j]) < 1e-7) ev[i*n+j] = 0;
				nm += ev[i*n+j]*ev[i*n+j];
			}
			if (nm != 1) {
				nm = sqrt(nm);
				for (int j = 0; j < n; j++) ev[i*n+j] /= nm;
			}
		}
		return;
	}

	template <typename T> std::vector<T> intersection(std::vector<T>& v1, std::vector<T>& v2) {
		std::vector<T> v3;
	    std::set_intersection(v1.begin(),v1.end(),
	                          v2.begin(),v2.end(),
	                          back_inserter(v3));
	    return v3;
	};

	template <typename DT> void write_mat(std::unique_ptr<DT[]>& mat, size_t x, size_t y, std::string file_dir) {
		std::ofstream matfile;
	    matfile.open (file_dir);
		for (int j = 0; j < y; ++j) {
	    	for (int i = 0; i < x; ++i) {
	    		matfile << std::setw(8) << mat[i+x*j];
	    		if (i < x-1) matfile << " ";
	    	}
	    	matfile << "\n";
	    }
	    matfile.close();
	};

	template <typename DT> void write_vec(std::vector<DT> vec, size_t x, size_t y, std::string file_dir) {
		std::ofstream matfile;
	    matfile.open (file_dir);
		for (int j = 0; j < y; ++j) {
	    	for (int i = 0; i < x; ++i) {
	    		matfile << vec.at(i+x*j);
	    		if (i < x-1) matfile << ",";
	    	}
	    	matfile << "\n";
	    }
	    matfile.close();
	};

	template <typename DT> void write_vec_neat(std::vector<DT> vec, size_t x, size_t y, std::string file_dir) {
		std::ofstream matfile;
	    matfile.open (file_dir);
		for (int j = 0; j < y; ++j)
	    	for (int i = 0; i < x; ++i)
	    		if (abs(vec.at(i+x*j)) > TOL)
		    		matfile << "[" << i << "," << j << "]: " << vec.at(i+x*j) << "\n";
	    matfile.close();
	};

	template <typename C, typename T> 
	std::vector<T> printDistinct(const C & arr, T tmp, int n, bool is_print = true) {
		// Second argument serves as a away to inform the template type in the container
	    std::vector<T> unique_val;
	    for (int i = 0; i < n; i++) {
	        int j;
	        for (j = 0; j < i; j++)
	           if (std::abs(arr[i] - arr[j]) < 1e-7)
	               break;
	        if (i == j) unique_val.push_back(arr[i]);
	    }
	    std::sort(unique_val.begin(), unique_val.end());
	    if (is_print) for (int i =0; i < unique_val.size(); ++i) std::cout << unique_val[i] << " ";
	    if (is_print) std::cout << std::endl;
	    return unique_val;
	};

	template <typename T> T trace(T *mat, int n) {
		T t = 0;
		for (int i = 0; i < n; ++i) t += mat[i+n*i];
		return t;
	};

	template <typename T> std::vector<T> vec_cross(std::vector<T> v1, std::vector<T> v2) {
		return std::vector<T>{v1[1]*v2[2]-v1[2]*v2[1],-v1[0]*v2[2]+v1[2]*v2[0],v1[0]*v2[1]-v1[1]*v2[0]};
	};

	template<typename T> std::vector<T> slice(std::vector<T> const &v, int m, int n) {
	    return std::vector<T> (v.begin()+m, v.begin()+n+1);;
	};

	template<typename T> std::vector<T> vec_mult(std::vector<T> v, T mult) {
		for (auto &ve : v) ve *= mult;
		return v;
	};
	// Add 2 vectors together
	template<typename T> void vec_add(std::vector<T>& v1, std::vector<T> v2) {
		if (v1.size() != v2.size()) return;
		for (size_t i = 0; i < v1.size(); ++i) v1[i] += v2[i];
	};

	template<typename T> bool veccmp(std::vector<T> v1, std::vector<T> v2) {
		if (v1.size() != v2.size()) return false;
		for (size_t i = 0; i < v1.size(); ++i) {
			if (v1[i] != v2[i]) return false;
		}
		return true;
	};

	template<typename T> std::vector<T> matmult(const std::vector<T>& v1, const std::vector<T>& v2, int size) {
		if (v1.size() != size*size || v2.size() != size*size) {
			std::invalid_argument("invalid input matrices for multiplication");
		}
		std::vector<T> mult(size*size,0);
		for (size_t i = 0; i < size; ++i) {
			for (size_t j = 0; j < size; ++j) {
				for(size_t m = 0; m < size; ++m) {
					mult.at(i*size+j) += v1.at(i*size+m) * v2.at(m*size+j);
				}
			}
		}
		return mult;
	};

	template<typename T> std::vector<T> matmultvec(const std::vector<T>& mat, const std::vector<T>& vec, int size) {
		// Matrix multiply a vector
		if (mat.size() != size*size || vec.size() != size) {
			std::invalid_argument("invalid input matrices for multiplication");
		}
		std::vector<T> vmult(size,0);
		for (size_t i = 0; i < size; ++i) {
			for (size_t j = 0; j < size; ++j) {
				vmult.at(i) += mat.at(i*size+j) * vec.at(j);
			}
		}
		return vmult;
	};


	template<typename T> std::vector<T> transpose(const std::vector<T>& mat, int m, int n) {
		std::vector<T> trans(m*n,T::value_type());
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < m; j++) {
				trans.at(j*n+i) = mat.at(i*m+j);
			}
		}
		return trans;
	};

	template<typename T, typename Fcomp, typename Feq>
	size_t binary_search(std::vector<T> &svec, T t, Fcomp comp, Feq eq) {
		size_t ind = svec.size()/2 - 1, end = svec.size() - 1;
		while (!eq(svec[ind],t)) { //abs(svec[ind]-t) > TOL 
			if (ind == end) return -1;
			else if (comp(t,svec[ind])) ind = (ind+end+1)/2;
			else if (comp(svec[ind],t)) {
				end = ind;
				ind /= 2;
			}
		}
		return ind;
	};

	template<typename T> bool is_zero_arr(T* arr, int s) {
		for (int i = 0; i < s; ++i) if (std::abs(arr[i]) > TOL) return false;
		return true;
	};

	template<typename T> std::vector<size_t> top_n(std::vector<T> &arr, size_t top) {
		// Takes in an array and returns top n elements index
		std::vector<size_t> indices(arr.size());
		std::iota(indices.begin(), indices.end(),0);
		std::partial_sort(indices.begin(), indices.begin()+top, indices.end(),
	                  [&](size_t i, size_t j) {return arr[i] > arr[j];});
		indices.resize(top);
		return indices;
	};

	template<typename T> std::vector<T> make_blk_mat(const std::vector<T>& a, 
		 int acopy = 1, const std::vector<T> b = {}, int bcopy = 1) {
		// Make Block diagonal matrix with a & b (with acopy of a and bcopy of b)
		// | a 0 |
		// | 0 b |
		size_t asize = std::sqrt(a.size()), bsize = std::sqrt(b.size());
		size_t blk_size = asize*acopy+bsize*bcopy;
		std::vector<T> blk_mat(blk_size*blk_size,0);
		for (size_t ac = 0; ac < acopy; ++ac) {
			for (size_t i = 0; i < asize; ++i) {
				for (size_t j = 0; j < asize; ++j) {
					blk_mat[(i+ac*asize)*blk_size+j+ac*asize] = a[i*asize+j];
				}
			}
		}
		size_t apad = acopy*asize;
		for (size_t bc = 0; bc < bcopy; ++bc) {
			for (size_t i = 0; i < bsize; ++i) {
				for (size_t j = 0; j < bsize; ++j) {
					blk_mat[(i+bc*bsize+apad)*blk_size+j+bc*bsize+apad] = b[i*bsize+j];
				}
			}
		}
		return blk_mat;
	};
}

#endif