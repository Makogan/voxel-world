#include "wavefront-loader.hpp"
#include <algorithm>

using namespace std;

bool stringtofloat(string str, float *val)
{
    float fvalue;
    try
    {
        fvalue = stof(str);
    }
    catch(invalid_argument)
    {
        return false;
    }

    *val = fvalue;
    return true;
}

void load_obj(string filename, vector<float> *vertices, vector<float> *normals, vector<float> *texture_coords)
{
    int vertex_num=0, normal_num=0, texture_num=0;

    vector<float> vs, ns, ts; 

    ifstream wfile;

    wfile.open(filename);

    if (!wfile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }

    string line;
    while(getline(wfile, line))
    {
        stringstream ss(line);
        string word;
        ss >> word;
        if(word == "v")
        {
            vertex_num++;
            while(ss>>word)
            {
                vs.push_back(stof(word));   
            }
        }
        else if(word == "vt")
        {
            texture_num++;
            while(ss>>word)
            {
                ts.push_back(stof(word));   
            }
        }
        else if(word == "vn")
        {
            normal_num++;
            while(ss>>word)
            {
                ns.push_back(stof(word));   
            }
        }
    }

    /*vertices = new vector<float>(vertex_num*3);
    normals = new vector<float>(normal_num*3);
    texture_coords = new vector<float>(texture_num*2);*/

    wfile.clear();
    wfile.seekg(0, ios::beg);

    while(getline(wfile, line))
    {
        stringstream ss(line);
        string word;
        ss >> word;

        if(word == "f")
        {
            while(ss>>word)
            {
                
                replace(word.begin(), word.end(), '/', ' ');
                
                stringstream ws(word);
                vector<int> indices;
                string number;
                
                for(int i=0; i<3; i++)
                {
                    ws >> number;
                    indices.push_back(stoi(number)-1);
                }
                
                for(int i=0; i<3; i++)
                {
                    vertices->push_back(vs[indices[0]*3 + i]);
                    normals->push_back(ns[indices[2]*3 + i]);
                }

                for(int i=0; i<2; i++)
                    texture_coords->push_back(ts[indices[1]*2 + i]);

            }
        }
    }

    wfile.close();
}

/*int main()
{
    vector<float> vs, ns, ts; 
    load_obj("Objs/cube.obj", &vs, &ns, &ts);
}*/