#include "FunctionNetwork.h"
#include <iostream>
#include <random>
#include <cmath>

std::default_random_engine FunctionNetwork::generator_;

FunctionNetwork::FunctionNetwork(
	int input_size,
	int hidden_layer_size,
	int output_size) :
	input_size_(input_size),
	hidden_layer_size_(hidden_layer_size),
	output_size_(output_size)
{
	// + 1 to include thresholds
	int n_input_weights = (hidden_layer_size_ + 1) * input_size_;
	int n_output_weights = (output_size_ + 1) * hidden_layer_size_;
	input_weights_ = new float[n_input_weights];
	output_weights_ = new float[n_output_weights];

  	//std::default_random_engine generator;
	std::normal_distribution<float> distribution(0.0f,1.0f);
	// Random init weights
	for (int i = 0; i < n_input_weights; ++i)
	{
		input_weights_[i] = distribution(generator_);
	}
	for (int i = 0; i < n_output_weights; ++i)
	{
		output_weights_[i] = distribution(generator_);
	}
}

FunctionNetwork::FunctionNetwork(const FunctionNetwork& f) :
	input_size_(f.input_size_),
	hidden_layer_size_(f.hidden_layer_size_),
	output_size_(f.output_size_)
{
	//FunctionNetwork(input_size_, hidden_layer_size_, output_size_);
  
  // + 1 to include thresholds
  int n_input_weights = (hidden_layer_size_ + 1) * input_size_;
  int n_output_weights = (output_size_ + 1) * hidden_layer_size_;
  input_weights_ = new float[n_input_weights];
  output_weights_ = new float[n_output_weights];
  
  // Copy weights
  for (int i = 0; i < n_input_weights; ++i)
  {
    input_weights_[i] = f.input_weights_[i];
  }
  for (int i = 0; i < n_output_weights; ++i)
  {
    output_weights_[i] = f.output_weights_[i];
  }
}

FunctionNetwork::~FunctionNetwork()
{
	//delete [] input_weights_;
	//delete [] output_weights_;
}

std::vector<float> FunctionNetwork::CalculateOutput(const std::vector<float>& input)
{
	if (!(input.size() == input_size_ + 1)) // Bad input
	{
		std::cout << "ERRORORRORORORORRRR!!!!!" << std::endl;
		return std::vector<float>();
	}

	// Allocate memory (put these as class mambers to make it faster?)
	std::vector<float> synaptic_hidden_signal(hidden_layer_size_);
	std::vector<float> outputs(output_size_);

	// Calculate the synaptic input to the hidden layer
	for (int i = 0; i < hidden_layer_size_; ++i) // rows
	{
		for (int j = 0; j < input_size_ + 1; ++j) // columns
		{
			synaptic_hidden_signal[i] += input_weights_[j + input_size_*i] * input[j];
		}
	}


	// Calculate the synaptic output from the hidden layer
	for (int i = 0; i < hidden_layer_size_; ++i)
	{
		synaptic_hidden_signal[i] = UnipolarSigmoidal(synaptic_hidden_signal[i]);
	}

	synaptic_hidden_signal.push_back(-1.0f); // Threshold input

	// Calculate the synaptic input to the output layer
	for (int i = 0; i < output_size_; ++i) // rows
	{
		for (int j = 0; j < hidden_layer_size_ + 1; ++j) // columns
		{
			outputs[i] += output_weights_[j + hidden_layer_size_*i] * synaptic_hidden_signal[j];
		}
	}

	// Calculate the synaptic output from the output layer
	for (int i = 0; i < output_size_; ++i)
	{
		outputs[i] = UnipolarSigmoidal(outputs[i]);
	}

	return outputs;
}

void FunctionNetwork::Mutate(float mutation_rate)
{
	int n_input_weights = (hidden_layer_size_ + 1) * input_size_;
	int n_output_weights = (output_size_ + 1) * hidden_layer_size_;
	
	std::uniform_int_distribution<int> index_distribution(0, n_input_weights + n_output_weights);
	std::normal_distribution<float> mutation_distribution(0.0f, mutation_rate);

	int mutation_index = index_distribution(generator_);
	float mutation = mutation_distribution(generator_);

	if (mutation_index < n_input_weights)
		input_weights_[mutation_index] += mutation;
	else
		output_weights_[mutation_index - n_input_weights] += mutation;
}

float FunctionNetwork::UnipolarSigmoidal(float x)
{
	return 1 / (1 + exp(-x));
}