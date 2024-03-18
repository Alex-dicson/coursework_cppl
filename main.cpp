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
    map<string, var_val> section;

    void readFile(ifstream &in);
    void add_var_val(string section_string, string var, string val);
    string get_value_string(string section_str, string var);
    string trim(const std::string &str,
                const std::string &whitespace = " \t");

public:

    iniParser(string filename);

    template<typename T>
    T get_value(string section_str, string var)
    {
        static_assert(sizeof(T) == -1, "not implemented type for get_value");
    }
};

// Конструктор, в нем пробуем открыть файл
iniParser::iniParser(std::string filename)
{


    ifstream ini_file(filename);
    _filename = filename;
    if (ini_file.is_open())
    {
        readFile(ini_file);
    } else
    {
        throw "Error open file"; // исключение если файл не удается открыть
    }


}

//Добавляем в карту карт секцию - переменную - значение
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

//Непосредственно чтение файл
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

//Удаляем лишние символы в начале и конце строки
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

//Получаем значение указанной переменной из указанной секции в виде строки
string iniParser::get_value_string(std::string section_str, std::string var)
{
    string found_sec, found;
    for (auto& section_read: section)
    {
        if (section_read.first == section_str)
        {
            found_sec = section_read.first;
            for (auto& var_it: section_read.second)
            {
                if(var_it.first == var)
                {
                    found = var_it.second;
                }
            }
        }
    }
    if (found_sec.empty()){ throw "Section not found";}
    else if (found.empty()){throw "Variable not found";}
    else { return found;}
}


//Шаблонные методы получение значения для различных типов данных
template<>
string iniParser::get_value(std::string section_str, std::string var)
{
    return get_value_string(section_str, var);
}

template<>
int iniParser::get_value(std::string section_str, std::string var)
{
    string val_str = get_value_string(section_str, var);
    try
    {
        int val_int = stoi(val_str);
        return val_int;
    }
    catch (...)
    {
        throw "Conversion error to int";
    }

}

template<>
double iniParser::get_value(std::string section_str, std::string var)
{
    string val_str = get_value_string(section_str, var);
    try
    {
        double val_double = stod(val_str);
        return val_double;
    }
    catch (...)
    {
        throw "Conversion error to double";
    }
}

template<>
float iniParser::get_value(std::string section_str, std::string var)
{
    string val_str = get_value_string(section_str, var);
    try
    {
        float val_float = stof(val_str);
        return val_float;
    }
    catch (...)
    {
        throw "Conversion error to float";
    }
}


int main()
{
    try
    {
        iniParser parser("in.ini");
        cout << parser.get_value<string>("Section1", "var1") << endl;
        cout << parser.get_value<int>("Section1", "var1") << endl;
        cout << parser.get_value<double>("Section1", "var1") << endl;
        //cout << parser.get_value<char>("Section1", "var1") << endl;
        
    }
    catch (const char* ex)
    {
        cout << ex << endl;
    }

    return 0;
}
