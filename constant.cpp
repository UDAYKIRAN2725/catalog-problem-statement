#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <utility>

using namespace std;

int decode_base_value(int base, const string& value) {
    // Decodes a value from a given base to base 10.
    return stoi(value, nullptr, base);
}

tuple<int, int, vector<pair<int, int>>> read_input(int n, int k, const vector<pair<int, pair<int, string>>>& data) {
    // Extracts n, k and the roots from the input list.
    vector<pair<int, int>> roots;

    for (const auto& el : data) {
        int x = el.first;  // x is the first element of the pair
        int base = el.second.first;  // base is the first element of the nested pair
        int y = decode_base_value(base, el.second.second);  // decode y

        roots.emplace_back(x, y);
    }

    return make_tuple(n, k, roots);
}

void create_matrix_and_vector(const vector<pair<int, int>>& roots, int k, vector<vector<double>>& A, vector<double>& b) {
    // Creates the matrix of the system of equations based on the roots.
    A.resize(k, vector<double>(k));
    b.resize(k);

    // Set up the matrix (A) and the result vector (b)
    for (int i = 0; i < k; ++i) {  // Only take the first k points
        int x = roots[i].first;
        int y = roots[i].second;

        for (int j = 0; j < k; ++j) {
            A[i][j] = pow(x, j);  // Create the row for x, from x^0 to x^(k-1)
        }
        b[i] = y;
    }
}

vector<double> solve_polynomial(vector<vector<double>>& A, vector<double>& b) {
    int n = A.size();
    vector<double> coeffs(n);

    // Gaussian elimination
    for (int i = 0; i < n; ++i) {
        // Forward elimination
        for (int j = i + 1; j < n; ++j) {
            double ratio = A[j][i] / A[i][i];
            for (int k = i; k < n; ++k) {
                A[j][k] -= ratio * A[i][k];
            }
            b[j] -= ratio * b[i];
        }
    }

    // Back substitution
    for (int i = n - 1; i >= 0; --i) {
        coeffs[i] = b[i];
        for (int j = i + 1; j < n; ++j) {
            coeffs[i] -= A[i][j] * coeffs[j];
        }
        coeffs[i] /= A[i][i];
    }

    return coeffs;
}

int main() {
    // Sample input
    int n = 4;
    int k = 3;
    
    vector<pair<int, pair<int, string>>> input_data = {
        {1, {10, "4"}},
        {2, {2, "111"}},
        {3, {10, "12"}},
        {6, {4, "213"}}
    };

    // Step 1: Read the input and decode the values
    int num_points, degree;
    vector<pair<int, int>> roots;
    tie(num_points, degree, roots) = read_input(n, k, input_data);

    // Step 2: Create the matrix (A) and vector (b) for the system of equations
    vector<vector<double>> A;
    vector<double> b;
    create_matrix_and_vector(roots, degree, A, b);

    // Step 3: Solve the system to find the coefficients
    vector<double> coeffs = solve_polynomial(A, b);

    // Step 4: Print the constant term (c), which is the first coefficient (x^0)
    double constant_term = coeffs[0];  // The constant term is the first coefficient (x^0)
    cout << "The constant term (c) is: " << constant_term << endl;

    return 0;
}
