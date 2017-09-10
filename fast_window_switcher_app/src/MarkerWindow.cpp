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

#include <cmath>

#include <QtGlobal>
#include <QtWidgets>
#include <QIcon>
#include <QFont>
#include <QFontMetrics>

#include "MarkerWindow.hpp"
#include "WindowFunctions.hpp"
#include "MarkerFunctions.hpp"
#include "PanelFunctions.hpp"
#include "SelectableElement.hpp"

namespace FastWindowSwitcher
{
  const int HIDDEN_POS = -20000;

  //Transparent maximized windows which shows the marker rectangles. Windows is move the visible area after the hotkey is press. Each Monitor has a seperate window
  MarkerWindow::MarkerWindow(const size_t& p_screen, FastWindowSwitcherLib::SelectableElementRepository& p_elementRespository,
    const QRect& p_windowRect, const QFont& p_markerFont, const QFontMetrics& p_markerFontMetrics)
    :m_screen(p_screen),
    m_elementRespository(p_elementRespository),
    m_windowRect(p_windowRect),
    m_markerFont(p_markerFont),
    m_markerFontMetrics(p_markerFontMetrics),
    m_rectanglePen(QColor(80, 102, 124), 2), //FIXME from settings
    m_rectangleBrush(QColor(57, 75, 93)),
    m_shakingOffsetX(0)
  {
    setAutoFillBackground(false);

    this->setFixedHeight(p_windowRect.height());
    this->setFixedWidth(p_windowRect.width());

    move(HIDDEN_POS, HIDDEN_POS);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
  }

  //Udate windows position and markers 
  void MarkerWindow::Update(QString p_pressedKeyCombination, int p_shakingOffsetX)
  {
    m_currentKeyCombination = p_pressedKeyCombination;
    m_shakingOffsetX = p_shakingOffsetX;

    //Blocking redraw  to update the window content before moving into the visible area
    repaint();

    //Move the window to the visible area
    if (x() == HIDDEN_POS)
    {
      move(m_windowRect.x(), m_windowRect.y());
      raise();
    }
  }

  //Move windows out of the visible area
  void MarkerWindow::Hide()
  {
    move(HIDDEN_POS, HIDDEN_POS);
    m_shakingOffsetX = 0;
  }

  //Paint all markers
  void MarkerWindow::paintEvent(QPaintEvent* /*p_event*/)
  {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.setFont(m_markerFont);
    painter.setBrush(m_rectangleBrush);

    for (std::size_t i = 0; i < m_elementRespository.Count(); i++)
    {
      const FastWindowSwitcherLib::SelectableElement& ui_element = m_elementRespository.GetElement(i);
      if (ui_element.IsDisabled())
      {
        continue;
      }

      if (ui_element.GetMonitor() != m_screen)
      {
        continue;
      }

      DrawElementAnnotations(ui_element, painter);
    }
  }

  //Paint marker per element
  void MarkerWindow::DrawElementAnnotations(const FastWindowSwitcherLib::SelectableElement& p_selectaleElement, QPainter &p_painter)
  {
    QString key_combination = p_selectaleElement.GetKeyCombination();
    QRect key_combination_rect = m_markerFontMetrics.boundingRect(key_combination);

    for (const QRect& text_rect : p_selectaleElement.GetMarkers())
    {
      DrawAnnotation(p_selectaleElement, text_rect, p_painter, key_combination_rect, key_combination);
    }
  }

  //Paint single marker
  void MarkerWindow::DrawAnnotation(const FastWindowSwitcherLib::SelectableElement& p_selectableElement, const QRect& p_textRect, QPainter& p_painter, const QRect& p_keyCombinationRect, const QString &p_keyCombination)
  {
    const int matchingChars = p_selectableElement.GetMatchingKeyCount();

    QRect text_rect_relative = QRect(p_textRect.left() - m_windowRect.left(), p_textRect.top() - m_windowRect.top(), p_textRect.width(), p_textRect.height());

    const QRect text_rect_shaked = text_rect_relative.translated(m_shakingOffsetX, 0);

    p_painter.setPen(m_rectanglePen);
    p_painter.drawRoundedRect(text_rect_shaked, 5, 5);

    double x_pos = (double)text_rect_shaked.center().rx() - (((double)p_keyCombinationRect.width() / 2));

    const double y_pos = (double)text_rect_shaked.center().ry() + (((double)p_keyCombinationRect.height() / 2) - 1) - (m_markerFontMetrics.descent() - 1);

    for (int i = 0; i < p_keyCombination.size(); i++)
    {

      //Paint matching key red
      if (i < matchingChars)
      {
        p_painter.setPen(Qt::red);
      }
      else
      {
        p_painter.setPen(Qt::white);
      }

      QString letter = p_keyCombination.mid(i, 1);
      p_painter.drawText(std::round(x_pos), std::round(y_pos), letter);
      x_pos += (m_markerFontMetrics.width(letter) - 1);
    }
  }
}