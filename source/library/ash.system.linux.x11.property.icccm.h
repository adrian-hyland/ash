#pragma once

#include <xcb/xcb.h>
#include "ash.utf8.h"
#include "ash.integer.bit.h"
#include "ash.system.linux.x11.icccm.h"
#include "ash.system.linux.x11.property.h"
#include "ash.system.linux.x11.atom.registry.h"


namespace Ash
{
	namespace System
	{
		namespace Linux
		{
			namespace X11
			{
				namespace Property
				{
					// Ref: ICCCM (Inter-Client Communication Conventions Manual) - see https://x.org/releases/X11R7.6/doc/xorg-docs/specs/ICCCM/icccm.html#introduction)
					namespace ICCCM
					{
						namespace Data
						{
							class Hints : public Ash::System::Linux::X11::Generic::Property::Data
							{
							public:
								using Format = Ash::System::Linux::X11::Property::Data::Word<32>::Format;

								using Content = Ash::Memory::Sequence<Format, 9>;

								using View = Ash::Memory::View<Format>;

								static constexpr size_t format(Content &content, size_t offset, const Ash::System::Linux::X11::ICCCM::Hints &value)
								{
									Ash::System::Linux::X11::ICCCM::Hints::Flag flag;
									bool input;
									Ash::System::Linux::X11::ICCCM::State initialState;
									xcb_pixmap_t iconPixmap;
									xcb_window_t iconWindow;
									Ash::UI::Position iconPosition;
									xcb_pixmap_t iconMask;
									xcb_window_t windowGroup;

									value.getFields(flag, input, initialState, iconPixmap, iconWindow, iconPosition, iconMask, windowGroup);

									return Ash::System::Linux::X11::Property::Data::Word<32>::format(content, offset, flag, input, initialState, iconPixmap, iconWindow, iconPosition.x, iconPosition.y, iconMask, windowGroup);
								}
	
								static constexpr size_t parse(View content, size_t offset, Ash::System::Linux::X11::ICCCM::Hints &value)
								{
									Ash::System::Linux::X11::ICCCM::Hints::Flag flag;
									bool input;
									Ash::System::Linux::X11::ICCCM::State initialState;
									xcb_pixmap_t iconPixmap;
									xcb_window_t iconWindow;
									Ash::UI::Position iconPosition;
									xcb_pixmap_t iconMask;
									xcb_window_t windowGroup;

									offset = Ash::System::Linux::X11::Property::Data::Word<32>::parse(content, offset, flag, input, initialState, iconPixmap, iconWindow, iconPosition.x, iconPosition.y, iconMask, windowGroup);
									if (offset != 0)
									{
										value.setFields(flag, input, initialState, iconPixmap, iconWindow, iconPosition, iconMask, windowGroup);
									}
									return offset;
								}
							};	

							class SizeHints : public Ash::System::Linux::X11::Generic::Property::Data
							{
							public:
								using Format = Ash::System::Linux::X11::Property::Data::Word<32>::Format;

								using Content = Ash::Memory::Sequence<Format, 18>;

								using View = Ash::Memory::View<Format>;

								static constexpr size_t format(Content &content, size_t offset, const Ash::System::Linux::X11::ICCCM::SizeHints &value)
								{
									Ash::System::Linux::X11::ICCCM::SizeHints::Flag flag;
									Ash::UI::Size minSize;
									Ash::UI::Size maxSize;
									Ash::UI::Size resizeIncrement;
									Ash::UI::Size minAspectRatio;
									Ash::UI::Size maxAspectRatio;
									Ash::UI::Size baseSize;
									xcb_gravity_t gravity;

									value.getFields(flag, minSize, maxSize, resizeIncrement, minAspectRatio, maxAspectRatio, baseSize, gravity);

									return Ash::System::Linux::X11::Property::Data::Word<32>::format(content, offset, flag, 0, 0, 0, 0,
									                                                                                        minSize.width,         minSize.height,
									                                                                                        maxSize.width,         maxSize.height,
									                                                                                        resizeIncrement.width, resizeIncrement.height,
									                                                                                        minAspectRatio.width,  minAspectRatio.height,
									                                                                                        maxAspectRatio.width,  maxAspectRatio.height,
									                                                                                        baseSize.width,        baseSize.height,
									                                                                                        gravity);
								}
	
								static constexpr size_t parse(View content, size_t offset, Ash::System::Linux::X11::ICCCM::SizeHints &value)
								{
									Ash::System::Linux::X11::ICCCM::SizeHints::Flag flag;
									uint32_t pad;
									Ash::UI::Size minSize;
									Ash::UI::Size maxSize;
									Ash::UI::Size resizeIncrement;
									Ash::UI::Size minAspectRatio;
									Ash::UI::Size maxAspectRatio;
									Ash::UI::Size baseSize;
									xcb_gravity_t gravity;

									offset = Ash::System::Linux::X11::Property::Data::Word<32>::parse(content, offset, flag, pad, pad, pad, pad, 
									                                                                                         minSize.width,         minSize.height,
									                                                                                         maxSize.width,         maxSize.height,
									                                                                                         resizeIncrement.width, resizeIncrement.height,
									                                                                                         minAspectRatio.width,  minAspectRatio.height,
									                                                                                         maxAspectRatio.width,  maxAspectRatio.height,
									                                                                                         baseSize.width,        baseSize.height,
									                                                                                         gravity);
									if (offset != 0)
									{
										value.setFields(flag, minSize, maxSize, resizeIncrement, minAspectRatio, maxAspectRatio, baseSize, gravity);
									}
									return offset;
								}
							};	
						}

						using Name = Ash::System::Linux::X11::Property::Utf8String<Ash::System::Linux::X11::Atom::ICCCM::Name>;

						using IconName = Ash::System::Linux::X11::Property::Utf8String<Ash::System::Linux::X11::Atom::ICCCM::IconName>;

						using ClientMachine = Ash::System::Linux::X11::Property::Utf8String<Ash::System::Linux::X11::Atom::ICCCM::ClientMachine>;

						using Hints = Ash::System::Linux::X11::Property::Value<Ash::System::Linux::X11::Atom::ICCCM::Hints, Ash::System::Linux::X11::Atom::ICCCM::Hints, Ash::System::Linux::X11::Property::ICCCM::Data::Hints>;

						using NormalHints = Ash::System::Linux::X11::Property::Value<Ash::System::Linux::X11::Atom::ICCCM::NormalHints, Ash::System::Linux::X11::Atom::ICCCM::SizeHints, Ash::System::Linux::X11::Property::ICCCM::Data::SizeHints>;

						using InstanceClass = Ash::System::Linux::X11::Property::Utf8StringArray<Ash::System::Linux::X11::Atom::ICCCM::InstanceClass>;

						using TransientFor = Ash::System::Linux::X11::Property::Window<Ash::System::Linux::X11::Atom::ICCCM::TransientFor>;

						using ColormapWindows = Ash::System::Linux::X11::Property::WindowArray<Ash::System::Linux::X11::Atom::ICCCM::ColormapWindows>;

						using Protocols = Ash::System::Linux::X11::Property::AtomArray<Ash::System::Linux::X11::Atom::ICCCM::Protocols>;
					}
				}
			}
		}
	}
}
