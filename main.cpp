#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <cassert>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <ranges>
#include <stack>
#include <thread>
#include <vector>   

#include "grid.h"

int main()
{
    constexpr auto win_size{800.f};
    sf::RenderWindow window(sf::VideoMode(win_size,win_size),"Maze generator");
    window.setFramerateLimit(20);

    //Initialise 
    Grid grid(window,20);
    //grid.generate_maze(0); // can use this instead if you dont want to watch generation in real time...

    std::stack<int> stck;
    stck.push(0);  

    while(window.isOpen())
    {

        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        if(!stck.empty())
        {
            auto curr = stck.top();
            grid.m_rooms_[curr].b_used_ = true;
            auto nbours = grid.get_neighbours(curr);
            auto is_unused = [&grid](std::pair<int,std::string> p) { return !grid.m_rooms_[p.first].b_used_; };
            std::vector<std::pair<int,std::string>> unused_neighbours;
            std::copy_if(nbours.begin(),nbours.end(),std::back_inserter(unused_neighbours),is_unused);


            if(!unused_neighbours.empty()) //if there are no unused neighbours
            {
                auto [next,dir] = unused_neighbours[rand()%unused_neighbours.size()]; //pick a random unused neighbour

                //connect current to neighbour by removing the appropriate wall
                if(dir == "up")
                {
                    grid.m_rooms_[next].m_downwall.setFillColor(sf::Color::White);
                }

                if(dir == "left")
                {
                    grid.m_rooms_[next].m_rightwall.setFillColor(sf::Color::White);
                }

                if(dir == "right")
                {
                    grid.m_rooms_[curr].m_rightwall.setFillColor(sf::Color::White);
                }

                if(dir == "down")
                {
                    grid.m_rooms_[curr].m_downwall.setFillColor(sf::Color::White);
                }

                stck.push(next); //add next cell onto the stack
            }

            else
            {
                //backtrack to previous cell
                stck.pop();
            }
            //grid.m_rooms_[curr].m_room.setFillColor(sf::Color::White); //dont want to keep it colored green forever!
        }


        window.clear();
        for(const auto& cell : grid) 
        {
            window.draw(cell.m_room);
            window.draw(cell.m_downwall); 
            window.draw(cell.m_rightwall);
        }
        window.display();
    }

    return 0;
    
}