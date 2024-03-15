#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>


using namespace std;


class iniParser
{
private:
    string _filename;
    typedef map<string, string> var_val;
    //typedef map<string, var_val> section;
    map<string, var_val> section;
    //vector<vector<string, vMap> val;

    void readFile(ifstream &in);
    void add_var_val(string section_string, string var, string val);

    string trim(const std::string &str,
                const std::string &whitespace = " \t");

public:

    iniParser(string filename);


};

iniParser::iniParser(std::string filename)
{


    ifstream ini_file(filename);
    _filename = filename;
    if (ini_file.is_open())
    {
        readFile(ini_file);
    } else
    {
        cout << "Ошибка открытия файла" << endl;
    }


}

void iniParser::add_var_val(string section_string, string var, string val)
{
    bool found = false;
    for (auto& sec: section)
    {
        if (sec.first == section_string)
        {
            found = true;
            for (auto& var_temp: sec.second)
            {
                if (var_temp.first == var)
                {
                    var_temp.second = val;
                }
            }
            section[sec.first].insert({var, val});
        }

    }
    if(!found)
    {
        var_val tmp;
        tmp.insert({var, val});
        section.insert({section_string, tmp});
    }
}

void iniParser::readFile(ifstream &in)
{
    string temp_line;
    string section_str;
    while (getline(in, temp_line))
    {
        string section_temp;
        var_val v_v;
        if (temp_line[0] == '[')
        {
            section_str = temp_line.substr(1, temp_line.size() - 2);
        }
        else if ((temp_line[0] == ';') || (temp_line[0] == '\t') || temp_line.empty())
        {
            continue;
        } else
        {
            string var = trim(temp_line.substr(0, temp_line.find('=')), "\t");
            string value;
            if (temp_line.find(';') != string::npos )
            {
                temp_line.erase(temp_line.find(';'), temp_line.size() - 1);
                value = trim(temp_line.substr((temp_line.find('=') + 1), (temp_line.size() - 1)),
                             "\t");
            } else
            {
                value = trim(temp_line.substr((temp_line.find('=') + 1), (temp_line.size() - 1)),
                             "\t");

            }
            add_var_val(section_str, var, value);
        }
    }

}

string iniParser::trim(const std::string &str,
                       const std::string &whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

int main()
{
    iniParser parser("in.ini");
    //auto value = parser.get_value<int>("section.value");
    return 0;
}
