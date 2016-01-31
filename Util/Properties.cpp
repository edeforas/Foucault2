#include "Properties.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
Properties::Properties()
{ }
///////////////////////////////////////////////////////////////////////////////
void Properties::set(string sKey, int iValue)
{
    stringstream ss;
    ss << iValue;

    _pairs[sKey]=ss.str();
}
///////////////////////////////////////////////////////////////////////////////
void Properties::set(string sKey, bool bValue)
{
    _pairs[sKey]=bValue?"1":"0";
}
///////////////////////////////////////////////////////////////////////////////
void Properties::set(string sKey, double dValue)
{
    stringstream ss;
    ss << setprecision(10) << dValue;

    _pairs[sKey]=ss.str();
}
///////////////////////////////////////////////////////////////////////////////
//void Properties::set(string sKey, char* sValue)
//{
//    set(sKey,string(sValue));
//}
///////////////////////////////////////////////////////////////////////////////
void Properties::set(string sKey, string sValue)
{
    _pairs[sKey]=sValue;
}
///////////////////////////////////////////////////////////////////////////////
void Properties::set(string sKey, const vector<double>& vdValue)
{
    stringstream ss;
    for(unsigned int i=0;i<vdValue.size();i++)
    {
        ss << vdValue[i] ;
        if(i<vdValue.size()-1)
            ss << " ";
    }

    _pairs[sKey]=ss.str();
}
///////////////////////////////////////////////////////////////////////////////
bool Properties::exist(string sKey)
{
    map<string,string>::iterator it=_pairs.find(sKey);
    return it!=_pairs.end();
}
///////////////////////////////////////////////////////////////////////////////
string Properties::get(string sKey)
{
    map<string,string>::iterator it=_pairs.find(sKey);
    if(it!=_pairs.end())
        return (*it).second;
    else
        return "";
}
///////////////////////////////////////////////////////////////////////////////
int Properties::get_int(string sKey)
{
    map<string,string>::iterator it=_pairs.find(sKey);
    if(it!=_pairs.end())
    {
        stringstream ss;
        int iTmp;
        ss << (*it).second; //.str((*it).second);
        ss >> iTmp;
        return iTmp;
    } else
        return 0;
}
///////////////////////////////////////////////////////////////////////////////
bool Properties::get_bool(string sKey)
{
    map<string,string>::iterator it=_pairs.find(sKey);
    if(it!=_pairs.end())
        return (*it).second!="0";
    else
        return false;
}

///////////////////////////////////////////////////////////////////////////////
double Properties::get_double(string sKey)
{
    map<string,string>::iterator it=_pairs.find(sKey);
    if(it!=_pairs.end())
    {
        stringstream ss;
        double dTmp;
        ss << (*it).second;
        ss >> dTmp;
        return dTmp;
    } else
        return 0.;
}
///////////////////////////////////////////////////////////////////////////////
vector<double> Properties::get_vector_double(string sKey)
{
    vector<double> vd;

    stringstream ss;
    string sValue=get(sKey);

    if(sValue.empty())
        return vd;

    ss.str(sValue);

    while(!ss.eof())
    {
        double d;
        if(ss >> d)
            vd.push_back(d);
    }

    return vd;
}
///////////////////////////////////////////////////////////////////////////////
bool Properties::save(string sFileName)
{
    ofstream f(sFileName.c_str());
    if(!f)
        return false;

    map<string,string>::iterator it;
    for(it=_pairs.begin();it!= _pairs.end();++it)
    {
        string sValue=(*it).second;
        string sKey=(*it).first;

        if(!sValue.empty())
        {

            istringstream sText(sValue);
            string sLine;
            while ( getline( sText, sLine))
            {
                f << sKey << "=" << sLine << "\n";
            }
        }
        else
            f << sKey << "=" << "\n";
    }

    f.close();
    return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Properties::load(string sFileName)
{
    _pairs.clear();
    string sLine;

    ifstream f(sFileName.c_str());
    if(!f)
        return false;

    while(!f.eof())
    {
        getline(f,sLine);

        if(sLine.size()==0)
            continue;

        int iPosEgal=sLine.find_first_of("=");
        if(iPosEgal==-1)
            continue; //pas de clef!

        //suprime le charactere 13 eventuel a la fin TODO: proprifier
        if((int)(sLine[sLine.size()-1])==13)
            sLine=sLine.substr(0,sLine.size()-1);

        string sKey=sLine.substr(0,iPosEgal);
        string sValue=sLine.substr(iPosEgal+1);

        if(exist(sKey))
            set(sKey,get(sKey)+'\n'+sValue);
        else
            set(sKey,sValue);
    }

    f.close();

    return true;
}
//////////////////////////////////////////////////////////////////////////////