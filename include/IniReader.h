#ifndef INIREADER_H
#define INIREADER_H
#include "ini_parser.hpp"
#include <string>
#include <Windows.h>

/*
*  String comparision functions, with case sensitive option
*/

using std::strcmp;

inline int strcmp(const char* str1, const char* str2, bool csensitive)
{
    return (csensitive ? ::strcmp(str1, str2) : ::_stricmp(str1, str2));
}

inline int strcmp(const char* str1, const char* str2, size_t num, bool csensitive)
{
    return (csensitive ? ::strncmp(str1, str2, num) : ::_strnicmp(str1, str2, num));
}

inline int compare(const std::string& str1, const std::string& str2, bool case_sensitive)
{
    if (str1.length() == str2.length())
        return strcmp(str1.c_str(), str2.c_str(), case_sensitive);
    return (str1.length() < str2.length() ? -1 : 1);
}

inline int compare(const std::string& str1, const std::string& str2, size_t num, bool case_sensitive)
{
    if (str1.length() == str2.length())
        return strcmp(str1.c_str(), str2.c_str(), num, case_sensitive);
    return (str1.length() < str2.length() ? -1 : 1);
}

inline int compare(const char* str1, const char* str2, bool case_sensitive)
{
    return strcmp(str1, str2, case_sensitive);
}

inline int compare(const char* str1, const char* str2, size_t num, bool case_sensitive)
{
    return strcmp(str1, str2, num, case_sensitive);
}

inline bool starts_with(const char* str, const char* prefix, bool case_sensitive)
{
    while (*prefix)
    {
        bool equal;
        if (case_sensitive)
            equal = (*str++ == *prefix++);
        else
            equal = (::tolower(*str++) == ::tolower(*prefix++));

        if (!equal) return false;
    }
    return true;
}

inline bool ends_with(const char* str, const char* prefix, bool case_sensitive)
{
    auto str2 = &str[strlen(str) - 1];
    auto prefix2 = &prefix[strlen(prefix) - 1];

    while (prefix2 >= prefix)
    {
        bool equal;
        if (case_sensitive)
            equal = (*str2-- == *prefix2--);
        else
            equal = (::tolower(*str2--) == ::tolower(*prefix2--));

        if (!equal) return false;
    }
    return true;
}

class CIniReader
{
private:
    std::string m_szFileName;

public:
    linb::ini data;

    CIniReader()
    {
        SetIniPath("");
    }

    CIniReader(char* szFileName)
    {
        SetIniPath(szFileName);
    }

    CIniReader(const char* szFileName)
    {
        SetIniPath((char*)szFileName);
    }

    CIniReader(std::stringstream& ini_mem)
    {
        data.load_file(ini_mem);
    }

    bool operator==(CIniReader& ir)
    {
        if (data.size() != ir.data.size())
            return false;

        for (auto& section : data)
        {
            for (auto& key : data[section.first])
            {
                if (key.second != ir.data[section.first][key.first])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(CIniReader& ir)
    {
        return !(*this == ir);
    }

    bool CompareBySections(CIniReader& ir)
    {
        if (data.size() != ir.data.size())
            return false;

        for (auto& section : data)
        {
            if (ir.data.find(section.first) == ir.data.end())
                return false;
            
            if (section.second.size() != ir.data.find(section.first)->second.size())
                return false;

            if (section.first != ir.data.find(section.first)->first)
                return false;
        }
        return true;
    }

    bool CompareByValues(CIniReader& ir)
    {
        return *this == ir;
    }

    const std::string& GetIniPath()
    {
        return m_szFileName;
    }

    void SetIniPath()
    {
        SetIniPath("");
    }

    void SetIniPath(char* szFileName)
    {
        char buffer[MAX_PATH];
        HMODULE hm = NULL;
        GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&ends_with, &hm);
        GetModuleFileNameA(hm, buffer, sizeof(buffer));
        std::string modulePath = buffer;

        if (strchr(szFileName, ':') != NULL)
        {
            m_szFileName = szFileName;
        }
        else if (std::string(szFileName).length() == 0)
        {
            m_szFileName = modulePath.substr(0, modulePath.find_last_of('.')) + ".ini";
        }
        else
        {
            m_szFileName = modulePath.substr(0, modulePath.rfind('\\') + 1) + szFileName;
        }

        data.load_file(m_szFileName);
    }

    int ReadInteger(char* szSection, char* szKey, int iDefaultValue)
    {
        try {
            auto str = data.get(szSection, szKey, std::to_string(iDefaultValue));
            return std::stoi(str, nullptr, starts_with(str.c_str(), "0x", false) ? 16 : 10);
        }
        catch (...) {
            return iDefaultValue;
        }
    }

    float ReadFloat(char* szSection, char* szKey, float fltDefaultValue)
    {
        try {
            return (float)atof(data.get(szSection, szKey, std::to_string(fltDefaultValue)).c_str());
        }
        catch (...) {
            return fltDefaultValue;
        }
    }

    bool ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue)
    {
        try {
            auto& config = data[szSection];
            if (config.count(szKey))
            {
                if (config[szKey].size() == 1) return config[szKey].front() != '0';
                return !!compare(config[szKey], "false", false);
            }
        }
        catch (...) {
            return bolDefaultValue;
        }
    }

    char* ReadString(char* szSection, char* szKey, const char* szDefaultValue)
    {
        char* szResult = new char[255];
        try {
            auto& config = data[szSection];
            if (config.count(szKey))
            {
                if (config[szKey].at(0) == '\"' || config[szKey].at(0) == '\'')
                    config[szKey].erase(0, 1);

                if (config[szKey].at(config[szKey].size() - 1) == '\"' || config[szKey].at(config[szKey].size() - 1) == '\'')
                    config[szKey].erase(config[szKey].size() - 1);

                strcpy(szResult, config[szKey].c_str());
                return szResult;
            }
        }
        catch (...) { }
        strcpy(szResult, szDefaultValue);
        return szResult;
    }

    std::string ReadString(char* szSection, char* szKey, std::string szDefaultValue)
    {
        char* str = ReadString(szSection, szKey, szDefaultValue.c_str());
        std::string* szResult = new std::string(str);
        return *szResult;
    }

    void WriteInteger(char* szSection, char* szKey, int iValue, bool useparser = false)
    {
        if (useparser)
        {
            data.set(szSection, szKey, std::to_string(iValue));
            data.write_file(m_szFileName);
        }
        else
        {
            char szValue[255];
            _snprintf_s(szValue, 255, "%s%d", " ", iValue);
            WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName.c_str());
        }
    }

    void WriteFloat(char* szSection, char* szKey, float fltValue, bool useparser = false)
    {
        if (useparser)
        {
            data.set(szSection, szKey, std::to_string(fltValue));
            data.write_file(m_szFileName);
        }
        else
        {
            char szValue[255];
            _snprintf_s(szValue, 255, "%s%f", " ", fltValue);
            WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName.c_str());
        }
    }

    void WriteBoolean(char* szSection, char* szKey, bool bolValue, bool useparser = false)
    {
        if (useparser)
        {
            data.set(szSection, szKey, std::to_string(bolValue));
            data.write_file(m_szFileName);
        }
        else
        {
            char szValue[255];
            _snprintf_s(szValue, 255, "%s%s", " ", bolValue ? "True" : "False");
            WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName.c_str());
        }
    }

    void WriteString(char* szSection, char* szKey, char* szValue, bool useparser = false)
    {
        if (useparser)
        {
            data.set(szSection, szKey, szValue);
            data.write_file(m_szFileName);
        }
        else
        {
            WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName.c_str());
        }
    }
};

#endif //INIREADER_H
