#include "XmlFunctions.hpp"
#include <QDebug>

#include <QXmlQuery>

namespace FastWindowSwitcher
{
  //XML helper functions
  namespace XmlFunctions
  {
    QString ReadString(const QString& p_fileContent, const QString& p_queryString, const QString& p_defaultValue)
    {
      Q_ASSERT(!p_queryString.isEmpty());

      QXmlQuery query;
      query.setFocus(p_fileContent);

      query.setQuery(p_queryString);
      if (!query.isValid())
      {
        Q_ASSERT(false);
        return p_defaultValue;
      }

      QString value;
      const bool ok = query.evaluateTo(&value);
      Q_ASSERT(ok);

      if (!ok)
      {
        return p_defaultValue;
      }

      //Bug? http://www.qtcentre.org/threads/42338-QXmlQuery-evaluateTo(QString)-adds-a-line-break
      if (value.endsWith('\n'))
      {
        value.chop(1);
      }

      return value;
    }

    int ReadInt(const QString& p_fileContent, const QString& p_queryString, const int& p_base, const int& p_defaultValue)
    {
      QString asString = ReadString(p_fileContent, p_queryString, "");
      if (asString.isEmpty())
      {
        return p_defaultValue;
      }

      bool convertOk = false;
      const int asInt = asString.toInt(&convertOk, p_base);
      if (!convertOk)
      {
        return p_defaultValue;
      }

      return asInt;
    }
  }
}
