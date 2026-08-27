#pragma once
#include <map>

template <class T>
class CSTLMap
{
public:
    using Iterator = typename std::map<int, T*>::iterator;

    std::map<int, T*> m_UserTypeMap;

    size_t GetSize() const { return m_UserTypeMap.size(); }

    bool PutData(int key_value, T* pData)
    {
        if (!pData) return false;
        m_UserTypeMap[key_value] = pData; // overwrite
        return true;
    }

    T* GetData(int key_value) const
    {
        auto iter = m_UserTypeMap.find(key_value);
        return (iter == m_UserTypeMap.end()) ? nullptr : iter->second;
    }

    bool DeleteData(int key_value)
    {
        auto iter = m_UserTypeMap.find(key_value);
        if (iter == m_UserTypeMap.end())
            return false;

        delete iter->second; // ⚠️ ownership varsa bırak
        m_UserTypeMap.erase(iter);
        return true;
    }

    void DeleteAllData()
    {
        for (auto& pair : m_UserTypeMap)
            delete pair.second;

        m_UserTypeMap.clear();
    }

    bool IsExist(int key_value) const
    {
        return m_UserTypeMap.find(key_value) != m_UserTypeMap.end();
    }

    bool IsEmpty() const { return m_UserTypeMap.empty(); }

    ~CSTLMap()
    {
        DeleteAllData();
    }
};