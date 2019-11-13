#include "InteractConfig.h"
#include "Ch.h"
#include "Item.h"
#include <SlateApplication.h>

#define LOAD_BUTTON_TEXTURE(texture) return LoadObject<UTexture2D>(NULL, TEXT(texture), NULL, LOAD_None, NULL);

EPlatform UInteractConfig::currentPlatform = EPlatform::PC;

EPlatform UInteractConfig::defaultPlatform = EPlatform::PC;

EPlatform UInteractConfig::defaultGamepadPlatform = EPlatform::Xbox;

void UInteractConfig::checkCurrentPlatform()
{
	auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
	if (genericApplication.Get() != nullptr && genericApplication->IsGamepadAttached())
	{
		UInteractConfig::currentPlatform = UInteractConfig::defaultGamepadPlatform;
	}
	else {
		UInteractConfig::currentPlatform = UInteractConfig::defaultPlatform;

	}
}

UTexture2D* UInteractConfig::GetActionButtonImage(EAction actionButton)
{
	if (EPlatform::PC == currentPlatform)
	{
		if (actionButton == EAction::Auto1 || actionButton == EAction::Auto2 || actionButton == EAction::Auto3 || actionButton == EAction::Auto4 || actionButton == EAction::Auto5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/buttonAuto.buttonAuto'");
		}
		else if (actionButton == EAction::LMB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/mouse_left_button.mouse_left_button'");
		}
		else if (actionButton == EAction::RMB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/mouse_right_button.mouse_right_button'");
		}
		else if (actionButton == EAction::SHIFT) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/shift.shift'");
		}
		else if (actionButton == EAction::TAB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/back.back'");
		}
		else if (actionButton == EAction::ESC) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/esc.esc'");
		}
		else if (actionButton == EAction::BottomArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/down.down'");
		}
		else if (actionButton == EAction::LeftArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/left.left'");
		}
		else if (actionButton == EAction::TopArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/up.up'");
		}
		else if (actionButton == EAction::RightArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/right.right'");
		}
		else if (actionButton == EAction::Q) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/q.q'");
		}
		else if (actionButton == EAction::W) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/w.w'");
		}
		else if (actionButton == EAction::E) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/e.e'");
		}
		else if (actionButton == EAction::R) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/r.r'");
		}
		else if (actionButton == EAction::T) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/t.t'");
		}
		else if (actionButton == EAction::Y) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/y.y'");
		}
		else if (actionButton == EAction::U) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/u.u'");
		}
		else if (actionButton == EAction::I) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/i.i'");
		}
		else if (actionButton == EAction::O) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/o.o'");
		}
		else if (actionButton == EAction::P) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/p.p'");
		}
		else if (actionButton == EAction::A) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/a.a'");
		}
		else if (actionButton == EAction::S) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/s.s'");
		}
		else if (actionButton == EAction::D) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/d.d'");
		}
		else if (actionButton == EAction::F) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f.f'");
		}
		else if (actionButton == EAction::G) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/g.g'");
		}
		else if (actionButton == EAction::H) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/h.h'");
		}
		else if (actionButton == EAction::J) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/j.j'");
		}
		else if (actionButton == EAction::K) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/k.k'");
		}
		else if (actionButton == EAction::L) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/l.l'");
		}
		else if (actionButton == EAction::Z) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/z.z'");
		}
		else if (actionButton == EAction::X) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/x.x'");
		}
		else if (actionButton == EAction::C) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/c.c'");
		}
		else if (actionButton == EAction::V) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/v.v'");
		}
		else if (actionButton == EAction::B) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/b.b'");
		}
		else if (actionButton == EAction::N) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/n.n'");
		}
		else if (actionButton == EAction::M) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/m.m'");
		}
		else if (actionButton == EAction::NUM1) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/1.1'");
		}
		else if (actionButton == EAction::NUM2) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/2.2'");
		}
		else if (actionButton == EAction::NUM3) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/3.3'");
		}
		else if (actionButton == EAction::NUM4) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/4.4'");
		}
		else if (actionButton == EAction::NUM5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/5.5'");
		}
		else if (actionButton == EAction::NUM6) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/6.6'");
		}
		else if (actionButton == EAction::NUM7) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/7.7'");
		}
		else if (actionButton == EAction::NUM8) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/8.8'");
		}
		else if (actionButton == EAction::NUM9) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/9.9'");
		}
		else if (actionButton == EAction::NUM0) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/0.0'");
		}
		else if (actionButton == EAction::F1) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f1.f1'");
		}
		else if (actionButton == EAction::F2) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f2.f2'");
		}
		else if (actionButton == EAction::F3) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f3.f3'");
		}
		else if (actionButton == EAction::F4) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f4.f4'");
		}
		else if (actionButton == EAction::F5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f5.f5'");
		}
		else if (actionButton == EAction::F6) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f6.f6'");
		}
		else if (actionButton == EAction::F7) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f7.f7'");
		}
		else if (actionButton == EAction::F8) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f8.f8'");
		}
		else if (actionButton == EAction::F9) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f9.f9'");
		}
		else if (actionButton == EAction::F10) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f10.f10'");
		}
		else if (actionButton == EAction::F11) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f11.f11'");
		}
		else if (actionButton == EAction::F12) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f12.f12'");
		}
		else if (actionButton == EAction::None) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/full.full'");
		}
	}
	else if (EPlatform::Xbox == currentPlatform)
	{
		if (actionButton == EAction::Auto1 || actionButton == EAction::Auto2 || actionButton == EAction::Auto3 || actionButton == EAction::Auto4 || actionButton == EAction::Auto5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/buttonAuto.buttonAuto'");
		}
		else if (actionButton == EAction::LMB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/y.y'");
		}
		else if (actionButton == EAction::RMB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/mouse_right_button.mouse_right_button'");
		}
		else if (actionButton == EAction::SHIFT) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/left_bumper.left_bumper'");
		}
		else if (actionButton == EAction::TAB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/tab.tab'");
		}
		else if (actionButton == EAction::ESC) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/view_back.view_back'");
		}
		else if (actionButton == EAction::BottomArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/dpad_down.dpad_down'");
		}
		else if (actionButton == EAction::LeftArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/dpad_left.dpad_left'");
		}
		else if (actionButton == EAction::TopArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/dpad_up.dpad_up'");
		}
		else if (actionButton == EAction::RightArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/dpad_right.dpad_right'");
		}
		else if (actionButton == EAction::Q) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/left_trigger.left_trigger'");
		}
		else if (actionButton == EAction::W) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/w.w'");
		}
		else if (actionButton == EAction::E) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/a.a'");
		}
		else if (actionButton == EAction::R) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/x.x'");
		}
		else if (actionButton == EAction::T) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/right_trigger.right_trigger'");
		}
		else if (actionButton == EAction::Y) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/y.y'");
		}
		else if (actionButton == EAction::U) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/u.u'");
		}
		else if (actionButton == EAction::I) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/menu_start.menu_start'");
		}
		else if (actionButton == EAction::O) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/o.o'");
		}
		else if (actionButton == EAction::P) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/p.p'");
		}
		else if (actionButton == EAction::A) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/a.a'");
		}
		else if (actionButton == EAction::S) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/s.s'");
		}
		else if (actionButton == EAction::D) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/d.d'");
		}
		else if (actionButton == EAction::F) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/right_bumper.right_bumper'");
		}
		else if (actionButton == EAction::G) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/right_stick.right_stick'");
		}
		else if (actionButton == EAction::H) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/left_stick.left_stick'");
		}
		else if (actionButton == EAction::J) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/j.j'");
		}
		else if (actionButton == EAction::K) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/k.k'");
		}
		else if (actionButton == EAction::L) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/l.l'");
		}
		else if (actionButton == EAction::Z) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/z.z'");
		}
		else if (actionButton == EAction::X) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/x.x'");
		}
		else if (actionButton == EAction::C) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Xbox/b.b'");
		}
		else if (actionButton == EAction::V) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/v.v'");
		}
		else if (actionButton == EAction::B) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/b.b'");
		}
		else if (actionButton == EAction::N) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/n.n'");
		}
		else if (actionButton == EAction::M) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/m.m'");
		}
		else if (actionButton == EAction::NUM1) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/1.1'");
		}
		else if (actionButton == EAction::NUM2) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/2.2'");
		}
		else if (actionButton == EAction::NUM3) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/3.3'");
		}
		else if (actionButton == EAction::NUM4) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/4.4'");
		}
		else if (actionButton == EAction::NUM5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/5.5'");
		}
		else if (actionButton == EAction::NUM6) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/6.6'");
		}
		else if (actionButton == EAction::NUM7) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/7.7'");
		}
		else if (actionButton == EAction::NUM8) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/8.8'");
		}
		else if (actionButton == EAction::NUM9) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/9.9'");
		}
		else if (actionButton == EAction::NUM0) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/0.0'");
		}
		else if (actionButton == EAction::F1) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f1.f1'");
		}
		else if (actionButton == EAction::F2) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f2.f2'");
		}
		else if (actionButton == EAction::F3) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f3.f3'");
		}
		else if (actionButton == EAction::F4) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f4.f4'");
		}
		else if (actionButton == EAction::F5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f5.f5'");
		}
		else if (actionButton == EAction::F6) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f6.f6'");
		}
		else if (actionButton == EAction::F7) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f7.f7'");
		}
		else if (actionButton == EAction::F8) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f8.f8'");
		}
		else if (actionButton == EAction::F9) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f9.f9'");
		}
		else if (actionButton == EAction::F10) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f10.f10'");
		}
		else if (actionButton == EAction::F11) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f11.f11'");
		}
		else if (actionButton == EAction::F12) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f12.f12'");
		}
		else if (actionButton == EAction::None) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/full.full'");
		}
	}
	else if (EPlatform::Play == currentPlatform)
	{
		if (actionButton == EAction::Auto1 || actionButton == EAction::Auto2 || actionButton == EAction::Auto3 || actionButton == EAction::Auto4 || actionButton == EAction::Auto5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/buttonAuto.buttonAuto'");
		}
		else if (actionButton == EAction::LMB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/triangle.triangle'");
		}
		else if (actionButton == EAction::RMB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/mouse_right_button.mouse_right_button'");
		}
		else if (actionButton == EAction::SHIFT) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/l1.l1'");
		}
		else if (actionButton == EAction::TAB) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/tab.tab'");
		}
		else if (actionButton == EAction::ESC) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/share.share'");
		}
		else if (actionButton == EAction::BottomArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/dpad_down.dpad_down'");
		}
		else if (actionButton == EAction::LeftArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/dpad_left.dpad_left'");
		}
		else if (actionButton == EAction::TopArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/dpad_up.dpad_up'");
		}
		else if (actionButton == EAction::RightArrow) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/dpad_right.dpad_right'");
		}
		else if (actionButton == EAction::Q) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/l2.l2'");
		}
		else if (actionButton == EAction::W) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/w.w'");
		}
		else if (actionButton == EAction::E) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/cross.cross'");
		}
		else if (actionButton == EAction::R) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/square.square'");
		}
		else if (actionButton == EAction::T) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/r2.r2'");
		}
		else if (actionButton == EAction::Y) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/y.y'");
		}
		else if (actionButton == EAction::U) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/u.u'");
		}
		else if (actionButton == EAction::I) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/options.options'");
		}
		else if (actionButton == EAction::O) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/o.o'");
		}
		else if (actionButton == EAction::P) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/p.p'");
		}
		else if (actionButton == EAction::A) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/a.a'");
		}
		else if (actionButton == EAction::S) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/s.s'");
		}
		else if (actionButton == EAction::D) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/d.d'");
		}
		else if (actionButton == EAction::F) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/r1.r1'");
		}
		else if (actionButton == EAction::G) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/right_stick.right_stick'");
		}
		else if (actionButton == EAction::H) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/left_stick.left_stick'");
		}
		else if (actionButton == EAction::J) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/j.j'");
		}
		else if (actionButton == EAction::K) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/k.k'");
		}
		else if (actionButton == EAction::L) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/l.l'");
		}
		else if (actionButton == EAction::Z) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/z.z'");
		}
		else if (actionButton == EAction::X) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/x.x'");
		}
		else if (actionButton == EAction::C) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/Play_Station/circle.circle'");
		}
		else if (actionButton == EAction::V) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/v.v'");
		}
		else if (actionButton == EAction::B) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/b.b'");
		}
		else if (actionButton == EAction::N) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/n.n'");
		}
		else if (actionButton == EAction::M) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/m.m'");
		}
		else if (actionButton == EAction::NUM1) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/1.1'");
		}
		else if (actionButton == EAction::NUM2) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/2.2'");
		}
		else if (actionButton == EAction::NUM3) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/3.3'");
		}
		else if (actionButton == EAction::NUM4) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/4.4'");
		}
		else if (actionButton == EAction::NUM5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/5.5'");
		}
		else if (actionButton == EAction::NUM6) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/6.6'");
		}
		else if (actionButton == EAction::NUM7) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/7.7'");
		}
		else if (actionButton == EAction::NUM8) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/8.8'");
		}
		else if (actionButton == EAction::NUM9) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/9.9'");
		}
		else if (actionButton == EAction::NUM0) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/0.0'");
		}
		else if (actionButton == EAction::F1) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f1.f1'");
		}
		else if (actionButton == EAction::F2) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f2.f2'");
		}
		else if (actionButton == EAction::F3) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f3.f3'");
		}
		else if (actionButton == EAction::F4) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f4.f4'");
		}
		else if (actionButton == EAction::F5) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f5.f5'");
		}
		else if (actionButton == EAction::F6) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f6.f6'");
		}
		else if (actionButton == EAction::F7) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f7.f7'");
		}
		else if (actionButton == EAction::F8) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f8.f8'");
		}
		else if (actionButton == EAction::F9) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f9.f9'");
		}
		else if (actionButton == EAction::F10) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f10.f10'");
		}
		else if (actionButton == EAction::F11) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f11.f11'");
		}
		else if (actionButton == EAction::F12) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/Platforms/PC/f12.f12'");
		}
		else if (actionButton == EAction::None) {
			LOAD_BUTTON_TEXTURE("Texture2D'/Game/ISM/Icons/full.full'");
		}
	}

	return nullptr;
}

#if WITH_EDITOR
void FDataGetter::OnAssetAdded(const FAssetData& AssetData)
{
	if (AssetData.AssetClass == FName("Texture2D")) {
		textures2D.Add(AssetData.GetAsset());
	}
	else if (AssetData.AssetClass == FName("CurveFloat")) {
		curveFloat.Add(AssetData.GetAsset());
	}
	else if (AssetData.AssetClass == FName("AnimMontage")) {
		animMontage.Add(AssetData.GetAsset());
	}
	else if (AssetData.AssetClass == FName("ParticleSystem")) {
		particleSystem.Add(AssetData.GetAsset());
	}
	else if (AssetData.AssetClass == FName("LevelSequence")) {
		sequenceClasses.Add(AssetData.GetAsset());
	}
	else if (AssetData.AssetClass == FName("Material") || AssetData.AssetClass == FName("MaterialInstanceConstant")) {
		materials.Add(AssetData.GetAsset());
	}
	else if (AssetData.AssetClass == FName("SoundWave") || AssetData.AssetClass == FName("SoundCue")) {
		sounds.Add(AssetData.GetAsset());
	}
	if (actorClasses.Num() == 0) {
		for (TObjectIterator<UClass> It; It; ++It)
		{
			//TODO igual que en el cpp
			if (It->IsChildOf(ACh::StaticClass()) || It->IsChildOf(AItem::StaticClass()) || It->IsChildOf(AActor::StaticClass())
				|| It->IsChildOf(APawn::StaticClass()) || It->IsChildOf(ACharacter::StaticClass()))
			{
				actorClasses.Add(*It);
			}
			else if (It->IsChildOf(USceneComponent::StaticClass()))
			{
				componentClasses.Add(*It);
				actorComponentClasses.Add(*It);
			}
			else if (It->IsChildOf(UActorComponent::StaticClass()))
			{
				actorComponentClasses.Add(*It);
			}
			else if (It->IsChildOf(UUserWidget::StaticClass()))
			{
				widgets.Add(*It);
			}
			classes.Add(*It);
		}
	}
}

void FDataGetter::ReloadAssetData(UWorld* world){
	actorClasses.Empty();
	componentClasses.Empty();
	widgets.Empty();
	for (TObjectIterator<UClass> It; It; ++It)
	{
		//TODO aqui es necesario sino no encuentra los de esa clase
		if (It->IsChildOf(ACh::StaticClass()) || It->IsChildOf(AItem::StaticClass()) || It->IsChildOf(AActor::StaticClass())
			|| It->IsChildOf(APawn::StaticClass()) || It->IsChildOf(ACharacter::StaticClass()))
		{
			actorClasses.Add(*It);
		}
		else if (It->IsChildOf(USceneComponent::StaticClass()))
		{
			componentClasses.Add(*It);
			actorComponentClasses.Add(*It);
		}
		else if (It->IsChildOf(UActorComponent::StaticClass()))
		{
			actorComponentClasses.Add(*It);
		}
		else if (It->IsChildOf(UUserWidget::StaticClass()))
		{
			widgets.Add(*It);
		}
		classes.Add(*It);
	}

	if (actualWorld != world || world->GetActorCount() != actorsLevel.Num()) {
		actorsLevel.Empty();
		actualWorld = world;
		for (TObjectIterator<AActor> actor; actor; ++actor)
		{
			if (actor->GetWorld() == world) {
				actorsLevel.Add(*actor);
			}
		}
	}
}
#endif