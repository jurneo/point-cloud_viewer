#ifndef SY_UTILITY_H
#define SY_UTILITY_H

#include <iostream>
#include <core/point3d.h>
#include <QVector>
#include <QVector3D>
#include <string>

namespace sis {

void readPly(std::string filename, QVector<float>& ptz, QVector<float> &colorz, float r[6], float centroid[3]);

}

#endif // SY_UTILITY_H
