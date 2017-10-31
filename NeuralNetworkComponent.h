/*
==============================================================================

NeuralNetworkComponent.h
Created: 16 Oct 2017 10:29:13pm
Author:  Owner

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// System includes
//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <time.h>
#include <algorithm>
#include <stdint.h>
#include <limits.h>
#include <stdexcept>

// OpenNN includes

#include "opennn/opennn.h"
#include <opennn/multilayer_perceptron.h>
#include <opennn/perceptron_layer.h>
#include "training_strategy.h"
//#include "tests/*.h"
#include "variables.h"
//#include "statistics.h"
using namespace OpenNN;
using namespace std;


//==============================================================================
/*
*/
class NeuralNetworkComponent : public Component,
	private Button::Listener,
	private Timer
{
public:
	NeuralNetworkComponent()
	{
		// In your constructor, you should add any child components, and
		// initialise any special settings that your component needs.
		trainNetworkButton.setButtonText("Train the Neural Network");
		addAndMakeVisible(trainNetworkButton);
		trainNetworkButton.addListener(this);
		message = "Initializing the Neural Network";
		setSize(200, 200);
	}

	~NeuralNetworkComponent()
	{
	}

	void paint(Graphics& g) override
	{
		/* This demo code just fills the component's background and
		draws some placeholder text to get you started.

		You should replace everything in this method with your own
		drawing code..
		*/

		g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

		g.setColour(Colours::grey);
		g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

		g.setColour(Colours::white);
		g.setFont(14.0f);
		/*g.drawText ("NeuralNetworkComponent", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text*/
	}

	void resized() override
	{
		Rectangle<int> brownies(getLocalBounds());
		int buttonHeight = 80;
		trainNetworkButton.setBounds(brownies.removeFromTop(buttonHeight).reduced(5));
	}

	String message;

	//dummy function
	void trainNeuralNetwork() {
		startTimer(500);
		if (getTimerInterval() >= 1000) {
			message = "Receiving Feature Vectors";
			stopTimer();
		}
		startTimer(500);
		if (getTimerInterval() >= 1000) {
			message = "Feedforward";
			stopTimer();
		}
		startTimer(500);
		if (getTimerInterval() > 1000) {
			message = "BackPropagation";
			stopTimer();
		}
		startTimer(500);
		if (getTimerInterval() > 1000) {
			message = "Done";
			stopTimer();
		}

		actuallyTrainNeuralNetwork();

	}

	int actuallyTrainNeuralNetwork() {
		try{
			std::cout << "OpenNN. MIXED DATASET" << std::endl;

			srand((unsigned int)time(NULL));

			/*DATA SET*/
			DataSet data_set;

			data_set.set_data_file_name("data/data.dat");

			data_set.set_separator("Comma");

			cout << "about to load data" << endl;

			data_set.load_data();

			cout << "loaded data" << endl;

			/*VARIABLES */

			Variables* variables_pointer = data_set.get_variables_pointer();

			variables_pointer->set_name(0, "Centroid");
			variables_pointer->set_use(0, Variables::Input);

			variables_pointer->set_name(1, "MFCC 0");
			variables_pointer->set_use(1, Variables::Input);

			variables_pointer->set_name(2, "MFCC 1");
			variables_pointer->set_use(2, Variables::Input);

			variables_pointer->set_name(3, "MFCC 3");
			variables_pointer->set_use(3, Variables::Input);

			variables_pointer->set_name(4, "MFCC 4");
			variables_pointer->set_use(4, Variables::Input);

			variables_pointer->set_name(5, "MFCC 6");
			variables_pointer->set_use(5, Variables::Input);

			variables_pointer->set_name(6, "MFCC 7");
			variables_pointer->set_use(6, Variables::Input);

			variables_pointer->set_name(7, "Peak Ratio Chroma F");
			variables_pointer->set_use(7, Variables::Input);

			variables_pointer->set_name(8, "Peak Ratio Avg Chroma A");
			variables_pointer->set_use(8, Variables::Input);

			variables_pointer->set_name(9, "Peak Ratio Minimum Chroma A");
			variables_pointer->set_use(9, Variables::Input);

			variables_pointer->set_name(10, "MFCC 7 2");
			variables_pointer->set_use(10, Variables::Input);

			variables_pointer->set_name(11, "MFCC 12");
			variables_pointer->set_use(11, Variables::Input);

			variables_pointer->set_name(12, "Peak Ratio Chroma F");
			variables_pointer->set_use(12, Variables::Input);

			variables_pointer->set_name(13, "Peak Ratio Minimum Chroma A 2"); //WTF
			variables_pointer->set_use(13, Variables::Input);

			variables_pointer->set_name(14, "MFCC 1 2");
			variables_pointer->set_use(14, Variables::Input);

			variables_pointer->set_name(15, "MFCC 5"); // WTF
			variables_pointer->set_use(15, Variables::Input);

			variables_pointer->set_name(16, "Peak Ratio Chroma C Sharp"); //WTF
			variables_pointer->set_use(16, Variables::Input);

			variables_pointer->set_name(17, "Peak Ratio Chroma D");
			variables_pointer->set_use(17, Variables::Input);

			variables_pointer->set_name(18, "Peak Ratio Min Chroma A 3");
			variables_pointer->set_use(18, Variables::Input); //WTF

			variables_pointer->set_name(19, "Zero Crossings"); //WTF
			variables_pointer->set_use(19, Variables::Input);

			variables_pointer->set_name(20, "MFCC 10"); // WTF
			variables_pointer->set_use(20, Variables::Input);

			variables_pointer->set_name(21, "Center");
			variables_pointer->set_use(21, Variables::Target);

			variables_pointer->set_name(22, "Halfedge");
			variables_pointer->set_use(22, Variables::Target);

			variables_pointer->set_name(23, "Rimshot");
			variables_pointer->set_use(23, Variables::Target);

			const Matrix<std::string> inputs_information = variables_pointer->arrange_inputs_information();
			const Matrix<std::string> targets_information = variables_pointer->arrange_targets_information();

			/*SCALING THE INPUT DATA FOR BETTER PERFORMANCE*/
			Instances* instances_pointer = data_set.get_instances_pointer();

			instances_pointer->split_random_indices();

			const Vector< Statistics<double> > inputs_statistics = data_set.scale_inputs_minimum_maximum();

			/*SCALING THE INPUT DATA FOR APPROXIMATION PROBLEMS*/
			const Vector< Statistics<double> > targets_statistics = data_set.scale_targets_minimum_maximum();

			/*OBTAIN AND SET INFO ABOUT THE VARIABLES OF THE DATA*/

			const size_t inputs_number = variables_pointer->count_inputs_number();//should give 20
			cout << "\n inputs number : " << inputs_number << endl;
			const size_t outputs_number = variables_pointer->count_targets_number(); //should give 3
			cout << "\n outputs number: " << outputs_number << endl;
			/*----------------------------------*/
			NeuralNetwork neural_network(inputs_number, 30, outputs_number);

			neural_network.construct_scaling_layer();

			ScalingLayer* scaling_layer_pointer = neural_network.get_scaling_layer_pointer();

			scaling_layer_pointer->set_statistics(inputs_statistics);

			scaling_layer_pointer->set_scaling_method(ScalingLayer::NoScaling);

			/*PROBABILISTIC LAYER - FOR PATTERN RECOGNITION & CLASSIFICATION NN'S*/
			neural_network.construct_probabilistic_layer();

			ProbabilisticLayer* probability_layer_pointer = neural_network.get_probabilistic_layer_pointer();

			probability_layer_pointer->set_probabilistic_method(ProbabilisticLayer::Probability);

			/*ERROR*/
			LossIndex loss_index(&neural_network, &data_set);//set both pointers at same time!
			loss_index.set_error_type("NORMALIZED_SQUARED_ERROR"); //not in the example.
			loss_index.set_regularization_type(LossIndex::NEURAL_PARAMETERS_NORM);

			/*TRAINING STRATEGY*/
			TrainingStrategy training_strategy(&loss_index);
			training_strategy.set_main_type(TrainingStrategy::QUASI_NEWTON_METHOD);
			QuasiNewtonMethod* quasi_Newton_method_pointer = training_strategy.get_quasi_Newton_method_pointer();
			quasi_Newton_method_pointer->set_maximum_iterations_number(30); //changed this.
			quasi_Newton_method_pointer->set_display_period(10);
			training_strategy.perform_training();

			TestingAnalysis testing_analysis(&neural_network, &data_set);
			Matrix<size_t> confusion = testing_analysis.calculate_confusion();

			scaling_layer_pointer->set_scaling_method(ScalingLayer::MinimumMaximum);

			neural_network.save_expression("expresion.txt");
			data_set.save("data/proceeds/data_set.xml");

			neural_network.save("data/proceeds/neural_network.xml");
			neural_network.save_expression("data/proceeds/expression.txt");
			confusion.save("data/proceeds/confusion.dat");

			training_strategy.save("data/proceeds/training_strategy.xml");

			return(0);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;

			return(1);
		}

	}

	TextButton* getButton() {
		return &trainNetworkButton;
	}

private:

	TextButton trainNetworkButton;

	void timerCallback() override
	{
		trainNetworkButton.setButtonText("Neural Network Training Starting");
	}

	void buttonClicked(Button* button) override
	{
		if (button == &trainNetworkButton)
		{
			startTimer(500);

		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuralNetworkComponent)
};
