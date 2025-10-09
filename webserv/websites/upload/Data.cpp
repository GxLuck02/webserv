/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: proton <proton@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:21:18 by proton            #+#    #+#             */
/*   Updated: 2025/04/26 01:21:20 by proton           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Data.hpp"


Data::Data()
{
	std::cout << "in data base constructor" << std::endl;
}

Data::Data( int age, std::string name ) : age_(age), name_(name)
{
	std::cout << "in direct assignation data class" << std::endl;
}

Data::Data( const Data& copy )
{
	std::cout << "in copy constructor of Data" << std::endl;
	*this = copy;
}

Data& Data::operator=( const Data& copy )
{
	std::cout << "in copy operator of class data" << std::endl;
	if (this != &copy)
	{
		this->age_ = copy.age_;
		this->name_ = copy.name_;
	}
	return (*this);
}

Data::~Data()
{
	std::cout << "in data class destructor" << std::endl;
}

int		Data::getAge()
{
	return (this->age_);
}

std::string	Data::getName()
{
	return (this->name_);
}

