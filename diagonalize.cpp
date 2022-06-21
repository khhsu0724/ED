#include <iostream> 
#include "diagonalize.hpp"

using namespace std;

// Lapack functions, need dsyevx or dsyevr
// Rerference: https://epubs.stfc.ac.uk/manifestation/1885/DLTR-2007-005.pdf

extern "C" {
	extern void dgees_(char*,char*,int(*)(double*,double*),int*,double*,int*,
						int*,double*,double*,double*,int*,double*,int*,int*,int*);
}
extern "C" {
	extern void dsyev_(char*,char*,int*,double*,int*,double*,double*,int*,int*);
}

void dgees(double *_mat, double *_eigvec, double* _eigReal, int n) {
	// Can also use for diagonlizing non-symmetric Hamiltonians
	char JOBVS='V',SORT='N';
	double *eigImag = new double[n]{0};
	int info = 0, sdim = 0, lwork = 6*n;
	int *bwork;
	double *work = new double[lwork]{0};
	int (*select)(double*,double*);

	dgees_(&JOBVS,&SORT,select,&n,_mat,&n,&sdim,_eigReal,eigImag,
			_eigvec,&n,work,&lwork,bwork,&info);

	try {
		if (info!=0) throw runtime_error( "Error: dgeev returned error code ");
	} catch(const exception &ex) {std::cout << ex.what() << "\n";}

	delete [] work;
	return;
}

void dsyev(double* _mat, double* _eigval, int n) {
	char JOBZ='V',UPLO='U';
	int info = 0, lwork = -1;
	double *work;
	double wkopt;

	dsyev_(&JOBZ,&UPLO,&n,_mat,&n,_eigval,&wkopt,&lwork,&info); // get lwork size
	lwork = (int)wkopt;
	work = new double[lwork]{0};
	dsyev_(&JOBZ,&UPLO,&n,_mat,&n,_eigval,work,&lwork,&info); // Should return orthonormal ev
	try {
		if (info!=0) throw runtime_error( "Error: dsyev returned error code ");
	} catch(const exception &ex) {std::cout << ex.what() << "\n";}
	delete [] work;
	return;
}