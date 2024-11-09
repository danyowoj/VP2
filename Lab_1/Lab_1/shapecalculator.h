#ifndef SHAPECALCULATOR_H
#define SHAPECALCULATOR_H


class ShapeCalculator
{
public:
    static double calculateSquareArea(double side);
    static double calculateRectangleArea(double width, double height);
    static double calculateParallelogramArea(double base, double height);
    static double calculateRhombusArea(double diagonal1, double diagonal2);
    static double calculateTriangleArea(double base, double height);
    static double calculateTrapezoidArea(double base1, double base2, double height);
    static double calculateCircleArea(double radius);
    static double calculateSectorArea(double radius, double angle);
};

#endif // SHAPECALCULATOR_H
