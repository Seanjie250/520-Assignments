#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <limits>

struct DataPoint {
    double x, y;
    double predicted;
    
    DataPoint(double x = 0, double y = 0) : x(x), y(y), predicted(0) {}
};

class LinearRegression {
private:
    std::vector<DataPoint> data;
    double slope;
    double intercept;
    double rSquared;

public:
    LinearRegression() : slope(0), intercept(0), rSquared(0) {}
    
    void addPoint(double x, double y) {
        data.push_back(DataPoint(x, y));
    }
    
    void generateSyntheticData(int numPoints, double trueSlope = 2.5,
                               double trueIntercept = 1.0, double noise = 0.5) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> xDis(0, 10);
        std::normal_distribution<> noiseDis(0, noise);
        
        for (int i = 0; i < numPoints; i++) {
            double x = xDis(gen);
            double y = trueSlope * x + trueIntercept + noiseDis(gen);
            addPoint(x, y);
        }
        
        std::cout << "Generated " << numPoints << " synthetic data points.\n";
        std::cout << "True model: y = " << trueSlope << "x + " << trueIntercept << "\n";
        std::cout << "Noise level: " << noise << "\n\n";
    }
    
    void fit() {
        if (data.empty()) {
            std::cerr << "Error: No data points to fit." << std::endl;
            return;
        }
        
        double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
        int n = data.size();
        
        for (const auto& point : data) {
            sumX += point.x;
            sumY += point.y;
            sumXY += point.x * point.y;
            sumX2 += point.x * point.x;
        }
        
        double meanX = sumX / n;
        double meanY = sumY / n;
        
        slope = (sumXY - n * meanX * meanY) / (sumX2 - n * meanX * meanX);
        intercept = meanY - slope * meanX;
        
        calculateMetrics();
        
        std::cout << "Model: y = " << slope << "x + " << intercept << std::endl;
    }
    
    void calculateMetrics() {
        if (data.empty()) return;
        
        double meanY = 0;
        for (const auto& point : data) {
            meanY += point.y;
        }
        meanY /= data.size();
        
        double totalSS = 0;
        double residualSS = 0;
        
        for (auto& point : data) {
            point.predicted = slope * point.x + intercept;
            double diff = point.y - point.predicted;
            residualSS += diff * diff;
            double meanDiff = point.y - meanY;
            totalSS += meanDiff * meanDiff;
        }
        
        rSquared = 1.0 - (residualSS / totalSS);
    }
    
    double calculateMSE() {
        if (data.empty()) return 0;
        
        double sumSquaredError = 0;
        for (const auto& point : data) {
            double error = point.y - point.predicted;
            sumSquaredError += error * error;
        }
        return sumSquaredError / data.size();
    }
    
    double calculateMAE() {
        if (data.empty()) return 0;
        
        double sumAbsError = 0;
        for (const auto& point : data) {
            sumAbsError += std::abs(point.y - point.predicted);
        }
        return sumAbsError / data.size();
    }
    
    double predict(double x) {
        return slope * x + intercept;
    }
    
    void saveAsImage(const std::string& filename) {
        if (data.empty()) {
            std::cerr << "Error: No data to visualize." << std::endl;
            return;
        }
        
        const int width = 800;
        const int height = 800;
        const double margin = 50;
        
        double minX = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::lowest();
        
        for (const auto& point : data) {
            minX = std::min(minX, point.x);
            maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y);
            maxY = std::max(maxY, point.y);
        }
        
        double rangeX = maxX - minX;
        double rangeY = maxY - minY;
        double scaleX = (width - 2 * margin) / rangeX;
        double scaleY = (height - 2 * margin) / rangeY;
        
        std::vector<std::vector<int>> r(width, std::vector<int>(height, 255));
        std::vector<std::vector<int>> g(width, std::vector<int>(height, 255));
        std::vector<std::vector<int>> b(width, std::vector<int>(height, 255));
        
        for (const auto& point : data) {
            int px = static_cast<int>((point.x - minX) * scaleX + margin);
            int py = static_cast<int>((point.y - minY) * scaleY + margin);
            py = height - py;
            
            if (px >= 0 && px < width && py >= 0 && py < height) {
                for (int dx = -3; dx <= 3; dx++) {
                    for (int dy = -3; dy <= 3; dy++) {
                        int nx = px + dx;
                        int ny = py + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            if (dx * dx + dy * dy <= 9) {
                                r[nx][ny] = 0;
                                g[nx][ny] = 0;
                                b[nx][ny] = 255;
                            }
                        }
                    }
                }
            }
        }
        
        double y1 = predict(minX);
        double y2 = predict(maxX);
        int px1 = static_cast<int>((minX - minX) * scaleX + margin);
        int py1 = static_cast<int>((y1 - minY) * scaleY + margin);
        py1 = height - py1;
        int px2 = static_cast<int>((maxX - minX) * scaleX + margin);
        int py2 = static_cast<int>((y2 - minY) * scaleY + margin);
        py2 = height - py2;
        
        int dx = px2 - px1;
        int dy = py2 - py1;
        int steps = std::max(std::abs(dx), std::abs(dy));
        
        for (int i = 0; i <= steps; i++) {
            int px = px1 + (dx * i) / steps;
            int py = py1 + (dy * i) / steps;
            
            if (px >= 0 && px < width && py >= 0 && py < height) {
                for (int d = -2; d <= 2; d++) {
                    int nx = px + d;
                    int ny = py + d;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        r[nx][ny] = 255;
                        g[nx][ny] = 0;
                        b[nx][ny] = 0;
                    }
                }
            }
        }
        
        std::ofstream file(filename);
        file << "P3\n" << width << " " << height << "\n255\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file << r[x][y] << " " << g[x][y] << " " << b[x][y] << " ";
            }
            file << "\n";
        }
        file.close();
        std::cout << "Saved visualization to " << filename << std::endl;
    }
    
    double getRSquared() const { return rSquared; }
    double getSlope() const { return slope; }
    double getIntercept() const { return intercept; }
};

int main() {
    LinearRegression lr;
    lr.generateSyntheticData(100);
    
    lr.fit();
    
    double mse = lr.calculateMSE();
    double mae = lr.calculateMAE();
    double r2 = lr.getRSquared();
    
    std::cout << "MSE: " << mse << std::endl;
    std::cout << "MAE: " << mae << std::endl;
    std::cout << "R²: " << r2 << std::endl;
    
    lr.saveAsImage("regression_output.ppm");
    
    return 0;
}
