#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


int main() {



    std::string fileName = "mesh.vtk.csv";

std::vector<double> x , y  , volume;

    std::vector<std::string> row;
    std::string line, word;

    std::fstream file (fileName, std::ios::in);
	if(file.is_open())
	{
        std::getline(file, line);
		while(std::getline(file, line))
		{
			row.clear();
 
            std::stringstream str(line);
 
			while(std::getline(str, word, ','))
            {
                row.push_back(word);
            }
            x.push_back(std::stod(row[1]));
            y.push_back(std::stod(row[2]));
            volume.push_back(std::stod(row[3]));

		}
	}



}
