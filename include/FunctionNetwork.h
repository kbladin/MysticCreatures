#ifndef FUNCTIONNETWORK_H
#define FUNCTIONNETWORK_H

#include <vector>
#include <random>

class FunctionNetwork
{
private:
	int input_size_;
	int hidden_layer_size_;
	int output_size_;
	std::default_random_engine generator_;
	
	std::vector<float> input_weights_;
	std::vector<float> output_weights_;

	float UnipolarSigmoidal(float x);

public:
	FunctionNetwork():input_size_(0),hidden_layer_size_(0),output_size_(0){};
	FunctionNetwork(
		int input_size,
		int hidden_layer_size,
		int output_size);
	//FunctionNetwork(const FunctionNetwork& f);
	~FunctionNetwork();
	
	std::vector<float> CalculateOutput(const std::vector<float>& input);
	void Mutate(float mutation_rate, float mutation_sigma);
};

#endif