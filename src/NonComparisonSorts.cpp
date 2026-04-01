#include "NonComparisonSorts.h"
#include <iostream>
#include <algorithm>

// ---------------------------------------------------------------------------
// Counting Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #1 -- Steps 1a, 1b, 1c (Count Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #2 -- Step 2 (Placement Phase)
//
void counting_sort(std::vector<int>& data) {
	if (data.size() <= 1) return;
    // TODO: Implement counting sort
    //   1. Find the min and max values
	int min_val = *std::min_element(data.begin(), data.end());
	int max_val = *std::max_element(data.begin(), data.end());

	// Step 1b. Create count array of size (max - min + 1) initialized to 0
	int k = max_val - min_val + 1; // Range of values
	std::vector<int> count(k, 0);

	// Step 1c. Count occurrences of each value
    for (int num : data) {
        count[num - min_val]++;
	}
   
 
	// Step 2: Place elements back into data based on counts
	int inx = 0; // Index for the original data vector
	for (int i = 0; i < k; ++i) {
        while (count[i] > 0) {
            data[inx++] = i + min_val; // Place the value back into data
            count[i]--;
        }
    }


}

// ---------------------------------------------------------------------------
// Bucket Sort
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #3 -- Steps 1a, 1b (Distribution Phase)
// ? SEE DIAGRAM: cpp_diagrams.md #4 -- Steps 2-3 (Sort + Concatenate)
//
void bucket_sort(std::vector<int>& data, int num_buckets) {
	if (data.size() <= 1) return; 

    // TODO: Implement bucket sort
    //   1. Find the min and max values to determine bucket ranges
	// 1a. Find min and max
	int min_val = *std::min_element(data.begin(), data.end());
	int max_val = *std::max_element(data.begin(), data.end());

	// escape early if all values are the same (already sorted)
	if (min_val == max_val) return;

	int range = max_val - min_val + 1; // Total range of values
	int range_per_bucket = static_cast<int>(std::ceil(static_cast<double>(range) / num_buckets)); // Range each bucket will cover

    //   1b. Create num_buckets empty buckets (vectors)
	std::vector<std::vector<int>> buckets(num_buckets);

	for (int num : data) {
        int bucket_index = (num - min_val) / range_per_bucket; // Determine which bucket to place the number in
        if (bucket_index >= num_buckets) bucket_index = num_buckets - 1; // Handle edge case for max_val
		buckets[bucket_index].push_back(num); // Place the number in the appropriate bucket
    }

	// step 2: Sort each bucket and concatenate back into data
	int inx = 0; // Index for the original data vector
	for (auto& bucket : buckets) {
        std::sort(bucket.begin(), bucket.end()); // Sort the bucket (can also use insertion sort for small buckets)

		// Step 3: Concatenate sorted buckets back into data
        for (int num : bucket) {
            data[inx++] = num; // Place sorted numbers back into data
        }
    }

}

// ---------------------------------------------------------------------------
// Radix Sort (LSD)
// ---------------------------------------------------------------------------
//
// ? SEE DIAGRAM: cpp_diagrams.md #5 -- Step 0 + Pass 1 Detail
// ? SEE DIAGRAM: cpp_diagrams.md #6 -- Passes 2-3 (Stability in Action)
//
static void counting_sort_by_digits(std::vector<int>& data, int exp) {
	int n = static_cast<int>(data.size()); // number of elements in data

    std::vector<int> output(n); // Output array to hold sorted order for this digit
	int count[10] = { 0 }; // k = 10 for digits 0-9

	// Count occurrences of each digit in the current position; needed for stability, ensures numbers with the same digit maintain their relative order from previous passes
    for (int i = 0; i < n; i++) {
        int digit = (data[i] / exp) % 10;
        count[digit]++;
    }
    // Update count array to hold cumulative counts
    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }
    // Build the output array by placing elements in the correct position
    for (int i = n-1; i >= 0; i--) {
        int digit = (data[i] / exp) % 10;
        output[count[digit] - 1] = data[i];
		count[digit]--;
    }
    // Copy the output array back to data for the next pass
    data = output;
}

void radix_sort(std::vector<int>& data) {
    if (data.size() <= 1) return; // data is already sorted


	int max_val = *std::max_element(data.begin(), data.end()); // Find the maximum number to know the number of digits

    //   3. After all digit passes, data is sorted
	for (int exp = 1; max_val / exp > 0; exp *= 10) { // exp *= 10 to move to the next digit (1s, 10s, 100s, etc.)
        counting_sort_by_digits(data, exp); // Sort data based on the current digit
	}
}

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------
void print_vector(const std::vector<int>& data, const std::string& label) {
    if (!label.empty()) std::cout << label << ": ";
    std::cout << "[";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data[i];
        if (i + 1 < data.size()) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}
