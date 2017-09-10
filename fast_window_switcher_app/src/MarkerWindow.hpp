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

#pragma  once

#include <QList>
#include <QPen>
#include <QBrush>
#include <QWidget>

class QRect;

#include "SelectableElementPtr.hpp"
#include "SelectableElementRepository.hpp"

namespace FastWindowSwitcher
{
  class MarkerWindow : public QWidget
  {
    Q_OBJECT

  public:
    MarkerWindow(const std::size_t& p_screen, FastWindowSwitcherLib::SelectableElementRepository& p_elementRespository, const QRect& p_windowRect, const QFont& p_markerFont, const QFontMetrics& p_fontMetrics);
    void paintEvent(QPaintEvent* event) override;

    void Hide();
    void Update(QString pressedKeyCombination, int shakingOffsetX);

  private:
    void DrawElementAnnotations(const FastWindowSwitcherLib::SelectableElement& ui_element, QPainter &painter);
    void DrawAnnotation(const FastWindowSwitcherLib::SelectableElement& ui_element, const QRect& text_rect, QPainter& painter, const QRect& key_combination_rect, const QString& key_combination);

    FastWindowSwitcherLib::SelectableElementRepository& m_elementRespository;
    const QRect m_windowRect;
    const QFont m_markerFont;
    const QFontMetrics m_markerFontMetrics;
    const QPen m_rectanglePen;
    const QBrush m_rectangleBrush;
    QString m_currentKeyCombination;
    int m_shakingOffsetX;
    std::size_t m_screen;
  };

  using TransparentMarkerWindowPtr = std::unique_ptr<MarkerWindow>;
};