/*
 * moving_average_filter.h
 *
 *  Created on: 09.06.2011
 *      Author: bouwerob
 */

#ifndef MOVING_AVERAGE_FILTER_H_
#define MOVING_AVERAGE_FILTER_H_

/**
 * Introduction
 * The moving average filter is a simple Low Pass FIR (Finite Impulse Response) filter
 * commonly used for smoothening an array of sampled data
 */

#define max_elements	8		// samples in the buffer
#define shiftSampleVal	 4		// number of bits to shift to obtain the average value

/**
 * data structure for the moving average filter
 */
typedef struct maf
{
	uint32_t	index;
	uint32_t	samples[max_elements];
	uint32_t	maf_value;
} maf_t;

/**
 * init the filter channel
 */
static void initFilterChannel(maf_t *filterChannel)
{
	uint32_t i;

	filterChannel->index = 0;
	filterChannel->maf_value = 0;
	for (i = 0; i < max_elements; i++)
	{
		filterChannel->samples[i] = 0;
	}
}

/**
 * add a new sample
 * - subtracts the oldest sample
 * - adds the new sample
 */
static void addSample(maf_t *filterChannel, uint32_t newSample)
{
	filterChannel->maf_value -= filterChannel->samples[filterChannel->index]; // subtract oldest value
	filterChannel->maf_value += newSample; // add new sample
	filterChannel->samples[filterChannel->index] = newSample; // save the sample
	filterChannel->index++;
	if (filterChannel->index >= max_elements)
		filterChannel->index = 0; // reset index
}

/**
 * get the average for the filter channel
 * (numerical instability ... ?)
 * the elements in buffer: 2^n - thus a bitshift is the most economical way
 */
static uint32_t getMovingAverage(maf_t *filterChannel)
{
	float average = filterChannel->maf_value;
	//average = average >> shiftSampleVal;
	average = average / max_elements;
	return (uint32_t) average + 0.5f;
}

#endif /* MOVING_AVERAGE_FILTER_H_ */