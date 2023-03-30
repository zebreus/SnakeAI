#include "apple.h"

#include "game_settings.h"

Apple::Apple() : Cell(rand() % FIELD_SIZE, rand() % FIELD_SIZE)
{

}

Apple::Apple(Cell cell) : Cell(cell)
{

}

void Apple::Draw(Field& field) const
{
    field[x][y].color = Apple::DEFAULT_COLOR;
    field[x][y].state = RenderableCell::State::Apple;
}