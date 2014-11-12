#include "FunctionNetwork.h"
#include <iostream>

FunctionNetwork::FunctionNetwork(
	int input_size,
	int hidden_layer_size,
	int output_size) :
	input_size_(input_size),
	hidden_layer_size_(hidden_layer_size),
	output_size_(output_size)
{
	// + 1 to include thresholds
	input_weights_ = new float[(hidden_layer_size_ + 1) * input_size_];
	output_weights_ = new float[(output_size_ + 1) * hidden_layer_size_];
}

FunctionNetwork::~FunctionNetwork()
{
	delete [] input_weights_;
	delete [] output_weights_;
}

std::vector<float> FunctionNetwork::CalculateOutput(const std::vector<float>& input)
{
	if (!(input.size() == input_size_)) // Bad input
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
			synaptic_hidden_signal[i] = input_weights_[j + input_size_*i] * input[j];
		}
	}

	// Calculate the synaptic output from the hidden layer
	for (int i = 0; i < hidden_layer_size_; ++i)
	{
		synaptic_hidden_signal[i] = UnipolarSigmoidal(synaptic_hidden_signal[i]);
	}

	// Calculate the synaptic input to the output layer
	for (int i = 0; i < output_size_; ++i) // rows
	{
		for (int j = 0; j < hidden_layer_size_ + 1; ++j) // columns
		{
			outputs[i] = output_weights_[j + hidden_layer_size_*i] * synaptic_hidden_signal[j];
		}
	}

	// Calculate the synaptic output from the output layer
	for (int i = 0; i < output_size_; ++i)
	{
		outputs[i] = UnipolarSigmoidal(outputs[i]);
	}

	return outputs;
}

float FunctionNetwork::UnipolarSigmoidal(float x)
{
	return 0.0f;
}