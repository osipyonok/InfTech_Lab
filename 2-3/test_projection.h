#ifndef TEST_PROJECTION_H
#define TEST_PROJECTION_H

#include <QString>
#include <database.h>
#include <QDebug>
#include <bits/stdc++.h>

using namespace std;

class Test_projection
{
public:
    Test_projection();
private:
    void printTest(bool verdict , DataTable tbl , vector<QString> res);
};

#endif // TEST_PROJECTION_H
