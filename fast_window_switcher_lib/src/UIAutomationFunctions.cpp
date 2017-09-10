/***************************************************************************
**
** Copyright (C) 2017 Jochen Baier
** Contact: email@jochen-baier.
**
** This file is part of the FastWindowSwitcher

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see < http://www.gnu.org/licenses/>.

Dieses Programm ist Freie Software : Sie können es unter den Bedingungen
der GNU General Public License, wie von der Free Software Foundation,
Version 3 der Lizenz oder(nach Ihrer Wahl) jeder neueren
veröffentlichten Version, weiterverbreiten und / oder modifizieren.

Dieses Programm wird in der Hoffnung, dass es nützlich sein wird, aber
OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
Siehe die GNU General Public License für weitere Details.

Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
Programm erhalten haben.Wenn nicht, siehe < http://www.gnu.org/licenses/>.
******************************************************************************/


#include <UIAutomation.h>
#include "UIAutomationFunctions.hpp"
#include "UIControlViewWalker.hpp"

namespace FastWindowSwitcherLib
{
  //Functions related to Microsoft UI Automation API. Neede to access the panel elements
  namespace UIAutomationFunctions
  {
    bool InitializeUIAutomation(IUIAutomation **ppAutomation)
    {
      const HRESULT res = CoCreateInstance(CLSID_CUIAutomation, nullptr, CLSCTX_INPROC_SERVER, IID_IUIAutomation, reinterpret_cast<void**>(ppAutomation));
      return (res == S_OK);
    }

    void DeInitializeUIAutomation(IUIAutomation& p_automation)
    {
      p_automation.Release();
    }

    //Returns the UI Automation handle for a windows handle
    UIElementPtr GetElementFromHandle(IUIAutomation& g_pAutomation, const quintptr& p_window)
    {
      Q_ASSERT(p_window);

      IUIAutomationElement* element = nullptr;

      const HRESULT hr = g_pAutomation.ElementFromHandle((HWND)p_window, &element);
      if (hr != S_OK)
      {
        return MakeUniqueUIElement(nullptr);
      }

      UIElementPtr elementPtr = MakeUniqueUIElement(element);
      return elementPtr;
    }

    //Calls the Invoke pattern. For panel elements this simulates a click
    void Invoke(IUIAutomationElement& p_element)
    {
      IUnknown* iUnknownInterface = nullptr;

      HRESULT hr = p_element.GetCurrentPattern(UIA_InvokePatternId, &iUnknownInterface);
      if (hr != S_OK)
      {
        //FIXME inform user
        Q_ASSERT(false);
        return;
      }

      Q_ASSERT(iUnknownInterface);

      p_element.SetFocus();

      auto invokePattern = (IUIAutomationInvokePattern*)(iUnknownInterface);
      hr = invokePattern->Invoke();
      Q_ASSERT(!FAILED(hr)); //FIXME inform user

      iUnknownInterface->Release();
    }

    //Returns the bounding rectangle of a panel element
    QRect GetElementRectangle(IUIAutomationElement& p_element)
    {
      VARIANT propertyValue = { 0 };
      HRESULT hr = p_element.GetCurrentPropertyValue(UIA_BoundingRectanglePropertyId, &propertyValue);
      if (hr != S_OK)
      {
        Q_ASSERT(false);
        return QRect();
      }

      double* data = nullptr;
      hr = SafeArrayAccessData(propertyValue.parray, (void**)&data);
      if (hr != S_OK)
      {
        Q_ASSERT(false);
        return QRect();
      }

      QRect rectangle(data[0], data[1], data[2], data[3]);

      hr = SafeArrayUnaccessData(propertyValue.parray);
      Q_ASSERT(hr == S_OK);

      return rectangle;
    }

    //Returns all UI child elements for a parent element
    std::vector<UIElementPtr> GetChildElements(IUIAutomation& p_automation, IUIAutomationElement& p_parentNode)
    {
      std::vector<UIElementPtr> childs;

      UIControlViewWalker p_controlViewWalker(p_automation);
      UIElementPtr child = p_controlViewWalker.GetFirstChildElement(p_parentNode);

      while (child)
      {
        IUIAutomationElement& currentSibling = *child;
        childs.push_back(std::move(child));
        child = p_controlViewWalker.GetNextSiblingElement(currentSibling);
      }

      return childs;
    }
  }
}
