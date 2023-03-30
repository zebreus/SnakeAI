#include "snake.h"

Snake::Snake(Cell head) : Genome({ 16 + FEEDBACK_SIZE, (16 + FEEDBACK_SIZE) * 2, (16 + FEEDBACK_SIZE) * 3, (16 + FEEDBACK_SIZE) * 1, 1 + FEEDBACK_SIZE }), cells { { head } }
// Snake::Snake(Cell head) : Genome({ 16 + FEEDBACK_SIZE + FIELD_SIZE*FIELD_SIZE, (16 + FEEDBACK_SIZE + FIELD_SIZE), (16 + FEEDBACK_SIZE), 1 + FEEDBACK_SIZE }), cells { { head } }
{
    static int ids { 0 };
    id = ids++;
}

void Snake::SetStatistics(GameStatistics* statistics)
{
    statistics->averageFitness += GetFitness();
    statistics->averageScore += cells.size();
    
    if (GetFitness() > statistics->bestFitness)
    {
        statistics->bestFitness = GetFitness();
    }

    if (cells.size() > statistics->bestScore)
    {
        statistics->bestScore = static_cast<int>(cells.size());
    }
    
    if (GetFitness() > statistics->bestFitnessOfPopulation)
    {
        statistics->bestFitnessOfPopulation = GetFitness();
    }

    if (cells.size() > statistics->bestScoreOfPopulation)
    {
        statistics->bestScoreOfPopulation = static_cast<int>(cells.size());
    }
}

bool Snake::EatsItself()
{
    for (int i = 1; i < cells.size(); i++)
    {
        if (cells[0].x == cells[i].x && cells[0].y == cells[i].y)
        {
            return true;
        }
    }

    return false;
}

void Snake::Update()
{
    SaveLastPositionOfCells();

    Decide();

    MoveHead();
    Clamp();
    IfEatingApple();
    MoveTail();

    age += 1;
}

void Snake::Decide()
{
    auto distanceToTail = DistanceToTail();

    int distanceForwardToWall;
    int distanceLeftToWall;
    int distanceRightToWall;
    int distanceBackToWall;

    int distanceForwardToTail;
    int distanceLeftToTail;
    int distanceRightToTail;
    int distanceBackToTail;

    int direction = (int) this->direction;

    int distanceForwardToApple;
    int distanceSidewaysToApple;

    std::vector<float> encodedField;
    
      for(int x = 0; x < FIELD_SIZE; x++){
            for(int y = 0; y < FIELD_SIZE; y++){
                encodedField.push_back(0);        
            }
        }

        
                for (const auto& snakeCell : cells)
                {
                    int coord = snakeCell.x + snakeCell.y * FIELD_SIZE;
                    encodedField[coord] = 1;
                }


    switch (this->direction)
    {
        case Direction::Up:
            distanceForwardToWall = GetDistanceToUpWall();
            distanceLeftToWall = GetDistanceToLeftWall();
            distanceRightToWall = GetDistanceToRightWall();
            distanceBackToWall = GetDistanceToDownWall();
            distanceForwardToTail = distanceToTail[3];
            distanceBackToTail = distanceToTail[2];
            distanceRightToTail = distanceToTail[1];
            distanceLeftToTail = distanceToTail[0];
            distanceForwardToApple = GetDistanceToAppleY();
            distanceSidewaysToApple = -GetDistanceToAppleX();
            break;
        case Direction::Left:
            distanceForwardToWall = GetDistanceToLeftWall();
            distanceLeftToWall = GetDistanceToDownWall();
            distanceRightToWall = GetDistanceToUpWall();
            distanceBackToWall = GetDistanceToRightWall();
            distanceForwardToTail = distanceToTail[0];
            distanceBackToTail = distanceToTail[1];
            distanceRightToTail = distanceToTail[3];
            distanceLeftToTail = distanceToTail[2];
            distanceForwardToApple = GetDistanceToAppleX();
            distanceSidewaysToApple = GetDistanceToAppleY();
            break;
        case Direction::Down:
            distanceForwardToWall = GetDistanceToDownWall();
            distanceLeftToWall = GetDistanceToRightWall();
            distanceRightToWall = GetDistanceToLeftWall();
            distanceBackToWall = GetDistanceToUpWall();
            distanceForwardToTail = distanceToTail[2];
            distanceBackToTail = distanceToTail[3];
            distanceRightToTail = distanceToTail[0];
            distanceLeftToTail = distanceToTail[1];
            distanceForwardToApple = -GetDistanceToAppleY();
            distanceSidewaysToApple = GetDistanceToAppleX();
            break;
        case Direction::Right:
            distanceForwardToWall = GetDistanceToRightWall();
            distanceLeftToWall = GetDistanceToUpWall();
            distanceRightToWall = GetDistanceToDownWall();
            distanceBackToWall = GetDistanceToLeftWall();
            distanceForwardToTail = distanceToTail[1];
            distanceBackToTail = distanceToTail[0];
            distanceRightToTail = distanceToTail[2];
            distanceLeftToTail = distanceToTail[3];
            distanceForwardToApple = -GetDistanceToAppleX();
            distanceSidewaysToApple = -GetDistanceToAppleY();
            break;
    }

// distanceForwardToWall = 0;
// distanceLeftToWall = 0;
// distanceRightToWall = 0;
// distanceBackToWall = 0;
// distanceForwardToTail = 0;
// distanceLeftToTail = 0;
// distanceRightToTail = 0;
// distanceBackToTail = 0;
    std::vector<float> input = {
        static_cast<float>(distanceForwardToWall)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceLeftToWall)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceRightToWall)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceBackToWall)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceForwardToTail)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceBackToTail)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceRightToTail)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceLeftToTail)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceForwardToApple)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(distanceSidewaysToApple)    / static_cast<float>(FIELD_SIZE),
        static_cast<float>(direction)    / static_cast<float>(3),
        static_cast<float>(movesLeft) / static_cast<float>(FIELD_SIZE * FIELD_SIZE + FIELD_SIZE),
        static_cast<float>(cells.size()) / static_cast<float>(FIELD_SIZE * FIELD_SIZE),
        static_cast<float>(age) / static_cast<float>(FIELD_SIZE * FIELD_SIZE * FIELD_SIZE),
        static_cast<float>(GetFitness()) / static_cast<float>(cells.size()),
        static_cast<float>(movesLeft) / static_cast<float>(maxAmountOfMoves),
    };

    input.insert(input.end(), std::begin(feedback), std::end(feedback));
    // input.insert(input.end(), std::begin(encodedField), std::end(encodedField));

    auto result = neuralNetwork.Feed(input);

    Direction newDirection;
    int turn = 0;

    if (result[0] < 0.3f){
        turn = 1;
        // for (int i = 49; i > 0; i--)
        // {
        //     this->leftRightBias[i-1] = this->leftRightBias[i];
        // }
        // this->leftRightBias[49] = true;
    }
    if (result[0] > 0.7f){
        turn = 2;
        // for (int i = 49; i > 0; i--)
        // {
        //     this->leftRightBias[i-1] = this->leftRightBias[i];
        // }
        // this->leftRightBias[49] = false;
    }

    std::memcpy(feedback, &result[1], sizeof(float) * FEEDBACK_SIZE);
    
    switch ( turn ) {
        case 0: {
            newDirection = this->direction;
            break;
        }
        case 1:{
            newDirection = (Direction) ((((int)this->direction) + 1) % 4) ;
            break;
        }
        case 2: {
            newDirection = (Direction) ((((int)this->direction) + 3) % 4) ;
            break;
        }
    }

    if (DirectionIsValid(newDirection))
    {
        SetDirection(newDirection);
    }
}

void Snake::IfEatingApple()
{
    if (GetHead() == apple)
    {
     

        float scoreForThis = 1.0 - ((maxAmountOfMoves - movesLeft) / (float)maxAmountOfMoves);
        // score += 1;
        score += scoreForThis;

        AddTail();
        maxAmountOfMoves += 1; // 0.5 extra
        if(cells.size() > FIELD_SIZE * FIELD_SIZE - 2){
        //   alive = false;
        }
        
        std::vector<Cell> possibleCells = {Cell(3,3)};
        for(int x = 0; x < FIELD_SIZE; x++){
            for(int y = 0; y < FIELD_SIZE; y++){
                Cell cell = Cell(x, y);
                bool isSnakeCell = false;
                for (const auto& snakeCell : cells)
                {
                    if (snakeCell == cell)
                    {
                        isSnakeCell = true;
                        break;
                    }
                }
                if(!isSnakeCell){
                    possibleCells.push_back(cell);
                }
            }
        }

        int randomIndex = rand() % possibleCells.size();
        this->apple =  Apple(possibleCells[randomIndex]);

        
        movesLeft = maxAmountOfMoves;
    }
}

void Snake::MoveHead()
{
    GetHead().x += moves[direction].x;
    GetHead().y += moves[direction].y;
    movesLeft -= 1;
}

void Snake::SaveLastPositionOfCells()
{
    for (auto& cell : cells)
    {
        cell.lastX = cell.x;
        cell.lastY = cell.y;
    }
}

void Snake::MoveTail()
{
    for (int i = 1; i < cells.size(); i++)
    {
        Cell& lastCell = cells[i - 1];
        cells[i].x = lastCell.lastX;
        cells[i].y = lastCell.lastY;
    }
}

void Snake::Draw(Field& field) const
{
    for (const auto& cell : cells)
    {
        int x = cell.x;
        int y = cell.y;

        field[x][y].state = RenderableCell::State::Snake;

        float r = neuralNetwork.GetWeight(1, 1, 0);
        float g = neuralNetwork.GetWeight(1, 2, 0);
        float b = neuralNetwork.GetWeight(1, 3, 0);

        field[x][y].color = ImVec4(
            std::min(0.1f + r, 1.0f),
            std::min(0.6f + g, 1.0f),
            std::min(0.2f + b, 1.0f),
            1.0f
        );
    }
}

void Snake::AddTail()
{
    Cell& lastCell = GetLastCell();
    int x = lastCell.x;
    int y = lastCell.y;

    RenderableCell cell = RenderableCell(x, y);
    
    cells.emplace_back(cell);
}

int Snake::GetDistanceToUpWall()
{
    return cells[0].y;
}

int Snake::GetDistanceToLeftWall()
{
    return cells[0].x;
}

int Snake::GetDistanceToDownWall()
{
    return FIELD_SIZE - cells[0].y - 1;
}

int Snake::GetDistanceToRightWall()
{
    return FIELD_SIZE - cells[0].x - 1;
}

int Snake::GetDistanceToAppleX()
{
    return cells[0].x - apple.x;
}

int Snake::GetDistanceToAppleY()
{
    return cells[0].y - apple.y;
}

std::array<float, 4> Snake::DistanceToTail()
{    
    std::array<float, 4> result;

    result[0] = FIELD_SIZE+1;
    result[1] = FIELD_SIZE+1;
    result[2] = FIELD_SIZE+1;
    result[3] = FIELD_SIZE+1;

    for (int i = 1; i < cells.size(); i++)
    {
        if (cells[i].y == cells[0].y)
        {
            // Left
            if (cells[i].x < cells[0].x)
            {
                auto possibleResult = static_cast<float>(cells[0].x - cells[i].x);
                if (possibleResult < result[0])
                {
                    result[0] = possibleResult;
                }
            }

            // Right
            if (cells[i].x > cells[0].x)
            {
                auto possibleResult = static_cast<float>(cells[i].x - cells[0].x);
                if (possibleResult < result[1])
                {
                    result[1] = possibleResult;
                }
            }
        }

        if (cells[i].x == cells[0].x)
        {
            // Down
            if (cells[i].y > cells[0].y)
            {
                auto possibleResult = static_cast<float>(cells[i].y - cells[0].y);
                if (possibleResult < result[2])
                {
                    result[2] = possibleResult;
                }
            }

            // Up
            if (cells[i].y < cells[0].y)
            {
                auto possibleResult = static_cast<float>(cells[0].y - cells[i].y);
                if (possibleResult < result[3])
                {
                    result[3] = possibleResult;
                }
            }
        }
    }

    return result;
}


void Snake::Clamp()
{
    if (EatsItself())
    {
        alive = false;
    }

    int leftTurns = 0;
    int rightTurns = 0;

    for (int i = 0; i < 50; i++)
    {
        if (this->leftRightBias[i])
        {
            leftTurns++;
        }else {
            rightTurns++;
        }
    }

    if (leftTurns == 50 || rightTurns == 50)
    {
        alive = false;
    }

    for (const auto& cell : cells)
    {
        int x = cell.x;
        int y = cell.y;

        if (x < 0 || x >= FIELD_SIZE || y < 0 || y >= FIELD_SIZE || movesLeft <= 0)
        {
            alive = false;
            break;
        }
    }
}

bool Snake::DirectionIsValid(Direction newDirection)
{
    if (direction == newDirection)                                         return false;

    if (direction == Direction::Left  && newDirection != Direction::Right) return true;
    if (direction == Direction::Right && newDirection != Direction::Left)  return true;
    if (direction == Direction::Up    && newDirection != Direction::Down)  return true;
    if (direction == Direction::Down  && newDirection != Direction::Up)    return true;

    return false;
}

void Snake::SetDirection(Direction newDirection)
{
    direction = newDirection;
    amountOfTurns += 1;
}

void Snake::Control(bool up, bool left, bool down, bool right)
{
    if (up)    SetDirection(Snake::Direction::Up);
    if (left)  SetDirection(Snake::Direction::Left);
    if (down)  SetDirection(Snake::Direction::Down);
    if (right) SetDirection(Snake::Direction::Right);
}

void Snake::Reset()
{
    cells.clear();
    cells.emplace_back(Cell(rand() % FIELD_SIZE, rand() % FIELD_SIZE));
    alive = true;
    direction = static_cast<Direction>(rand() % 4);
}

int fibonacci(int limit) {
    return (limit * (limit + 1)) / 2;
}

float Snake::GetFitness() const
{
    // return cells.size() - (age / 1000.0);
    
    return score;


    // int length = cells.size();
    // int bestScore = FIELD_SIZE * length + fibonacci(length);
    // int score = std::max(0, bestScore - age) * 1.0;
    // return score;

    // return age + cells.size() * 1000.0;

    // return (static_cast<float>(std::max(cells.size() - 2.0,tgamma(length+1) 0.0)) * 1000.0) / static_cast<float>(age + FIELD_SIZE - cells.size());
    
    // length | best age
    // 1 > FIELD_SIZE
    // 2 > FIELD_SIZE + FIELD_SIZE+1
    // 3 > FIELD_SIZE + FIELD_SIZE+1 + FIELD_SIZE+2



}

bool Snake::IsAlive() const
{
    return alive;
}

Cell& Snake::GetHead()
{
    return cells[0];
}

Cell& Snake::GetLastCell()
{
    return cells[cells.size() - 1];
}

Apple& Snake::GetApple()
{
    return apple;
}

int Snake::GetID() const
{
    return id;
}