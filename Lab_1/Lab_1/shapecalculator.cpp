#include "shapecalculator.h"

double ShapeCalculator::calculateSquareArea(double a) {
    return a * a;
}

double ShapeCalculator::calculateRectangleArea(double a, double b) {
    return a * b;
}

double ShapeCalculator::calculateParallelogramArea(double a, double h) {
    return a * h;
}

double ShapeCalculator::calculateRhombusArea(double a, double b) {
    return 0.5 * a * b;
}

double ShapeCalculator::calculateTriangleArea(double a, double h) {
    return 0.5 * a * h;
}

double ShapeCalculator::calculateTrapezoidArea(double a, double b, double h) {
    return (a + b) * h * 0.5;
}

double ShapeCalculator::calculateCircleArea(double r) {
    return 3.14159265359 * r * r;
}

double ShapeCalculator::calculateSectorArea(double r, double alfa) {
    return 3.14159265359 * r * r * (alfa / 360);
}
