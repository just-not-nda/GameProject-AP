#pragma once

#ifndef GETERROR_H
#define GETERROR_H

#include <iostream>
#include <string>
using namespace std;

class GetError
{
    private:
        string place = "";
    public:
        GetError() {}

        GetError(string place)
        {
            this->place = place;
        }
        void Status(const string &error)
        {
            cout << place << ": " << error << endl;
        }
};

#endif // GETERROR_H
