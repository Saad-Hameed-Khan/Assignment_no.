/*****************************************************************************
 *                                                                           *
 * C++ implementation of a few helpers for std::vectors                      *
 *                                                                           *
 * This software is (c) 2022 by                                              *
 * Jens Schneider,                                                           *
 * Division of Information and Computing Technology,                         *
 * College of Science and Engineering,                                       *
 * Hamad Bin Khalifa University,                                             *
 * Qatar Foundation, Doha Qatar                                              *
 *                                                                           *
 * It is distributed under the CC BY-NC-ND 4.0 licence:                      *
 * https://creativecommons.org/licenses/by-nc-nd/4.0/legalcode               *
 * For use in commercial projects, contact mailto:jeschneider@hbku.edu.qa.   *
 *                                                                           *
 * If you use this software for academic projects,                           *
 * an acknowledgement will be appreciated.                                   *
 *                                                                           *
 *                                                  last update: 16/Jan/2022 *
 *****************************************************************************/
#pragma once
#include<chrono>
#include<random>
#include<vector>
#include<iostream>

 // returns a random permutation of the vector
template<class T>
void perfect_shuffle(std::vector<T>& array,
	uint64_t seed = std::chrono::high_resolution_clock::now().time_since_epoch().count())
{
	std::mt19937_64 generator(seed);
	for (size_t n = 0; n < array.size() - 1; n++) {
		std::uniform_int_distribution<size_t> rnd(n, array.size() - 1);
		size_t k = rnd(generator);
		std::swap(array[n], array[k]);
	}
}

// outputs the vector to into an output stream (such as std::cout)
template<class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& array) {
	if (array.empty()) stream << "[]";
	stream << "[ ";
	for (const auto& elem : array) stream << elem << " ";
	return (stream << "]");
}#pragma once
