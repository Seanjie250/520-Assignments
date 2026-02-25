#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <limits>

struct Point {
    double x, y;
    int cluster;
    
    Point(double x = 0, double y = 0) : x(x), y(y), cluster(-1) {}
};

class KMeans {
private:
    std::vector<Point> points;
    std::vector<Point> centers;
    int k;
    double tolerance;
    int maxIterations;
    int iterations;

public:
    KMeans(int k, double tolerance = 1e-4, int maxIterations = 100)
        : k(k), tolerance(tolerance), maxIterations(maxIterations), iterations(0) {
    }
    
    void addPoint(double x, double y) {
        points.push_back(Point(x, y));
    }
    
    void generateSyntheticData(int numPoints) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dis(0, 1);
        
        std::vector<std::pair<double, double>> clusterCenters = {
            {2, 2}, {8, 8}, {8, 2}
        };
        
        int pointsPerCluster = numPoints / k;
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < pointsPerCluster; j++) {
                double x = clusterCenters[i % clusterCenters.size()].first + dis(gen) * 0.5;
                double y = clusterCenters[i % clusterCenters.size()].second + dis(gen) * 0.5;
                addPoint(x, y);
            }
        }
    }
    
    double distance(const Point& p1, const Point& p2) {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    void initializeCenters() {
        centers.clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, points.size() - 1);
        
        std::vector<bool> used(points.size(), false);
        for (int i = 0; i < k; i++) {
            int idx;
            do {
                idx = dis(gen);
            } while (used[idx]);
            used[idx] = true;
            centers.push_back(points[idx]);
            centers[i].cluster = i;
        }
    }
    
    void assignPoints() {
        for (auto& point : points) {
            double minDist = std::numeric_limits<double>::max();
            int nearestCluster = -1;
            
            for (size_t i = 0; i < centers.size(); i++) {
                double dist = distance(point, centers[i]);
                if (dist < minDist) {
                    minDist = dist;
                    nearestCluster = i;
                }
            }
            point.cluster = nearestCluster;
        }
    }
    
    bool updateCenters() {
        bool converged = true;
        std::vector<Point> newCenters(k);
        std::vector<int> counts(k, 0);
        
        for (int i = 0; i < k; i++) {
            newCenters[i] = Point(0, 0);
        }
        
        for (const auto& point : points) {
            if (point.cluster >= 0 && point.cluster < k) {
                newCenters[point.cluster].x += point.x;
                newCenters[point.cluster].y += point.y;
                counts[point.cluster]++;
            }
        }
        
        for (int i = 0; i < k; i++) {
            if (counts[i] > 0) {
                newCenters[i].x /= counts[i];
                newCenters[i].y /= counts[i];
            } else {
                newCenters[i] = centers[i];
            }
            
            double dist = distance(newCenters[i], centers[i]);
            if (dist > tolerance) {
                converged = false;
            }
        }
        
        centers = newCenters;
        return converged;
    }
    
    double calculateInertia() {
        double inertia = 0.0;
        for (const auto& point : points) {
            if (point.cluster >= 0 && point.cluster < k) {
                double dist = distance(point, centers[point.cluster]);
                inertia += dist * dist;
            }
        }
        return inertia;
    }
    
    void fit() {
        initializeCenters();
        
        for (iterations = 0; iterations < maxIterations; iterations++) {
            assignPoints();
            bool converged = updateCenters();
            
            double inertia = calculateInertia();
            std::cout << "Iteration " << iterations + 1 << ": Inertia = " << inertia << std::endl;
            
            if (converged) {
                std::cout << "Converged after " << iterations + 1 << " iterations." << std::endl;
                return;
            }
        }
        
        std::cout << "Reached maximum iterations (" << maxIterations << ")." << std::endl;
    }
    
    void saveAsImage(const std::string& filename) {
        const int width = 800;
        const int height = 800;
        const double margin = 50;
        
        double minX = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::lowest();
        
        for (const auto& point : points) {
            minX = std::min(minX, point.x);
            maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y);
            maxY = std::max(maxY, point.y);
        }
        
        double rangeX = maxX - minX;
        double rangeY = maxY - minY;
        double scale = std::min((width - 2 * margin) / rangeX, (height - 2 * margin) / rangeY);
        
        std::vector<std::vector<int>> r(width, std::vector<int>(height, 255));
        std::vector<std::vector<int>> g(width, std::vector<int>(height, 255));
        std::vector<std::vector<int>> b(width, std::vector<int>(height, 255));
        
        std::vector<std::vector<int>> colors = {
            {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
            {255, 255, 0}, {255, 0, 255}, {0, 255, 255},
            {128, 0, 128}, {255, 165, 0}, {128, 128, 128}
        };
        
        for (const auto& point : points) {
            int px = static_cast<int>((point.x - minX) * scale + margin);
            int py = static_cast<int>((point.y - minY) * scale + margin);
            py = height - py;
            
            if (px >= 0 && px < width && py >= 0 && py < height) {
                int colorIdx = point.cluster % colors.size();
                for (int dx = -2; dx <= 2; dx++) {
                    for (int dy = -2; dy <= 2; dy++) {
                        int nx = px + dx;
                        int ny = py + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            r[nx][ny] = colors[colorIdx][0];
                            g[nx][ny] = colors[colorIdx][1];
                            b[nx][ny] = colors[colorIdx][2];
                        }
                    }
                }
            }
        }
        
        for (size_t i = 0; i < centers.size(); i++) {
            int px = static_cast<int>((centers[i].x - minX) * scale + margin);
            int py = static_cast<int>((centers[i].y - minY) * scale + margin);
            py = height - py;
            
            if (px >= 0 && px < width && py >= 0 && py < height) {
                for (int dx = -5; dx <= 5; dx++) {
                    for (int dy = -5; dy <= 5; dy++) {
                        int nx = px + dx;
                        int ny = py + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            if (std::abs(dx) == std::abs(dy) || dx == 0 || dy == 0) {
                                r[nx][ny] = 0;
                                g[nx][ny] = 0;
                                b[nx][ny] = 0;
                            }
                        }
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
};

int main() {
    KMeans kmeans(3, 1e-4, 100);
    kmeans.generateSyntheticData(300);
    kmeans.fit();
    kmeans.saveAsImage("kmeans_output.ppm");
    return 0;
}
