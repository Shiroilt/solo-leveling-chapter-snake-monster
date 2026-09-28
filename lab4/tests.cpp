#include <cassert>
#include <iostream>
#include <vector>
#include "../snake0/Snake.h"
#include "../snake0/Food.h"

static void rule1_snake_moves_one_cell() {
    Snake snake(10, 10, 3);
    Food food;
    Point before = snake.getHead();
    snake.move(food);
    Point after = snake.getHead();
    assert(after.xCoord == before.xCoord + 1);
    assert(after.yCoord == before.yCoord);

    // These directions execute the movement branches for coverage, but the
    // Part C suite intentionally does not assert their exact positions.
    snake.changeDirection(DIR_UP);
    snake.move(food);
    snake.changeDirection(DIR_LEFT);
    snake.move(food);
    snake.changeDirection(DIR_DOWN);
    snake.move(food);
}

static void rule2_opposite_direction_is_ignored() {
    Snake snake(10, 10, 3);
    snake.changeDirection(DIR_LEFT);
    assert(snake.getDirection() == DIR_RIGHT);
    snake.changeDirection(DIR_UP);
    assert(snake.getDirection() == DIR_UP);
    snake.changeDirection(DIR_DOWN);
    assert(snake.getDirection() == DIR_UP);
}

static void rule3_food_spawn_avoids_snake() {
    Food food;
    std::vector<Point> body = {Point(5,5), Point(5,6), Point(5,7)};
    food.spawn(body, 20, 20);
    assert(food.isActive());
    for (const Point& segment : body) {
        assert(!(food.getPosition() == segment));
    }
}

static void rule4_boundary_collision_is_detected() {
    Snake right(18, 10, 3);
    Food food;
    right.move(food);
    assert(right.checkBoundaryCollision(20, 20));

    Snake middle(10, 10, 3);
    assert(!middle.checkBoundaryCollision(20, 20));
}

static void coverage_only_food_deactivation() {
    Food food;
    food.spawn({}, 20, 20);
    food.deactivate();
    // Deliberately no assertion: Part C asks us to identify a behavior
    // executed by the suite but not checked by an assertion.
}

// Part D: deterministic fake collaborator through the object seam.
class FakeFood : public FoodPort {
public:
    Point position;
    bool active;
    bool deactivateCalled = false;

    explicit FakeFood(Point p) : position(p), active(true) {}
    Point getPosition() override { return position; }
    bool isActive() override { return active; }
    void deactivate() override {
        deactivateCalled = true;
        active = false;
    }
};

static void partD_snake_grows_when_food_is_eaten() {
    Snake snake(10, 10, 3);
    Point target(11, 10);
    FakeFood food(target);

    int before = snake.getLength();
    bool eaten = snake.move(food);

    assert(eaten);
    assert(snake.getLength() == before + 1);
    assert(!food.isActive());
    assert(food.deactivateCalled);
}

int main() {
    rule1_snake_moves_one_cell();
    rule2_opposite_direction_is_ignored();
    rule3_food_spawn_avoids_snake();
    rule4_boundary_collision_is_detected();
    coverage_only_food_deactivation();
    partD_snake_grows_when_food_is_eaten();
    std::cout << "All Lab 4 tests passed.\n";
    return 0;
}
