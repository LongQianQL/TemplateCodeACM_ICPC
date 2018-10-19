#include <bits/stdc++.h>
using namespace std;

class Matrix{
public:
    vector<vector<int>> matrix;
    int m;
    int n;
    Matrix(){}
    Matrix(int m, int n){
        this->m=m;
        this->n=n;
        this->matrix = vector<vector<int>>(m, vector<int>(n, 0));
    }

    Matrix operator *(const Matrix& m){
        assert(this->n == m.m);

        Matrix newmatrix(this->m, this->n);
        for(int i = 0; i<this->m; i ++){
            for(int j = 0; j<m.n; j++){
                int sum = 0;
                for(int k = 0; k<this->n; k++){
                    sum += this->matrix[i][k]*m.matrix[k][j];
                }
                newmatrix.matrix[i][j] = sum;
            }
        }
        return newmatrix;
    }

    Matrix& operator = (const Matrix& m){
        this->matrix=m.matrix;
        return *this;
    }
};

Matrix power(Matrix m, int n){
    if(n == 1) return m;
    if(n & 1) return power(m, n-1) * m;
    Matrix matrix = power(m, n/2);
    return matrix*matrix;
}
