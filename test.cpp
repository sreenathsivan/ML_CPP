#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
vector<vector<T>> vector2DSlice(vector<vector<T>> vec, int startRow, int endRow, int startCol, int endCol)
{

  std::vector<std::vector<T>> sliced;

  for (int i = startRow; i < endRow; ++i)
  {
    std::vector<T> row(vec[i].begin() + startCol, vec[i].begin() + endCol + 1);
    sliced.push_back(row);
  }
  return sliced;
}

int main()
{
  string filepath = "file.txt";
  ifstream data_stream(filepath);

  // Check if the file opened successfully
  if (!data_stream)
  {
    cout << "Error: Unable to open the file." << endl;
    return 1;
  }

  string line;
  vector<vector<vector<float>>> vector3D; // 3D vector to store the data

  for (int i = 0; i < 1000; i++) {
        vector<vector<float>> temp2D; // Temporary 2D vector
        for (int j = 0; j < 10; j++) {
            vector<float> temp1D(100, 0.0f); // 1D vector initialized with 0.0
            temp2D.push_back(temp1D); // Add 1D vector to 2D vector
        }
        vector3D.push_back(temp2D); // Add 2D vector to 3D vector
    }

    for(int i=0;i<1000;i++)
  {
    for(int j=0;j<10;j++)
    {
      for(int k=0;k<100;k++)
      {
        cout<<vector3D[i][j][k];

      }
    }
  }




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
        if (index1 >= vector3D.size())
        {
          vector3D.resize(index1 + 1);
        }
      }
      else if (i == 1)
      {
        index2 = stoi(feature);
        if (index2 >= vector3D[index1].size())
        {
          vector3D[index1].resize(index2 + 1);
        }
      }

      if (i == 3)
      {
        // vector[i]=(stof(feature));
        vector3D[index1][index2].push_back(stof(feature));
      }

      i++;
    }
  }

  // Close the file
  data_stream.close();

  // Example output of the 3D vector
  for (const auto &matrix : vector3D)
  {
    for (const auto &row : matrix)
    {
      for (const auto &value : row)
      {
        // cout << value << "\n";
      }
    }
    cout << "----" << endl; // Separator for matrices
  }

  vector<vector<int>> vector2D;
  // std::vector<std::vector<int>> vector2D(1000, std::vector<int>(128, 0));

  // vector2D = {{1, 2, 3},
  //             {1, 2, 3}};
  for (int i = 0; i < 1000; i++)
  {
    vector<int> row;
    for (int j = 0; j < 128; j++)
    {
      row.push_back(0);
    }
    vector2D.push_back(row);
  }


  int ds = 16;

  int m = 0;
  int skip = 0;
  int start = m * ds;
  int M = 8;
  while (m < M)
  {

    int end = (m + 1) * ds;
    cout << "start" << start << "\n";
    cout << "end" << end;
    // std::vector<std::vector<int>> sliced = vector2DSlice(vector2D, 0, 2, 0, 1);

    cout << "\n\n";
    start = end + 1;
    m++;
  }

  // std::vector<std::vector<int>> sliced = vector2DSlice(vector2D, 0, 2, 0, 1);

  // int M=8;
  // for(int m=0;m<M;m++)
  // {

  // }
  // cout<<size(sliced);
  // cout<<size(sliced[0]);

  // for (int i = 0; i <= size(sliced); i++)
  // {
  //   for (int j = 0; j <= size(sliced[0]); j++)
  //   {
  //     cout<<sliced[i][j]<<"\t";

  //   }
  //   cout<<"\n";
  // }
  return 0;
}
