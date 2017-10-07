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

#include "SelectableElementRepository.hpp"

#include <QDebug>

#include "SelectableElement.hpp"

namespace FastWindowSwitcherLib
{
  //Repository of selectable elements

  //Sets new elements
  void SelectableElementRepository::SetElements(std::vector<SelectableElementPtr>& p_elements)
  {
    Q_ASSERT(m_elements.empty());
    m_elements = std::move(p_elements);
  }

  void SelectableElementRepository::ClearAll()
  {
    m_elements.clear();
  }

  //Get single element by reference
  SelectableElement& SelectableElementRepository::GetElement(const std::size_t& p_index)
  {
    Q_ASSERT(p_index < m_elements.size());
    return *m_elements[p_index];
  }

  //Get single element by const reference
  const SelectableElement& SelectableElementRepository::GetElement(const std::size_t& p_index) const //-V659
  {
    Q_ASSERT(p_index < m_elements.size());
    return *m_elements.at(p_index);
  }

  //Returns the count of the elements
  const std::size_t SelectableElementRepository::Count() const
  {
    return m_elements.size();
  }

  //Returns the index of an element which matches 'newText'
  const int SelectableElementRepository::GetIndexMatchingText(QString p_text, bool& p_oneElementStartsWith) const
  {
    Q_ASSERT(m_elements.size() > 0);
    Q_ASSERT(p_text.size() > 0);

    for (std::size_t i = 0; i < m_elements.size(); i++)
    {
      const SelectableElement& element = *m_elements[i];

      if (element.IsDisabled())
      {
        continue;
      }

      QString key_combination = element.GetKeyCombination();

      p_oneElementStartsWith |= key_combination.startsWith(p_text);

      if (p_text == key_combination)
      {
        return i;
      }
    }

    return -1;
  }

  //Sets matching key count for all elements
  void SelectableElementRepository::UpdateMatchingKeyCount(const QString& p_currentKeyCombination)
  {
    for (SelectableElementPtr& ui_element : m_elements)
    {
      int matchingLetterCount = 0;
      QString elementKeyCombination = ui_element->GetKeyCombination();

      Q_ASSERT(p_currentKeyCombination.size() <= elementKeyCombination.size());

      for (int i = 0; i < p_currentKeyCombination.size(); i++)
      {
        if (i >= elementKeyCombination.size())
        {
          break;
        }
        const QChar pressKey = p_currentKeyCombination.at(i);
        const QChar elementKey = elementKeyCombination.at(i);
        if (pressKey != elementKey)
        {
          break;
        }

        matchingLetterCount++;
      }

      ui_element->SetMatchingKeyCount(matchingLetterCount);
    }
  }
}
