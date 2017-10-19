#include "utility.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>

namespace sis {


auto getwords(std::string s) -> std::vector<std::string>
{
    std::vector<std::string> v;
    std::string k;
    int i = 0;
    for(auto c : s)
    {
        ++i;
        if (c == ' ')
        {
            v.push_back(k);
            k.clear();
            continue;
        }
        k.push_back(c);
        if (i == s.size())
        {
            v.push_back(k);
        }
    }
    return v;
}

void getwords1(std::string s, std::string* v)
{
    std::string k;
    int i = 0;
    int j = 0;
    for(auto c : s)
    {
        ++i;
        if (c == ' ')
        {
            v[j] = k;
            ++j;
            k.clear();
            continue;
        }
        if (j == 6)
            break;
        k.push_back(c);
        if (i == s.size())
        {
            v[j] = k;
            ++j;
        }
    }
}

void readPly(std::string filename, QVector<float>& ptz, QVector<float> &colorz, float r[], float centroid[])
{
    boost::iostreams::stream<boost::iostreams::file_source> f(filename);
    std::string s;
    int t = 5;
    for(int i = 0; i<t; ++i)
    {
        std::getline(f, s);
        if (s.find("element vertex") != std::string::npos)
        {
            break;
        }
        if (i == 3)
        {
            // try another 3 lines
            t = t + 3;
        }
    }
    auto w = getwords(s);
    if (w.size()<3)  return;

    int sz = std::stoi(w.back());
    for(int i = 0; i<15; ++i)
    {
        std::getline(f, s);
        if (s.find("end_header") != std::string::npos)
        {
            break;
        }
    }

    QVector<float>& points = ptz;
    points.resize(sz * 3);
    colorz.resize(sz * 3);

    std::vector<float> dz;
    float rz[6] = {1e5,-1e5,1e5,-1e5,1e5,-1e5};
    std::copy(rz, rz+6, r);
    std::string v[6];
    float xx = 0, yy = 0, zz = 0;
    for(int i=0; i<sz; ++i)
    {
        std::getline(f,s);
        getwords1(s, v);

        auto x = std::stof(v[0]);
        auto y = std::stof(v[1]);
        auto z = std::stof(v[2]);
        auto rr = std::stof(v[3]);
        auto gg = std::stof(v[4]);
        auto bb = std::stof(v[5]);

        points[i*3+0] = x;
        points[i*3+1] = y;
        points[i*3+2] = z;

        colorz[i*3+0] = rr/255.0;
        colorz[i*3+1] = gg/255.0;
        colorz[i*3+2] = bb/255.0;

        xx += x;
        yy += y;
        zz += z;

        if (r[0]>x) r[0] = x;
        if (r[1]<x) r[1] = x;
        if (r[2]>y) r[2] = y;
        if (r[3]<y) r[3] = y;
        if (r[4]>z) r[4] = z;
        if (r[5]<z) r[5] = z;
    }

    xx /= (float)sz;
    yy /= (float)sz;
    zz /= (float)sz;

    for(int i = 0; i<sz; ++i)
    {
        points[i*3+0] -= xx;
        points[i*3+1] -= yy;
        points[i*3+2] -= zz;
    }

    centroid[0] = xx;
    centroid[1] = yy;
    centroid[2] = zz;
}

}
