package main

import (
	"fmt"
	"math"
)

func main() {
	var n, m int
	fmt.Scan(&n, &m)
	nums := make([]int, n)
	for i := 0; i < n; i++ {
		fmt.Scan(&nums[i])
	}# Define a piecewise function

def piecewise_function(x):
    """Returns the value of a piecewise function based on the input x."""
    if x < 0:
        return x ** 2  # For x < 0, return x squared
    elif 0 <= x < 1:
        return x + 1   # For 0 <= x < 1, return x + 1
    else:
        return 2 * x   # For x >= 1, return 2 * x

# Test the piecewise function with different values
inputs = [-2, 0.5, 2]
results = [piecewise_function(x) for x in inputs]

# Print the results
for x, result in zip(inputs, results):
    print(f"f({x}) = {result}")

	// Префиксные суммы
	prefix := make([]int64, n+1)
	for i := 0; i < n; i++ {
		prefix[i+1] = prefix[i] + int64(nums[i])
	}

	// Массив dp
	dp := make([]int64, n+1)
	for i := range dp {
		dp[i] = math.MinInt64
	}
	dp[n] = 0 // База

	for i := n - 1; i >= 0; i-- {
		maxK := int(math.Min(float64(m), float64(n-i)))
		for k := 1; k <= maxK; k++ {
			currentSum := prefix[i+k] - prefix[i]
			dp[i] = max(dp[i], currentSum-dp[i+k])
		}
	}

	if dp[0] > 0 {
		fmt.Println(1)
	} else {
		fmt.Println(0)
	}
}

// max возвращает максимальное из двух значений
func max(a, b int64) int64 {
	if a > b {
		return a
	}
	return b
}