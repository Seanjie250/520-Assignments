# HW5: Supervised and Unsupervised Machine Learning with C++

This assignment implements two fundamental machine learning algorithms:
1. **K-Means Clustering** (Unsupervised Learning)
2. **Linear Regression** (Supervised Learning)

## Files

- `kmeans.cpp` - K-Means clustering implementation
- `regression.cpp` - Linear regression implementation
- `Makefile` - Build configuration
- `README.md` - This file

## Building

```bash
make
```

This builds:
- `kmeans` - K-Means clustering executable
- `regression` - Linear regression executable

## Running

### K-Means Clustering

```bash
./kmeans
```

This will:
1. Generate 300 synthetic 2D data points in 3 clusters
2. Run K-Means clustering algorithm
3. Print iteration information and inertia
4. Save visualization to `kmeans_output.ppm`

**Output:**
- Console: Iteration progress and convergence status
- File: `kmeans_output.ppm` - Visualization with colored clusters and centers

### Linear Regression

```bash
./regression
```

This will:
1. Generate 100 synthetic data points with noise
2. Fit a linear regression model
3. Calculate and print metrics (MSE, MAE, R²)
4. Save visualization to `regression_output.ppm`

**Output:**
- Console: Model equation, MSE, MAE, R² score
- File: `regression_output.ppm` - Visualization with data points and regression line

## Viewing PPM Images

PPM images can be viewed with:
- Image viewers that support PPM format
- Online PPM viewers
- Convert to PNG using ImageMagick: `convert output.ppm output.png`

## K-Means Implementation

### Required Functions

- `distance(p1, p2)` - Euclidean distance between two points
- `initializeCenters()` - Randomly select K initial centers
- `assignPoints()` - Assign each point to nearest center
- `updateCenters()` - Recalculate centers, return convergence status
- `fit()` - Main algorithm loop with iteration output
- `calculateInertia()` - Sum of squared distances to centers
- `saveAsImage(filename)` - Generate PPM visualization

### Algorithm Steps

1. **Initialize**: Randomly select K points as cluster centers
2. **Assign**: Assign each point to the nearest center
3. **Update**: Recalculate centers as mean of assigned points
4. **Converge**: Repeat until centers stabilize or max iterations

## Linear Regression Implementation

### Required Functions

- `fit()` - Fit regression line using Least Squares
- `calculateMetrics()` - Calculate predictions and R²
- `calculateMSE()` - Mean Squared Error
- `calculateMAE()` - Mean Absolute Error
- `predict(x)` - Predict y value for given x
- `saveAsImage(filename)` - Generate PPM visualization

### Model

Fits a line: **y = mx + b**

Where:
- `m` = slope (computed via Least Squares)
- `b` = intercept (computed via Least Squares)

### Metrics

- **MSE** (Mean Squared Error): Average squared prediction error
- **MAE** (Mean Absolute Error): Average absolute prediction error
- **R²** (Coefficient of Determination): Proportion of variance explained

## Example Output

### K-Means
```
Iteration 1: Inertia = 1234.56
Iteration 2: Inertia = 567.89
Iteration 3: Inertia = 234.12
Converged after 3 iterations.
Saved visualization to kmeans_output.ppm
```

### Linear Regression
```
Generated 100 synthetic data points.
True model: y = 2.5x + 1
Noise level: 0.5

Model: y = 2.487x + 1.023
MSE: 0.234
MAE: 0.412
R²: 0.987
Saved visualization to regression_output.ppm
```

## Clean

```bash
make clean
```

Removes executables and generated PPM files.

## Requirements Met

✅ K-Means: All 7 required functions implemented  
✅ Linear Regression: All 6 required functions implemented  
✅ PPM image visualization for both algorithms  
✅ Synthetic data generation  
✅ Comprehensive output and metrics  
