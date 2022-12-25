#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>

// output_vec = vec1 + alpha*vec2
std::vector<double> linear_trans(std::vector<double> & vec1, std::vector<double> & vec2, double alpha){
    if(vec1.size()!=vec2.size())
	{
	    throw std::out_of_range(
		"the size of the two vectors are different ");
	}
    std::vector<double> output_vec(vec1.size());
    std::transform(vec2.begin(), vec2.end(), vec2.begin(),std::bind(std::multiplies<double>(), std::placeholders::_1, alpha));
    std::transform( vec1.begin(), vec1.end(), vec2.begin(), output_vec.begin(), std::plus<double>());
    return output_vec;
}
std::vector<double> proj2vector(std::vector<double> & from_vec, std::vector<double> & to_vec){
    if(from_vec.size()!= to_vec.size())
	{
	    throw std::out_of_range(
		"error in proj2vector: the size of from_vec "
		"differs from that of to_vec");
	}
    std::vector<double> ret_vec(from_vec.size());
    double num = std::inner_product(from_vec.begin(), from_vec.end(), to_vec.begin(), 0.0);
    double den = std::inner_product(to_vec.begin(), to_vec.end(), to_vec.begin(), 0.0);
    std::transform(to_vec.begin(), to_vec.end(), ret_vec.begin(),std::bind(std::multiplies<double>(), std::placeholders::_1, num/den));
 
    return ret_vec;
}
void cross_product(std::vector<double> & vector_a, std::vector<double> & vector_b, std::vector<double> & ret_vector) {
    if(vector_a.size()!= vector_b.size()){
	    throw std::out_of_range(
		"the size of vectors are different ");
	} else if(vector_b.size() != ret_vector.size()){
	    throw std::out_of_range(
		"the size of vectors are different ");
	} 

    ret_vector.at(0) = (vector_a[1] * vector_b[2] - vector_a[2] * vector_b[1]);
    ret_vector.at(1) = (vector_a[2] * vector_b[0] - vector_a[0] * vector_b[2]);
    ret_vector.at(2) = (vector_a[0] * vector_b[1] - vector_a[1] * vector_b[0]);
    return;
}