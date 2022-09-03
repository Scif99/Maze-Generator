#include "grid.h"

#include <cstdlib>

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <stack>
#include <thread>
#include <utility>
#include <vector>   

Grid::Grid(sf::RenderWindow& window, int dim) : m_dim_{dim}
{
    m_rooms_.reserve(dim*dim);
    constexpr auto wall_thickness{10.f};
    
    //Fill the grid with cells
    for(int i =0;i<dim;++i)
    {
        for(int j =0; j<dim;++j)
        {
            const auto cell_size{window.getSize().x/dim};
            //set properties of the cell
            Cell c;
            c.m_room.setSize(sf::Vector2f(cell_size - wall_thickness, cell_size - wall_thickness));
            c.m_room.setPosition(j*cell_size, i*cell_size);            
            c.m_room.setFillColor(sf::Color::White);

            //create walls around the room
            c.m_rightwall.setSize(sf::Vector2f(wall_thickness, c.m_room.getSize().y));
            c.m_rightwall.setPosition(c.m_room.getPosition().x + c.m_room.getSize().x, c.m_room.getPosition().y);
            c.m_rightwall.setFillColor(sf::Color::Black);
            
            c.m_downwall.setSize(sf::Vector2f(c.m_room.getSize().x, wall_thickness));
            c.m_downwall.setPosition(c.m_room.getPosition().x , c.m_room.getPosition().y + c.m_room.getSize().y);
            c.m_downwall.setFillColor(sf::Color::Black);

            c.m_corner.setSize(sf::Vector2f(wall_thickness,wall_thickness));
            c.m_corner.setPosition(c.m_room.getPosition().x + c.m_room.getSize().x, c.m_room.getPosition().y +  c.m_room.getSize().y);
            c.m_corner.setFillColor(sf::Color::Black);
            
            //shift so that it creates a border around the whole grid
            //The outer walls wont be removed anyway...
            constexpr auto half_thickness{wall_thickness/2};
            c.m_rightwall.move(half_thickness,half_thickness);
            c.m_downwall.move(half_thickness,half_thickness);
            c.m_corner.move(half_thickness,half_thickness);
            c.m_room.move(half_thickness,half_thickness);


            m_rooms_.push_back(std::move(c));
        }
    }
}

//Return a vector containing the indices of the neighbours for a cell, and the corresponding direction
//Only returns valid neighbours (i.e for a cell on the first column, its neighbours vector will not contain the left neighbour)
std::vector<std::pair<int, std::string>> Grid::get_neighbours(int i)
{
    std::vector<std::pair<int, std::string>> ans;

    if(i>=m_dim_) {ans.emplace_back(i - m_dim_,"up");} //up
    if(i<m_dim_*(m_dim_-1)) {ans.emplace_back(i + m_dim_,"down");} //down
    if(i%m_dim_!=0) {ans.emplace_back(i -1,"left");} //left
    if((i+1)%m_dim_!=0) {ans.emplace_back(i + 1,"right");} //right

    return ans;
}


void Grid::generate_maze(int start) 
{
    std::stack<int> stck;
    stck.push(start);            


    while(!stck.empty())
    {
        auto curr = stck.top();
        m_rooms_[curr].b_used_ = true;

        auto nbours = get_neighbours(curr);
        auto is_unused = [this](std::pair<int,std::string> p) { return !m_rooms_[p.first].b_used_; };
        std::vector<std::pair<int,std::string>> unused_neighbours;
        std::copy_if(nbours.begin(),nbours.end(),std::back_inserter(unused_neighbours),is_unused);

            if(!unused_neighbours.empty()) //if there are no unused neighbours
            {
                auto p = unused_neighbours[rand()%unused_neighbours.size()]; //pick a random unused neighbour

                //connect current to neighbour by removing the appropriate wall
                if(p.second == "up")
                {
                    m_rooms_[p.first].m_downwall.setFillColor(sf::Color::White);
                }

                if(p.second== "left")
                {
                    m_rooms_[p.first].m_rightwall.setFillColor(sf::Color::White);
                }

                if(p.second== "right")
                {
                    m_rooms_[curr].m_rightwall.setFillColor(sf::Color::White);
                }

                if(p.second== "down")
                {
                    m_rooms_[curr].m_downwall.setFillColor(sf::Color::White);
                }

                stck.push(p.first); //add next cell onto the stack
            }

        else
        {
            //backtrack to previous cell
            stck.pop();
        }
    }
    std::cout<<"Done\n";
}