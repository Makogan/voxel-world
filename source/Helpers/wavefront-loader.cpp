//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	Defines methods needed to load wavefront (.obj) meshes.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "wavefront-loader.hpp"
#include <algorithm>

using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Wavefront loader:
*/
//========================================================================================

/*
* Function to load the mesh information from a .obj file, it assumes triangular 
* meshes only. All return arrays must be cleared before using the function, else 
* information will be returned at the end of the arrays.
*
* Params: 
*   filename: the path to the file to be loaded.
*   vertices: a pointer to a vector of floats where the vertex information will be loaded
*   normals: a pointer to a vector of floats where the normal information will be loaded
*   texture_coords: a pointer to a vector of floats where the texture 
*       mapping information will be loaded
*/
void load_obj(string filename, vector<float> *vertices, vector<float> *normals, 
    vector<float> *texture_coords)
{
    //temporal vectores to store the information until properly organized
    vector<float> vs, ns, ts; 

    // Create a file stream object to read mesh info
    ifstream wfile;
    wfile.open(filename);

    //error check
    if (!wfile) {
        cerr << "Unable to open file " << filename <<endl;
        exit(1);   // call system to stop
    }

    //read line by line 
    //WARNING! if a non number sequence of characters is found 
    //during string to float conversion this will crash
    //Be careful of comments and similar situations
    string line;
    while(getline(wfile, line))
    {
        //Create string stream and read the first word
        stringstream ss(line);
        string word;
        ss >> word;

        //If the first word in the line is 'v' 
        //then this is a vertex information line
        if(word == "v")
        {
            //convert and store info to temporary vertex array
            while(ss>>word)
            {
                vs.push_back(stof(word));   
            }
        }
        //If the first word in teh line is 'vt' this is 
        //a texture coordinate information line
        else if(word == "vt")
        {
            //convert and store info to temporary texture coordinates array
            while(ss>>word)
            {
                ts.push_back(stof(word));   
            }
        }
        //If the first word in teh line is 'vn' this is 
        //a normals information line
        else if(word == "vn")
        {
            //convert and store info to temporary normals array
            while(ss>>word)
            {
                ns.push_back(stof(word));   
            }
        }
    }

    //reset stream to the beggining of the file
    wfile.clear();
    wfile.seekg(0, ios::beg);

    //once again read every line
    while(getline(wfile, line))
    {
        //once again read word by word
        stringstream ss(line);
        string word;
        ss >> word;

        //if the first word is 'f' this is a face information line
        if(word == "f")
        {
            //process the word, there should be 3 consecutive such words only
            while(ss>>word)
            {
                //split the string
                replace(word.begin(), word.end(), '/', ' ');
                
                //Store index information of the vertex/texture/normal relationship
                stringstream ws(word);
                vector<int> indices;
                string number;
                for(int i=0; i<3; i++)
                {
                    ws >> number;
                    indices.push_back(stoi(number)-1);
                }
                
                //Push associated vertex and normal information into the return arrays 
                //Now each vertex is associated with the correct normal.
                for(int i=0; i<3; i++)
                {
                    vertices->push_back(vs[indices[0]*3 + i]);
                    normals->push_back(ns[indices[2]*3 + i]);
                }

                //Same as above for texture coordinates
                for(int i=0; i<2; i++)
                    texture_coords->push_back(ts[indices[1]*2 + i]);

            }
        }
    }
    //Close the file
    wfile.close();
}

//########################################################################################