#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include <utility>
#include <stdexcept>

using namespace std;

long long decode_base_value(int base, const string& value) {
    // Decodes a value from a given base to base 10.
    long long result = 0;
    for (char digit : value) {
        result *= base;
        if (isdigit(digit)) {
            result += digit - '0'; // For '0'-'9'
        } else if (isupper(digit)) {
            result += digit - 'A' + 10; // For 'A'-'Z'
        } else if (islower(digit)) {
            result += digit - 'a' + 10; // For 'a'-'z'
        } else {
            throw invalid_argument("Invalid character in input value");
        }
    }
    return result;
}

tuple<int, int, vector<pair<int, int>>> read_input(int n, int k, const vector<pair<int, pair<int, string>>>& data) {
    vector<pair<int, int>> roots;

    for (const auto& el : data) {
        int x = el.first;  // x is the first element of the pair
        int base = el.second.first;  // base is the first element of the nested pair
        long long y = decode_base_value(base, el.second.second);  // decode y

        roots.emplace_back(x, y);
    }

    return make_tuple(n, k, roots);
}

void create_matrix_and_vector(const vector<pair<int, int>>& roots, int k, vector<vector<double>>& A, vector<double>& b) {
    A.resize(k, vector<double>(k));
    b.resize(k);

    for (int i = 0; i < k; ++i) {
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
    int n = 9;
    int k = 6;

    vector<pair<int, pair<int, string>>> input_data = {
        {1, {10, "28735619723837"}},
        {2, {16, "1A228867F0CA"}},
        {3, {12, "32811A4AA0B7B"}},
        {4, {11, "917978721331A"}},
        {5, {16, "1A22886782E1"}},
        {6, {10, "28735619654702"}},
        {7, {14, "71AB5070CC4B"}},
        {8, {9, "122662581541670"}},
        {9, {8, "642121030037605"}}
    };

    int num_points, degree;
    vector<pair<int, int>> roots;
    tie(num_points, degree, roots) = read_input(n, k, input_data);

    vector<vector<double>> A;
    vector<double> b;
    create_matrix_and_vector(roots, degree, A, b);

    vector<double> coeffs = solve_polynomial(A, b);

    double constant_term = coeffs[0];
    cout << "The constant term (c) is: " << constant_term << endl;

    return 0;
}
