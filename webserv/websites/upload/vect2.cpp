/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:51:12 by proton            #+#    #+#             */
/*   Updated: 2025/06/18 09:47:22 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vect2.hpp"

vect2::vect2()
{
	std::cout << "in base constructor" << std::endl;
}

vect2::vect2( int x, int y ): _x(x), _y(y)
{
	std::cout << "in direct assignation constructor" << std::endl;
}

vect2::vect2( const vect2& copy )
{
	*this = copy;
}

vect2& vect2::operator=( const vect2& copy )
{
	if (this != &copy)
	{
		this->_x = copy._x;
		this->_y = copy._y;
	}
	return (*this);
}

vect2::~vect2()
{
	std::cout << "in destructor" << std::endl;
}

vect2&	vect2::operator+( int num )
{
	this->_x += num;
	this->_y += num;
	return (*this);
}

vect2&	vect2::operator+( const vect2& copy )
{
	this->_x += copy._x;
	this->_y += copy._y;
	return (*this);
}

vect2&	vect2::operator-( int num )
{
	this->_x -= num;
	this->_y -= num;
	return (*this);
}

vect2&	vect2::operator-( const vect2& copy )
{
	this->_x -= copy._x;
	this->_y -= copy._y;
	return (*this);
}

vect2&	vect2::operator*( int num )
{
	this->_x *= num;
	this->_y *= num;
	return (*this);
}

vect2&	vect2::operator*( const vect2& copy )
{
	this->_x *= copy._x;
	this->_y *= copy._y;
	return (*this);
}

vect2&	vect2::operator+=( int num )
{
	this->_x += num;
	this->_y += num;
	return (*this);
}

vect2&	vect2::operator+=( const vect2& copy )
{
	this->_x += copy._x;
	this->_y += copy._y;
	return (*this);
}

vect2&	vect2::operator-=( int num )
{
	this->_x -= num;
	this->_y -= num;
	return (*this);
}

vect2&	vect2::operator-=( const vect2& copy )
{
	this->_x -= copy._x;
	this->_y -= copy._y;
	return (*this);
}

vect2&	vect2::operator*=( int num )
{
	this->_x *= num;
	this->_y *= num;
	return (*this);
}

vect2&	vect2::operator*=( const vect2& copy )
{
	this->_x *= copy._x;
	this->_y *= copy._y;
	return (*this);
}

