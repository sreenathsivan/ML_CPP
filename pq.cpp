#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <numeric>
#include <bits/stdc++.h>
using namespace std;

class PQ
{
public:
  string filepath;
  string vec_filepath;
  vector<vector<vector<float>>> codewords;
  vector<int> code_vector;

  template <typename T>
  vector<vector<T>> read2DVector(string vec_filepath)
  {
    vector<vector<T>> vector_2d;
    this->vec_filepath = vec_filepath;
    ifstream data_stream(this->vec_filepath);
    string line;
    if (!data_stream)
    {
      cout << "Error: Unable to open the file." << endl;
    }
    while (getline(data_stream, line))
    {
      stringstream ss(line);
      string feature;

      int i = 0;
      int index1 = -1, index2 = -1;

      //   // Read indices and initialize dimensions
      while (getline(ss, feature, ','))
      {
        if (i == 0)
        {
          index1 = stoi(feature);
          if (index1 >= vector_2d.size())
          {
            vector_2d.resize(index1 + 1);
          }
        }
        if (i == 1)
        {
          index2 = stoi(feature);
          if (index2 >= vector_2d[index1].size())
          {
            vector_2d[index1].resize(index2 + 1);
          }
          //  index2 = stof(feature);
          // cout<<index1<<endl;
          // cout<<index2<<endl;
        }
        if (i == 2)
        {

          vector_2d[index1][index2] = stod(feature);
        }

        i++;
      }
    }
    return vector_2d;
  }

  vector<vector<vector<float>>> readDatafromFile(string filepath)
  {

    this->filepath = filepath;
    ifstream data_stream(this->filepath);
    string line;

    // Check if the file opened successfully
    if (!data_stream)
    {
      cout << "Error: Unable to open the file." << endl;
    }

    // 3D vector to store the data

    while (getline(data_stream, line))
    {
      stringstream ss(line);
      string feature;

      int i = 0;
      int index1 = -1, index2 = -1;

      // Read indices and initialize dimensions
      while (getline(ss, feature, ','))
      {
        if (i == 0)
        {
          index1 = stoi(feature);
          if (index1 >= this->codewords.size())
          {
            this->codewords.resize(index1 + 1);
          }
        }
        else if (i == 1)
        {
          index2 = stoi(feature);
          if (index2 >= this->codewords[index1].size())
          {
            this->codewords[index1].resize(index2 + 1);
          }
        }

        if (i == 3)
        {
          // vector[i]=(stof(feature));
          this->codewords[index1][index2].push_back(stod(feature));
        }

        i++;
      }
    }

    // Close the file
    data_stream.close();
    return this->codewords;
  }

  vector<vector<float>> getRandomVector(int N, int D)
  {
    vector<vector<float>> vector2D;
    for (int i = 0; i < N; i++)
    {
      vector<float> row;
      for (int j = 0; j < D; j++)
      {
        row.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
      }
      vector2D.push_back(row);
    }
    return vector2D;
  }
  vector<vector<int>> getZeroVector(int N, int M)
  {
    vector<vector<int>> vector2D;
    for (int i = 0; i < N; i++)
    {
      vector<int> row;
      for (int j = 0; j < M; j++)
      {
        row.push_back(0.0);
      }
      vector2D.push_back(row);
    }
    return vector2D;
  }
  template <typename T>
  vector<vector<T>> vector2DSlice(vector<vector<T>> vec, int startRow, int endRow, int startCol, int endCol)
  {

    std::vector<std::vector<T>> sliced;

    for (int i = startRow; i < endRow; ++i)
    {
      std::vector<T> row(vec[i].begin() + startCol, vec[i].begin() + endCol);
      sliced.push_back(row);
    }
    auto o = size(sliced);
    return sliced;
  }

  vector<vector<int>> encode(vector<vector<float>> vec, int N, int D, int M, vector<vector<vector<float>>> codewords)
  {
    // auto codes = this->getZeroVector(M, N);
    vector<vector<int>> codes;
    int Ds = D / M;
    int m = 0;
    int skip = 0;
    int start = m * Ds;
    int end;
    vector<vector<float>> temp;
    while (m < M)
    {

      end = (m + 1) * Ds;
      // if (m == 4)
      // {
        // cout << "start" << start << "\n";
        // cout << "end" << end;
        std::vector<std::vector<float>> vec_sub = this->vector2DSlice(vec, 0, N, start, end);
        // auto a = vec_sub[10000];

        auto index_array = this->vq(vec_sub, codewords[m]);

        codes.push_back(index_array);
      // }
      // cout << "\n\n";
      start = end;
      m++;
    }
    // auto c = codes[0][1197];
    return codes;
  }
  template <typename T>
  vector<vector<T>> transpose(vector<vector<T>> codes)
  {

    vector<vector<T>> vect1(size(codes[0]), std::vector<T>(size(codes), 0));

    for (int i = 0; i < size(codes); i++)
    {

      for (int j = 0; j < size(codes[0]); j++)
      {
        vect1[j][i] = codes[i][j];
      }
    }
    return vect1;
  }

  int distanceCalc(vector<float> row, vector<vector<float>> codeword_m)
  {

    double distance = 0.0;

    double min_distance = 1000;
    int index = 0;
    for (int i = 0; i < size(codeword_m); i++)
    {

      // row
      double sum = 0.0;
      double calc_distance = 0.0;
      for (int j = 0; j < size(codeword_m[0]); j++)
      {
        auto code_element = codeword_m[i][j];
        auto element = row[j];
        sum += ((element - code_element) * (element - code_element));
      }
      calc_distance = sqrt(sum);
      // cout<<calc_distance<<endl;
      if (calc_distance < min_distance)
      {
        min_distance = calc_distance;
        index = i;
      }
    }
    // cout<<index<<endl;
    // exit(0);
    return index;
  }
  template <typename T>
  vector<int> vq(vector<vector<T>> vec_sub, vector<vector<float>> codeword_m)
  {
    vector<int> index_array(10000);
    for (int i = 0; i < size(vec_sub); i++)
    {

      // row.push_back(vec[i]);
      int index = this->distanceCalc(vec_sub[i], codeword_m);
      // cout<<index<<endl;
      index_array[i] = index;

      // auto row=vec[i];
      // for(int j=0;j<size(vec[0]);j++)
      // {
      //   float element=vec[i][j];
      // }
    }
    return index_array;
  }
  template <typename T>
  double checkCosineSimiliarity(vector<T> row, vector<T> py_row)
  {
    double dot_prod = dot_product(row, py_row);
    double magnitude1 = this->magnitude(row);
    double magnitude2 = this->magnitude(py_row);
    double sim = (dot_prod) / ((magnitude1) * (magnitude2));

    return sim;
  }
  template <typename T>

  double magnitude(vector<T> vec)
  {
    // return std::sqrt(this->dot_product(v, v));
    double sum = 0.0;
    for (int i = 0; i < size(vec); i++)
    {

      sum += (vec[i] * vec[i]);
    }
    return sqrt(sum);
  }
  template <typename T>
  double dot_product(vector<T> vec1, vector<T> vec2)
  {
    double ret = 0;
    for (unsigned i = 0; i < size(vec1); ++i)
    {
      ret += vec1[i] * vec2[i];
    }
    return ret;
  }
  template <typename T>
  vector<T> reshape(vector<vector<T>> vec)
  {
    vector<T> vec1;
    for (int i = 0; i < size(vec); i++)
    {
      for (int j = 0; j < size(vec[0]); j++)
      {

        vec1.push_back(vec[i][j]);
      }
    }
    return vec1;
  }
};

int main()
{

  string filepath = "../file.txt";

  PQ pq;

  auto codewords = pq.readDatafromFile(filepath);

  // for(int i=0;i<size(codewords);i++){
  //   for(int j=0;j<size(codewords[0]);j++){
  //     for(int k=0;k<size(codewords[0][0]);k++){
  //       cout<<i<< " "<<j << " "<<k<<" ";
  //       cout<<pq.codewords[i][j][k];
  //       cout<<"\n";
  //     }
  //   }
  // }
  int N = 10000;
  int D = 128;
  int M = 8;
  // auto X = pq.getRandomVector(N, D);
  auto X = pq.read2DVector<float>("../vec.txt");

  // for (int i = 0; i <= size(X); i++)
  // {
  //   for (int j = 0; j <= size(X[0]); j++)
  //   {
  //     cout << X[i][j] << "\t";
  //   }
  //   cout << "\n";
  // }

  auto X_code = pq.encode(X, N, D, M, codewords);

  X_code = pq.transpose(X_code);
  auto X_codePy = pq.read2DVector<int>("../X_code.txt");

  for (int i = 0; i < size(X_code); i++)
  {
    for (int j = 0; j < size(X_code[0]); j++)
    {
      if (X_code[i][j] != X_codePy[i][j])
      {
        cout << i << endl
             << j;
      }
    }
  }
  // cout<<X_code[1197][4]<<endl;
  // cout<<X_codePy[1197][4];
  exit(0);

  auto X_code_reshape = pq.reshape(X_code);
  auto X_codePy_reshape = pq.reshape(X_codePy);
  double sim = pq.checkCosineSimiliarity(X_code_reshape, X_codePy_reshape);
  std::vector<int> vector1 = X_code_reshape;
  std::vector<int> vector2 = X_codePy_reshape;

  int result = std::inner_product(vector1.begin(), vector1.end(), vector2.begin(), 0);

  auto norm1 = pq.magnitude(X_code_reshape);
  auto norm2 = pq.magnitude(X_codePy_reshape);
  auto n = norm1 * norm2;

  double sim1 = (result) / ((norm1) * (norm2));

  for (int i = 0; i < size(vector1); i++)
  {
    if (vector1[i] != vector2[i])
    {
      cout << vector1[i] << endl
           << i << endl;
      cout << vector2[i] << endl;
    }
  }

  return 0;
}
