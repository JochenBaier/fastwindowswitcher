#pragma once

#include <QString>

namespace FastWindowSwitcher
{
  namespace XmlFunctions
  {
    QString ReadString(const QString& p_fileContent, const QString& p_queryString, const QString& p_defaultValue);
    int ReadInt(const QString& p_fileContent, const QString& p_queryString, const int& p_base, const int& p_defaultValue);
  };
};