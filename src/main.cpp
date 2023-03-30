#include "window/window.h"

#include "snake_ai.h"

#include "ai/genetic/genetic_algorithm.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;
int main()
{
    Window window;
    
    InitRandomEngine();

    SnakeAI snakeAi(window);
    
    static float mupdateTimer { 0 };

    
    window.BeginFrame();
    snakeAi.Render();
    window.EndFrame();
    auto lastUpdate = high_resolution_clock::now();

    while (!window.ShouldClose())
    {
        snakeAi.Update();
        auto now = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> delta = now - lastUpdate;
        std::cout << delta.count() << std::endl;
                     if (delta.count() > (1000/30) || !snakeAi.settings.forceFullSpeed)
            {   
            window.BeginFrame();
                snakeAi.Render();
            window.EndFrame();
            lastUpdate = now;
            }
    }

    return 0;
}