#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

class NeuralNetwork {
public:
    NeuralNetwork(int input_size, int hidden_size, int output_size);
    void train(const vector<vector<double>>& inputs, const vector<double>& targets, int epochs, double learning_rate);
    double predict(const vector<double>& input);
    
private:
    int input_size;
    int hidden_size;
    int output_size;
    vector<vector<double>> weights_input_hidden;
    vector<vector<double>> weights_hidden_output;
    vector<double> hidden_layer;
    
    double sigmoid(double x);
    double sigmoid_derivative(double x);
    void forward(const vector<double>& input);
    void backward(const vector<double>& input, double target, double learning_rate);
};

NeuralNetwork::NeuralNetwork(int input_size, int hidden_size, int output_size)
    : input_size(input_size), hidden_size(hidden_size), output_size(output_size) {
    srand(time(0));
    
    // Initialize weights with random values
    weights_input_hidden.resize(input_size, vector<double>(hidden_size));
    for (auto& row : weights_input_hidden) {
        for (auto& weight : row) {
            weight = static_cast<double>(rand()) / RAND_MAX;  // Random weight
        }
    }
    
    weights_hidden_output.resize(hidden_size, vector<double>(output_size));
    for (auto& row : weights_hidden_output) {
        for (auto& weight : row) {
            weight = static_cast<double>(rand()) / RAND_MAX;  // Random weight
        }
    }
    
    hidden_layer.resize(hidden_size);
}

double NeuralNetwork::sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double NeuralNetwork::sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

void NeuralNetwork::forward(const vector<double>& input) {
    // Calculate hidden layer
    for (int j = 0; j < hidden_size; ++j) {
        hidden_layer[j] = 0;
        for (int i = 0; i < input_size; ++i) {
            hidden_layer[j] += input[i] * weights_input_hidden[i][j];
        }
        hidden_layer[j] = sigmoid(hidden_layer[j]);
    }
}

void NeuralNetwork::backward(const vector<double>& input, double target, double learning_rate) {
    // Calculate output layer
    vector<double> output(output_size);
    for (int j = 0; j < output_size; ++j) {
        output[j] = 0;
        for (int i = 0; i < hidden_size; ++i) {
            output[j] += hidden_layer[i] * weights_hidden_output[i][j];
        }
        output[j] = sigmoid(output[j]);
    }

    // Calculate error
    double error = target - output[0];

    // Update weights for the hidden to output layer
    for (int j = 0; j < output_size; ++j) {
        for (int i = 0; i < hidden_size; ++i) {
            weights_hidden_output[i][j] += learning_rate * error * sigmoid_derivative(output[j]) * hidden_layer[i];
        }
    }

    // Update weights for the input to hidden layer
    for (int j = 0; j < hidden_size; ++j) {
        for (int i = 0; i < input_size; ++i) {
            weights_input_hidden[i][j] += learning_rate * error * sigmoid_derivative(output[0]) * weights_hidden_output[j][0] * input[i];
        }
    }
}

void NeuralNetwork::train(const vector<vector<double>>& inputs, const vector<double>& targets, int epochs, double learning_rate) {
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (size_t i = 0; i < inputs.size(); ++i) {
            forward(inputs[i]);
            backward(inputs[i], targets[i], learning_rate);
        }
    }
}

double NeuralNetwork::predict(const vector<double>& input) {
    forward(input);
    return hidden_layer[0];  // Return output from the first output node
}

int main() {
    NeuralNetwork nn(2, 2, 1);
    
    // Training data for XOR problem
    vector<vector<double>> inputs = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    vector<double> targets = {0, 1, 1, 0}; // XOR target values
    
    nn.train(inputs, targets, 10000, 0.1);
    
    // Test the neural network
    for (const auto& input : inputs) {
        cout << "Input: (" << input[0] << ", " << input[1] << ") -> Prediction: " << nn.predict(input) << endl;
    }
    
    return 0;
}
