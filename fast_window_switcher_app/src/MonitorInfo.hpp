#pragma once

#include <QRect>
#include <QWidget>

#include "TaskbarPanel.hpp"
#include <memory>
#include <optional>
#include "MarkerWindow.h"

class MonitorInfo
{
public:

  MonitorInfo(const QRect&  p_availableGeometry, const QWidget* p_screenWidget, const int& p_index, const std::optional<TaskbarPanel>& p_panel, const bool& p_isPrimary) :
    m_availableGeometry(p_availableGeometry), m_screenWidget(p_screenWidget), m_index(p_index), m_panel(p_panel), m_isPrimary(p_isPrimary)
  {
  }

  const QRect GetAvailableGemoetry() const
  {
    return m_availableGeometry;
  }

  const QWidget* GetScreenWidget() const
  {
   return m_screenWidget;
  }

  const int GetIndex() const
  {
    return m_index;
  }

  bool HasPanel() const
  {
	  return m_panel.has_value();
  }
   
  const std::optional<TaskbarPanel> GetPanel() const
  {
	  return m_panel;
  }

  const bool IsPrimary() const
  {
    return m_isPrimary;
  }
   
private:
  
  const QRect m_availableGeometry;
  const QWidget* m_screenWidget;
  const int m_index;

  const bool m_isPrimary;
    
  const std::optional<TaskbarPanel> m_panel;

  
   
};

typedef std::unique_ptr<MonitorInfo> MonitorPtr;
