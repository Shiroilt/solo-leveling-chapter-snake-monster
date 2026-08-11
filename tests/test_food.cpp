#include <iostream>
#include <cassert>
#include "../snake0/Food.h"

void test_food_initially_inactive() {
    Food food;

    assert(food.isActive() == false);

    std::cout << "PASS: Food is initially inactive\\n";
}

void test_food_activation() {
    Food food;

    food.setPosition({5, 5});

    assert(food.isActive() == true);

    std::cout << "PASS: Food becomes active after setting position\\n";
}

void test_food_position() {
    Food food;

    Point p = {8, 12};
    food.setPosition(p);

    Point foodPos = food.getPosition();

    assert(foodPos.x == 8);
    assert(foodPos.y == 12);

    std::cout << "PASS: Food position is stored correctly\\n";
}

void test_food_deactivation() {
    Food food;

    food.setPosition({3, 4});
    food.deactivate();

    assert(food.isActive() == false);

    std::cout << "PASS: Food deactivates correctly\\n";
}

int main() {
    std::cout << "Running Food Tests...\\n\\n";

    test_food_initially_inactive();
    test_food_activation();
    test_food_position();
    test_food_deactivation();

    std::cout << "\\nAll food tests passed successfully!\\n";

    return 0;
}
