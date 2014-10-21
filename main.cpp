#include <iostream>
#include "HMM.hpp"
#include <vector>

int main()
{
    int a[] = {1,2,3,4,5,1,2,3,4,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,1,2,3,1,2,2,2,3,4,2,3,4,2,3,3,1,0};
	std::vector<int>in;
	for(int i=0;i<90;++i)
		in.push_back(a[i]);

	HMM test(6,6); //create HMM obj
	test.reset(); //random start guess
	test.BaumWelch(in); //BaumWelch

	std::cerr <<"Converged: "<< test.isConverged() << std::endl;

    std::vector<int> ut = test.Viterbi(in);
    for(int i=0; i<(int)ut.size();++i)
    {
        std::cerr << ut[i] << " ";
    }
    std::cerr << std::endl;


	return 0;
}
