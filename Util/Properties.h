#ifndef _Properties_
#define _Properties_

#include <map>
#include <string>
#include <vector>

using namespace std;

/**
Simple association (dictionnary key-values ) class

Handle creation, loading and saving from files, adding and testing keys.

@author Etienne de Foras
@version 1.01
*/
class Properties
{

public:
    Properties();

    bool save(string sFileName);
    bool load(string sFileName);

    void set(string sKey, string sValue);
    void set(string sKey, int iValue);
    void set(string sKey, unsigned int uiValue);
    void set(string sKey, bool bValue);
    void set(string sKey, double dValue);
    void set(string sKey, const vector<double>& vdValue);


    bool exist(string sKey);

    string get(string sKey);
    int get_int(string sKey);
    unsigned int get_unsigned_int(string sKey);
    bool get_bool(string sKey);
    double get_double(string sKey);
    vector<double> get_vector_double(string sKey);

private:
    map<string,string> _pairs;
};

#endif
