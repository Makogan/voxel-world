#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h> 
#include <string>

void load_obj(std::string filename, std::vector<float> *vertices, std::vector<float> *normals, std::vector<float> *texture_coords);