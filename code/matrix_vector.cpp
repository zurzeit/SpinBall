#include "matrix_vector.hpp"

Matrix::Matrix(): m_nrow(0), m_ncol(0){}
// constructor with size
Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol)
{
    reset_buffer(nrow, ncol);
}
// constructor with vector
Matrix::Matrix
(
    size_t nrow, size_t ncol, std::vector<double> const & vec
)
    : m_nrow(nrow), m_ncol(ncol)
{

    if (size() != vec.size())
    {
        throw std::out_of_range("number of elements mismatch");
    }

    reset_buffer(m_nrow, m_ncol);

    for(size_t i = 0; i < m_nrow; i ++){
        for(size_t j = 0; j < m_ncol; j ++){
            m_buffer[i][j] = vec.at(i*m_ncol + j);
        }
    }

    // m_buffer = vec;
}

// // assignment operator with vec
// Matrix & Matrix::operator=(std::vector<double> const & vec)
// {
//     if (size() != vec.size())
//     {
//         throw std::out_of_range("number of elements mismatch");
//     }

//     size_t k = 0;
//     for (size_t i=0; i<m_nrow; ++i)
//     {
//         for (size_t j=0; j<m_ncol; ++j)
//         {
//             (*this)(i,j) = vec[k];
//             ++k;
//         }
//     }

//     return *this;
// }
// copy constructor
Matrix::Matrix(const Matrix & other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
{
    m_buffer = other.m_buffer;
    m_nrow = other.m_nrow;
    m_ncol = other.m_ncol;
}
// copy assign operator
Matrix & Matrix::operator=(const Matrix & other)
{
    m_buffer = other.m_buffer;
    m_nrow = other.m_nrow;
    m_ncol = other.m_ncol;
    return (*this);
}
// move constructor
Matrix::Matrix( Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
{
    m_buffer.swap(other.m_buffer);
    other.m_nrow = 0;
    other.m_ncol = 0;
}
// move assign operator
Matrix & Matrix::operator=(Matrix && other)
{
    if(this != &other){
        m_buffer.swap(other.m_buffer);
        m_nrow = other.m_nrow;
        m_ncol = other.m_ncol;
    }
    return (*this);
}
// destructor
Matrix::~Matrix(){
    for(size_t i = 0; i < m_nrow; i ++){
        m_buffer[i].clear();
        m_buffer[i].shrink_to_fit();
    }
	m_buffer.clear();
    m_buffer.shrink_to_fit();
    m_nrow = 0;
    m_ncol = 0;
}
// void Matrix::setter(std::pair <size_t, size_t> key, double val){
//     m_buffer[index(key.first,key.second)] = val;
// }
// double Matrix::getter(std::pair <size_t, size_t> key){
//     return m_buffer[index(key.first,key.second)];
// }

// Matrix & Matrix::operator+=(Matrix const & mat1)
// {
// 	if ((*this).ncol() != mat1.ncol())
// 	{
// 	    throw std::out_of_range(
// 		"the number of column "
// 		"differs from that of second matrix column");
// 	}
// 	else if ((*this).nrow() != mat1.nrow())
// 	{
// 	    throw std::out_of_range(
// 		"the number of first matrix row "
// 		"differs from that of second matrix row");
// 	}

// 	std::transform(m_buffer.begin(), m_buffer.end(), mat1.m_buffer.begin(), 
//         m_buffer.begin(), std::plus<double>());

// 	return (*this);
// }
bool Matrix::operator==(const Matrix & other)const{
	if(other.nrow()!=nrow() || other.ncol() != ncol())
	    return false;
    if(m_buffer != other.m_buffer)
        return false;
    for(size_t i = 0; i < nrow(); i ++){
        if(m_buffer[i] != other.m_buffer[i])
            return false;
    }
    return true;
}

// to locate the value
double   Matrix::operator() (size_t row, size_t col) const
{
	return m_buffer[row][col];
}
double & Matrix::operator() (size_t row, size_t col)
{
	return m_buffer[row][col];
}
std::vector<double>   Matrix::operator() (size_t row) const
{
	return m_buffer[row];
}
std::vector<double> & Matrix::operator() (size_t row)
{
	return m_buffer[row];
}
size_t Matrix::nrow() const { return m_nrow; }//return the number of row of the matrix
size_t Matrix::ncol() const { return m_ncol; }//return the number of col of the matrix

size_t Matrix::size() const { return m_nrow * m_ncol; }//return the number of element of the matrix

void Matrix::get_mat(){
	std::cout << "row:" << m_nrow << " col:" << m_ncol << std::endl;
	size_t k = 0;
	for(size_t i = 0; i < m_nrow; i ++){
	    for(size_t j = 0; j < m_ncol; j ++){
		std::cout << m_buffer[i][j] << " ";
		k ++;
	    }
	    std::cout << "\n";
	}
}
void Matrix::get_row(size_t row){
	std::cout << "row:" << row << std::endl;
	size_t k = 0;

    for(size_t j = 0; j < m_ncol; j ++){
        std::cout << m_buffer[row][j] << " ";
    }
    std::cout << "\n";
}



size_t Matrix::index(size_t row, size_t col) const
{
	return row * m_ncol + col;
}

void Matrix::reset_buffer(size_t nrow, size_t ncol)
{

	const size_t nelement = nrow * ncol;

    m_buffer.resize(nrow);
    for(size_t i = 0; i < m_buffer.size(); i ++){
        m_buffer[i].resize(ncol);
    }
    m_nrow = nrow;
    m_ncol = ncol;
    // std::cout << m_buffer.size();
    // std::cout << m_buffer[0].size();
}

// Matrix operator*(Matrix const & mat1, Matrix const & mat2)
// {
//     return multiply_naive(mat1, mat2);
// }

// Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2){
//     if (mat1.ncol() != mat2.nrow())
//     {
//         throw std::out_of_range(
//             "the number of first matrix column "
//             "differs from that of second matrix row");
//     }

//     Matrix ret(mat1.nrow(), mat2.ncol());

//     for (size_t i=0; i<ret.nrow(); ++i)
//     {
//         for (size_t k=0; k<ret.ncol(); ++k)
//         {
//             double v = 0;
//             for (size_t j=0; j<mat1.ncol(); ++j)
//             {
//                 v += mat1(i,j) * mat2(j,k);
//             }
//             ret(i,k) = v;
//         }
//     }
//     return ret;
// }


// Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t tsize){
//     if (mat1.ncol() != mat2.nrow())
//     {
//         throw std::out_of_range(
//             "the number of first matrix column "
//             "differs from that of second matrix row");
//     }

//     Matrix ret(mat1.nrow(), mat2.ncol());
//     Matrix mat2_col_major = mat2.row_maj_to_col_maj();
//     size_t tile_limit_i, tile_limit_j, tile_limit_k;
//     size_t k_dim = mat1.ncol();

//     double tmp = 0;
//     for (size_t tile_start_i=0; tile_start_i<ret.nrow(); tile_start_i += tsize){
//         for (size_t tile_start_j=0; tile_start_j<ret.ncol(); tile_start_j += tsize){
//             for (size_t tile_start_k=0; tile_start_k<k_dim; tile_start_k += tsize){

//                 tile_limit_i = std::min(tile_start_i + tsize, ret.nrow());
//                 tile_limit_j = std::min(tile_start_j + tsize, ret.ncol());
//                 tile_limit_k = std::min(tile_start_k + tsize, mat1.ncol());

//                 for (size_t i = tile_start_i; i < tile_limit_i; i ++){
//                     for (size_t j = tile_start_j; j < tile_limit_j; j ++){
//                         tmp = 0;
//                         for (size_t k = tile_start_k; k < tile_limit_k; k ++){
//                             tmp += mat1.m_buffer[i*k_dim+k] * mat2_col_major.m_buffer[k + j*k_dim];
//                         }
//                         ret.m_buffer[i*ret.ncol() + j] += tmp;
//                     }
//                 }
//             }
//         }
//     }
    
//     return ret;
// }

// Matrix Matrix::row_maj_to_col_maj() const
// {
//     Matrix ret(nrow(), ncol());
//     for(size_t i = 0; i < nrow(); i ++){
//         for(size_t j = 0; j < ncol(); j ++){
//             ret.m_buffer[j*nrow() + i] = m_buffer[i*ncol() + j];
//         }
//     }
//     return ret;

// }
// Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2){
//     Matrix ans_mat(mat1.nrow(), mat2.ncol());
//     int m = mat1.nrow();
//     int n = mat2.ncol();
//     int k = mat1.ncol();
//     double alpha = 1;
//     double beta = 0;

//     cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
//            m, n, k, alpha, mat1.get_buffer_pointer(), k, mat2.get_buffer_pointer(), n, beta, ans_mat.get_buffer_pointer(), n);
//     return ans_mat;
// }


