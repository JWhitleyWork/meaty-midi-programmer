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
  border_panel->setSize("parent.innerwidth - 20", "parent.innerheight - 20");
  border_panel->getRenderer()->setBackgroundColor(tgui::Color::White);
  add(border_panel);

  // MIDI controls panel
  auto midi_panel = tgui::Panel::create();
  midi_panel->setPosition(2, 2);
  midi_panel->setSize("parent.innerwidth - 4", "parent.innerheight - 80");
  midi_panel->getRenderer()->setBackgroundColor(tgui::Color::Red);
  border_panel->add(midi_panel);

  auto left_controls_group = tgui::Group::create();
  left_controls_group->setPosition(0, 0);
  left_controls_group->setSize("50%", "100%");
  midi_panel->add(left_controls_group);

  // Left slider
  auto slider_s1 = tgui::Slider::create();
  slider_s1->setOrientation(tgui::Orientation::Vertical);
  slider_s1->setPosition(15, "10%");
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
  left_buttons_panel->setPosition(
    tgui::bindRight(left_controls_group), tgui::bindBottom(left_controls_group) - 15);
  left_buttons_panel->setSize("85%", "44%");
  left_buttons_panel->setOrigin(1.0f, 1.0f);
  left_buttons_panel->getRenderer()->setBackgroundColor(tgui::Color::Red);
  left_controls_group->add(left_buttons_panel);

  auto left_white_buttons_layout = tgui::HorizontalLayout::create();
  left_white_buttons_layout->setPosition(0, "parent.innerheight");
  left_white_buttons_layout->setSize("100%", 50);
  left_white_buttons_layout->setOrigin(0.0f, 1.0f);
  left_buttons_panel->add(left_white_buttons_layout);

  auto button_w1 = tgui::Button::create();
  button_w1->setSize(40, 40);
  button_w1->getRenderer()->setBackgroundColor(tgui::Color::White);
  button_w1->getRenderer()->setBackgroundColorHover(tgui::Color::Black);
  button_w1->getRenderer()->setTextColor(tgui::Color::Black);
  button_w1->getRenderer()->setTextColorHover(tgui::Color::White);
  button_w1->getRenderer()->setBorderColor(tgui::Color::Black);

  button_w1->setText("W1");
  auto button_w2 = tgui::Button::copy(button_w1);
  button_w2->setText("W2");
  auto button_w3 = tgui::Button::copy(button_w1);
  button_w3->setText("W3");
  auto button_w4 = tgui::Button::copy(button_w1);
  button_w4->setText("W4");
  auto button_w5 = tgui::Button::copy(button_w1);
  button_w5->setText("W5");
  auto button_w6 = tgui::Button::copy(button_w1);
  button_w6->setText("W6");
  auto button_w7 = tgui::Button::copy(button_w1);
  button_w7->setText("W7");

  float inter_button_spacing = 0.005f;

  left_white_buttons_layout->add(button_w1);
  left_white_buttons_layout->addSpace(inter_button_spacing);
  left_white_buttons_layout->add(button_w2);
  left_white_buttons_layout->addSpace(inter_button_spacing);
  left_white_buttons_layout->add(button_w3);
  left_white_buttons_layout->addSpace(inter_button_spacing);
  left_white_buttons_layout->add(button_w4);
  left_white_buttons_layout->addSpace(inter_button_spacing);
  left_white_buttons_layout->add(button_w5);
  left_white_buttons_layout->addSpace(inter_button_spacing);
  left_white_buttons_layout->add(button_w6);
  left_white_buttons_layout->addSpace(inter_button_spacing);
  left_white_buttons_layout->add(button_w7);

  auto left_black_buttons_layout = tgui::HorizontalLayout::create();
  left_black_buttons_layout->setPosition(0, "parent.innerheight - 51");
  left_black_buttons_layout->setSize("100%", 50);
  left_black_buttons_layout->setOrigin(0.0f, 1.0f);
  left_buttons_panel->add(left_black_buttons_layout);

  auto button_b1 = tgui::Button::copy(button_w1);
  button_b1->setText("B1");

  button_b1->getRenderer()->setTextColor(tgui::Color::White);
  button_b1->getRenderer()->setTextColorHover(tgui::Color::Black);
  button_b1->getRenderer()->setBackgroundColor(tgui::Color::Black);
  button_b1->getRenderer()->setBackgroundColorHover(tgui::Color::White);
  button_b1->getRenderer()->setBorderColor(tgui::Color::White);

  auto button_b2 = tgui::Button::copy(button_b1);
  button_b2->setText("B2");
  auto button_b3 = tgui::Button::copy(button_b1);
  button_b3->setText("B3");
  auto button_b4 = tgui::Button::copy(button_b1);
  button_b4->setText("B4");
  auto button_b5 = tgui::Button::copy(button_b1);
  button_b5->setText("B5");

  left_black_buttons_layout->addSpace(0.5f);
  left_black_buttons_layout->add(button_b1);
  left_black_buttons_layout->addSpace(inter_button_spacing);
  left_black_buttons_layout->add(button_b2);
  left_black_buttons_layout->addSpace(1.0f);
  left_black_buttons_layout->add(button_b3);
  left_black_buttons_layout->addSpace(inter_button_spacing);
  left_black_buttons_layout->add(button_b4);
  left_black_buttons_layout->addSpace(inter_button_spacing);
  left_black_buttons_layout->add(button_b5);
  left_black_buttons_layout->addSpace(inter_button_spacing);
  left_black_buttons_layout->addSpace(0.5f);

  // Right controls group
  auto right_controls_group = tgui::Group::create();
  right_controls_group->setPosition("50%", 0);
  right_controls_group->setSize("50%", "100%");
  midi_panel->add(right_controls_group);

  auto steak_image = tgui::Picture::create("../images/steak_400x400.svg");
  steak_image->setSize(90, 90);
  steak_image->setPosition(5, 10);
  steak_image->setOrigin(0.0f, 0.0f);
  right_controls_group->add(steak_image);

  // Right knobs panel
  auto right_knobs_panel = tgui::Panel::create();
  right_knobs_panel->setPosition("parent.innerwidth - 15", 15);
  right_knobs_panel->setSize("13%", "60%");
  right_knobs_panel->setOrigin(1.0f, 0.0f);
  right_knobs_panel->getRenderer()->setBackgroundColor(tgui::Color::White);
  right_controls_group->add(right_knobs_panel);

  right_knobs_panel->getRenderer()->setRoundedBorderRadius(right_knobs_panel->getFullSize().y / 2.0f);

  // Right buttons panel
  auto right_buttons_panel = tgui::Panel::create();
  right_buttons_panel->setPosition("parent.innerwidth - 15", "parent.innerheight - 15");
  right_buttons_panel->setSize("100% - 15", "44%");
  right_buttons_panel->setOrigin(1.0f, 1.0f);
  right_buttons_panel->getRenderer()->setBackgroundColor(tgui::Color::Red);
  right_controls_group->add(right_buttons_panel);

  auto right_white_buttons_layout = tgui::HorizontalLayout::create();
  right_white_buttons_layout->setPosition(0, "parent.innerheight");
  right_white_buttons_layout->setSize("100%", 50);
  right_white_buttons_layout->setOrigin(0.0f, 1.0f);
  right_buttons_panel->add(right_white_buttons_layout);

  auto button_w8 = tgui::Button::copy(button_w1);
  button_w8->setText("W8");
  auto button_w9 = tgui::Button::copy(button_w1);
  button_w9->setText("W9");
  auto button_w10 = tgui::Button::copy(button_w1);
  button_w10->setText("W10");
  auto button_w11 = tgui::Button::copy(button_w1);
  button_w11->setText("W11");
  auto button_w12 = tgui::Button::copy(button_w1);
  button_w12->setText("W12");
  auto button_w13 = tgui::Button::copy(button_w1);
  button_w13->setText("W13");
  auto button_w14 = tgui::Button::copy(button_w1);
  button_w14->setText("W14");
  auto button_w15 = tgui::Button::copy(button_w1);
  button_w15->setText("W15");

  right_white_buttons_layout->addSpace(inter_button_spacing);
  right_white_buttons_layout->add(button_w8);
  right_white_buttons_layout->addSpace(inter_button_spacing);
  right_white_buttons_layout->add(button_w9);
  right_white_buttons_layout->addSpace(inter_button_spacing);
  right_white_buttons_layout->add(button_w10);
  right_white_buttons_layout->addSpace(inter_button_spacing);
  right_white_buttons_layout->add(button_w11);
  right_white_buttons_layout->addSpace(inter_button_spacing);
  right_white_buttons_layout->add(button_w12);
  right_white_buttons_layout->addSpace(inter_button_spacing);
  right_white_buttons_layout->add(button_w13);
  right_white_buttons_layout->addSpace(inter_button_spacing);
  right_white_buttons_layout->add(button_w14);
  right_white_buttons_layout->addSpace(inter_button_spacing);
  right_white_buttons_layout->add(button_w15);

  auto right_black_buttons_layout = tgui::HorizontalLayout::create();
  right_black_buttons_layout->setPosition(0, "parent.innerheight - 51");
  right_black_buttons_layout->setSize("100%", 50);
  right_black_buttons_layout->setOrigin(0.0f, 1.0f);
  right_buttons_panel->add(right_black_buttons_layout);

  auto button_b6 = tgui::Button::copy(button_b1);
  button_b6->setText("B6");
  auto button_b7 = tgui::Button::copy(button_b1);
  button_b7->setText("B7");
  auto button_b8 = tgui::Button::copy(button_b1);
  button_b8->setText("B8");
  auto button_b9 = tgui::Button::copy(button_b1);
  button_b9->setText("B9");
  auto button_b10 = tgui::Button::copy(button_b1);
  button_b10->setText("B10");

  right_black_buttons_layout->addSpace(0.5f);
  right_black_buttons_layout->add(button_b6);
  right_black_buttons_layout->addSpace(inter_button_spacing);
  right_black_buttons_layout->add(button_b7);
  right_black_buttons_layout->addSpace(1.0f);
  right_black_buttons_layout->add(button_b8);
  right_black_buttons_layout->addSpace(inter_button_spacing);
  right_black_buttons_layout->add(button_b9);
  right_black_buttons_layout->addSpace(inter_button_spacing);
  right_black_buttons_layout->add(button_b10);
  right_black_buttons_layout->addSpace(inter_button_spacing);
  right_black_buttons_layout->addSpace(1.5f);

  // Bottom controls panel
  auto controls_panel = tgui::Panel::create();
  controls_panel->setPosition(2, "parent.innerheight - 76");
  controls_panel->setSize("parent.innerwidth - 4", 74);
  controls_panel->getRenderer()->setBackgroundColor(tgui::Color::Red);
  border_panel->add(controls_panel);
}

}  // namespace MeatyMidiEditor
