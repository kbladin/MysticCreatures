#ifndef FUNCTIONNETWORK_H
#define FUNCTIONNETWORK_H

#include <vector>
#include <random>

class FunctionNetwork
{
private:
	const int input_size_;
	const int hidden_layer_size_;
	const int output_size_;
	static std::default_random_engine generator_;
	
	float* input_weights_;
	float* output_weights_;

	float UnipolarSigmoidal(float x);

public:
	FunctionNetwork(
		int input_size,
		int hidden_layer_size,
		int output_size);
	FunctionNetwork(const FunctionNetwork& f);
	~FunctionNetwork();
	
	std::vector<float> CalculateOutput(const std::vector<float>& input);
	void Mutate(float mutation_rate);
};

#endif