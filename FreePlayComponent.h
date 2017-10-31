/*
==============================================================================

FreePlayComponent.h
Created: 16 Oct 2017 10:29:26pm
Author:  Owner

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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
class FreePlayComponent : public Component,
	private Button::Listener,
	private Timer
{
public:
	FreePlayComponent()
	{
		// In your constructor, you should add any child components, and
		// initialise any special settings that your component needs.
		freePlayButton.setButtonText("Free Play Mode");
		addAndMakeVisible(freePlayButton);
		freePlayButton.addListener(this);
		message = "Starting Free Play Mode";
		setSize(200, 200);
	}

	~FreePlayComponent()
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
		/*g.drawText ("FreePlayComponent", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text*/
	}

	void resized() override
	{
		// This method is where you should set the bounds of any child
		// components that your component contains..
		Rectangle<int> brownies(getLocalBounds());
		int buttonHeight = 80;
		freePlayButton.setBounds(brownies.removeFromTop(buttonHeight).reduced(5));
	}
	TextButton* getButton() {
		return &freePlayButton;
	}

	int classifyIncomingVectors() {

		try
		{
			bool debug = false;
			/*LOAD A PREVIOUSLY TRAINED NEURAL NETWORK !*/
			tinyxml2::XMLDocument myDoc;
			myDoc.LoadFile("data/proceeds/neural_network.xml");
			if (debug) { cout << "found xml file" << endl; }
			if (debug) { cout << "creating neural network from xml file" << endl; }
			NeuralNetwork neural_network(myDoc);
			if (debug) { cout << "created neural_network from xml file" << endl; }

			/*READ THE INPUT FOR AN INCOMING INSTANCE (UN LABELED) - MUST FORMAT CORRECTLY */
			Vector<double> vector_input;
			vector_input.load("data/input/single_instance.dat");
			if (debug) { cout << vector_input.to_string() << "\n" << endl; }

			/*READ THE INPUT FOR AN INCOMING MATRIX OF INSTANCES - MUST FORMAT CORRECTLY*/
			Matrix<double> matrix_input;
			matrix_input.load("data/input/some_instances.dat");
			if (debug) { cout << "matrix input: " << matrix_input.to_string() << "\n" << endl; }

			/*COMPARING THE SIZE OF ATTRIBUTES FOR AN INSTANCE ,
			AND THE NUMBER OF ATTRIBUTES THE NEURAL NETWORK WAS TRAINED ON
			THESE SHOULD MATCH!!!*/
			if (debug) { cout << "vector_input size :" << vector_input.size() << endl; }
			if (debug) { cout << "neural_network inputs number:" << neural_network.get_inputs_number() << endl; }

			/*CALCULATE THE ACTIVATION OF THE LAST LEVEL OF THE NEURAL NETWORK
			SINGLE INSTANCE*/
			Vector<double> output_vector = neural_network.calculate_outputs(vector_input);
			/*GET THE INDEX OF THE MAXIMUM ACTIVATION == RESULTING CLASSIFICATION !!!*/
			size_t myClassificationIndex = output_vector.calculate_maximal_index();

			/*CALCULATE THE ACTIVATION OF THE LAST LEVEL OF THE NEURAL NETWORK
			MULTIPLE INSTANCES AT ONCE*/
			Matrix<double> output_matrix = neural_network.calculate_output_data(matrix_input);
			cout << "output matrix number of rows" << output_matrix.get_rows_number() << endl;
			Vector<double> maxIndices;
			for (size_t i = 0; i < output_matrix.get_rows_number(); i++) {
				maxIndices.emplace_back(output_matrix.arrange_row(i).calculate_maximal_index());//getcalculate_maximal_index();
			}

			/*SHOW RESULTS - FOR SINGLE INPUT INSTANCE - WHAT IS THE RESULTING CLASSIFICATION?*/
			if (debug) {
				cout << "\n SHOW RESULTS - FOR MULTIPLE INPUT INSTANCES - WHAT ARE THE RESULTING OUTPUT FROM NN? \n" << endl;
				cout << "\n neural_network result for vector_input: " << output_vector.to_string() << "\n\n" << endl;
			}
			cout << "\n ------------Resulting classifications--------- \n" << endl;
			cout << "\n For vector_input: " << myClassificationIndex << "\n" << endl;

			/*SHOW RESULTS - FOR MULTIPLE INPUT INSTANCES - WHAT ARE THE RESULTING CLASSIFICATIONS?*/
			//if (debug) {
			cout << "\n SHOW RESULTS - FOR MULTIPLE INPUT INSTANCES - WHAT ARE THE RESULTING OUTPUT FROM NN? \n" << endl;
			cout << "\n neural_network result for matrix_input:\n " << output_matrix.to_string() << "\n\n" << endl;
			//}
			cout << "\n For matrix_input: " << maxIndices.to_string() << "\n" << endl;

			
			output_vector.save("data/classified_vectors/single_instance.dat");
			maxIndices.save("data/classified_vectors/multiple_instances.dat");


			cin.get();
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return(1);
		}
	}

private:
	TextButton freePlayButton;
	String message;
	void timerCallback() override
	{
		freePlayButton.setButtonText("Free Play Mode Starting");
	}

	void buttonClicked(Button* button) override
	{
		if (button == &freePlayButton)
		{
			startTimer(500);

		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreePlayComponent)
};
