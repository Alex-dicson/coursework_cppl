#include <iostream>
#include <vector>
#include <fstream>
#include


using namespace std;


class ini_parser
{
private:
    string _filename;
    vector<string> _section_name;
    string _var_name;
public:

    ini_parser(string filename)
    {
        ifstream ini_file;
        try
        {
            ini_file.open(filename);
            _filename = filename;
        }
        catch (const ifstream::failure & ex)
        {
            cout << ex.what() << endl;
            cout << "Error, file not open!!!" << endl;
        }
    }
    ini_parser(const ini_parser& other): ini_parser(other._filename){}
    ~ini_parser()
    {
        ifstream ini_file;
        ini_file.open(_filename);
        if(ini_file.is_open())
        {
            ini_file.close();
        }
    }
    template<class T>
    T get_value(string varname)
    {
        string section, var;
        section = varname.substr(0, varname.find('.'));
        varname.erase(0, (varname.find('.') + 1));
        var = varname;
        string temp_line, temp_subline;
        ifstream in_file(_filename);
        bool found;
        vector<string> lines;
        while (getline(in_file,temp_line))
        {
            if((temp_line[0] == '[') && (temp_line.find(section) != string::npos))
            {
                found = true;
                while (getline(in_file,temp_subline))
                {
                    if(temp_line[0] == '[')
                    {
                        break;
                    }
                    lines.push_back(temp_subline);

                }
            }
        }
        if (lines.size() == 0)
        {
            throw ("Not find value");
        } else
        {
            for (auto str: lines)
            {
                if (str.find(var) != string::npos)
                {
                    int pos = str.find('=');
                    auto value = str.substr(pos, str.size());
                    return stoi(value);

                }
            }
        }


    }


};


int main()
{
    ini_parser parser("in.ini");
    auto value = parser.get_value<int>("section.value");
    return 0;
}
