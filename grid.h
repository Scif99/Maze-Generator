#pragma once

#include <SFML/Graphics.hpp>

#include <array>
#include <utility>
#include <vector>   

/*
- Note that each cell only needs a right and down wall. We could add a wall on each four sides
- The m_corner member...
*/

struct Cell
{
    sf::RectangleShape m_room;
    sf::RectangleShape m_rightwall;
    sf::RectangleShape m_downwall;

    sf::RectangleShape m_corner; 
    bool b_used_{false}; //is this room used in the maze?

};


struct Grid
{
    std::vector<Cell> m_rooms_;
    int m_dim_;

    Grid(sf::RenderWindow& window, int dim);

    std::vector<std::pair<int, std::string>> get_neighbours(int i);
    void generate_maze(int start);

    auto begin() {return m_rooms_.begin();}
    auto end() {return m_rooms_.end();}

    int size() const noexcept {return m_dim_*m_dim_;}
};
