// Copyright 2025 Electrified Autonomy, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the “Software”), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "midi_editor.hpp"

namespace MeatyMidiEditor
{

MidiEditor::MidiEditor(sf::RenderWindow & window)
: tgui::Gui{window}
{
  build_midi_gui();
}

void MidiEditor::mainLoop()
{
  auto window = dynamic_cast<sf::RenderWindow *>(getWindow());

  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      tgui::Gui::handleEvent(event);

      if (event.type == sf::Event::Closed) {
        window->close();
        return;
      } else if (event.type == sf::Event::Resized) {
        continue;
      }
    }

    window->clear(tgui::Color::Red);
    draw();
    window->display();
  }
}

void MidiEditor::build_midi_gui()
{
  // Border panel to create white frame
  auto border_panel = tgui::Panel::create();
  border_panel->setPosition(10, 10);
  border_panel->setSize(880, 340);
  border_panel->getRenderer()->setBackgroundColor(tgui::Color::White);
  add(border_panel);

  // MIDI controls panel
  auto midi_panel = tgui::Panel::create();
  midi_panel->setPosition(2, 2);
  midi_panel->setSize(876, 236);
  midi_panel->getRenderer()->setBackgroundColor(tgui::Color::Red);
  border_panel->add(midi_panel);

  auto left_controls_group = tgui::Group::create();
  left_controls_group->setPosition(0, 0);
  left_controls_group->setSize("50%", "100%");
  midi_panel->add(left_controls_group);

  // Left slider
  auto slider_s1 = tgui::Slider::create();
  slider_s1->setOrientation(tgui::Orientation::Vertical);
  slider_s1->setPosition("3.5%", "10%");
  slider_s1->setSize("8%", "80%");
  slider_s1->setValue(5.0);
  slider_s1->setEnabled(false);
  left_controls_group->add(slider_s1);

  // Top knobs panel
  auto top_knobs_panel = tgui::Panel::create();
  top_knobs_panel->setPosition(tgui::bindRight(left_controls_group) - 20, tgui::bindTop(slider_s1));
  top_knobs_panel->setSize("77.5%", "25%");
  top_knobs_panel->setOrigin(1.0f, 0.0f);
  top_knobs_panel->getRenderer()->setBackgroundColor(tgui::Color::White);
  left_controls_group->add(top_knobs_panel);

  top_knobs_panel->getRenderer()->setRoundedBorderRadius(top_knobs_panel->getFullSize().y / 2.0f);

  auto knob_k1 = tgui::Knob::create();
  knob_k1->setPosition("10%", "50%");
  knob_k1->setSize(40, 40);
  knob_k1->setOrigin(0.5f, 0.5f);
  knob_k1->setEnabled(false);
  knob_k1->getRenderer()->setBackgroundColor(tgui::Color::Black);
  knob_k1->getRenderer()->setThumbColor(tgui::Color::Red);
  top_knobs_panel->add(knob_k1);

  auto knob_k2 = tgui::Knob::copy(knob_k1);
  knob_k2->setPosition("36.66%", "50%");
  top_knobs_panel->add(knob_k2);

  auto knob_k3 = tgui::Knob::copy(knob_k1);
  knob_k3->setPosition("63.33%", "50%");
  top_knobs_panel->add(knob_k3);

  auto knob_k4 = tgui::Knob::copy(knob_k1);
  knob_k4->setPosition("90%", "50%");
  top_knobs_panel->add(knob_k4);

  // Left buttons panel
  auto left_buttons_panel = tgui::Panel::create();
  left_buttons_panel->setPosition(tgui::bindRight(left_controls_group), tgui::bindBottom(slider_s1));
  left_buttons_panel->setSize("85%", "45%");
  left_buttons_panel->setOrigin(1.0f, 1.0f);
  left_buttons_panel->getRenderer()->setBackgroundColor(tgui::Color::White);
  left_controls_group->add(left_buttons_panel);

  // Right controls group
  auto right_controls_group = tgui::Group::create();
  right_controls_group->setPosition(tgui::bindRight(left_controls_group), 0);
  right_controls_group->setSize("50%", "100%");
  midi_panel->add(right_controls_group);

  // Bottom controls panel
  auto controls_panel = tgui::Panel::create();
  controls_panel->setPosition(2, 240);
  controls_panel->setSize(876, 96);
  controls_panel->getRenderer()->setBackgroundColor(tgui::Color::Red);
  border_panel->add(controls_panel);
}

}  // namespace MeatyMidiEditor
