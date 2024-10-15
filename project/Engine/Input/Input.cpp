#include "Input.h"
#include<assert.h>
#include<WinApp.h>
/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}
/// <summary>
/// 初期化
/// </summary>
void Input::Initialize() {
	HRESULT result;

	// DirectInoutの初期化
	//IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		WinApp::GetInstance()->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボードデバイスの生成
	//IDirectInputDevice8* keyboard = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	// 入力データの形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);// 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	/*マウスデバイスの生成*/
	result = directInput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	assert(SUCCEEDED(result));

	/*入力データの形式のセット*/
	result = devMouse->SetDataFormat(&c_dfDIMouse2);/*標準形式*/
	assert(SUCCEEDED(result));

	/*排他制御レベルのセット*/
	result = devMouse->SetCooperativeLevel(
		WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	// XInputパッドの準備（4つのパッドに対応）
	for (int i = 0; i < 4; ++i) {
		Joystick joystick = {};
		joystick.type_ = PadType::XInput;
		devJoysticks_.push_back(joystick);
	}
}
/// <summary>
/// 更新
/// </summary>
void Input::Update() {

	// 前回のキー入力を保存
	memcpy(preKey, key, sizeof(key));
	/*マウス*/
	mousePre = mouse;

	// キーボード情報の取得開始
	keyboard->Acquire();
	/*マウス*/
	devMouse->Acquire();

	// 全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);
	devMouse->GetDeviceState(sizeof(mouse), &mouse);

	// マウス位置の取得
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(WinApp::GetInstance()->GetHwnd(), &point);
	mousePosition_ = Vector2(static_cast<float>(point.x), static_cast<float>(point.y));

	// XInputコントローラーの入力取得
	for (int i = 0; i < 4; ++i) {
		Joystick& joystick = devJoysticks_[i];

		if (joystick.type_ == PadType::XInput) {
			joystick.statePre_ = joystick.state_; // 前回の状態を保存

			// XInputのステートを取得
			if (XInputGetState(i, &joystick.state_.xInput_) == ERROR_SUCCESS) {
				// XInputのコントローラーが接続されている場合の処理

				// 左スティック
				float leftThumbX = joystick.state_.xInput_.Gamepad.sThumbLX;
				float leftThumbY = joystick.state_.xInput_.Gamepad.sThumbLY;

				// 左スティックのデッドゾーン処理
				if (std::abs(leftThumbX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
					//leftThumbX = 0;
					joystick.state_.xInput_.Gamepad.sThumbLX = 0;
				}
				if (std::abs(leftThumbY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
					//leftThumbY = 0;
					joystick.state_.xInput_.Gamepad.sThumbLY = 0;
				}

				// 右スティック
				float rightThumbX = joystick.state_.xInput_.Gamepad.sThumbRX;
				float rightThumbY = joystick.state_.xInput_.Gamepad.sThumbRY;

				// 右スティックのデッドゾーン処理
				if (std::abs(rightThumbX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
					//rightThumbX = 0;
					joystick.state_.xInput_.Gamepad.sThumbRX = 0;
				}
				if (std::abs(rightThumbY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
					//rightThumbY = 0;
					joystick.state_.xInput_.Gamepad.sThumbRY = 0;
				}

				// トリガー
				BYTE leftTrigger = joystick.state_.xInput_.Gamepad.bLeftTrigger;
				BYTE rightTrigger = joystick.state_.xInput_.Gamepad.bRightTrigger;

				// トリガーのデッドゾーン処理
				if (leftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
					//leftTrigger = 0;
					joystick.state_.xInput_.Gamepad.bLeftTrigger = 0;
				}
				if (rightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
					//rightTrigger = 0;
					joystick.state_.xInput_.Gamepad.bRightTrigger = 0;
				}

			}
		}
	}
}
/// <summary>
/// キーの押下をチェック
/// </summary>
/// <param name="keyNumber">キー番号( DIK_0 等)</param>
/// <returns>押されているか</returns>
//bool PushKey(BYTE keyNumber) const;
bool Input::PushKey(uint8_t keyNumber)const {
	if (key[keyNumber]) {
		return true;
	}
	return false;
}

/// <summary>
/// キーのトリガーをチェック
/// </summary>
/// <param name="keyNumber">キー番号( DIK_0 等)</param>
/// <returns>トリガーか</returns>
//bool TriggerKey(BYTE keyNumber) const;
bool Input::TriggerKey(uint8_t keyNumber) const {
	if (key[keyNumber] && !preKey[keyNumber]) {
		return true;
	}
	return false;
}

const DIMOUSESTATE2& Input::GetAllMouse() const
{
	return mouse;
}

bool Input::IsPressMouse(int32_t mouseNumber) const
{
	return mouse.rgbButtons[mouseNumber] & 0x80;
}

bool Input::IsTriggerMouse(int32_t buttonNumber) const
{
	return (mouse.rgbButtons[buttonNumber] & 0x80) && !(mousePre.rgbButtons[buttonNumber] & 0x80);
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove move = { mouse.lX, mouse.lY, mouse.lZ };
	return move;
}

int32_t Input::GetWheel() const
{
	return mouse.lZ;
}

const Vector2& Input::GetMousePosition() const
{
	return mousePosition_;
}
bool Input::GetJoystickState(int32_t stickNo, XINPUT_STATE& out) const
{
	if (stickNo < 0 || stickNo >= devJoysticks_.size()) return false;
	if (devJoysticks_[stickNo].type_ != PadType::XInput) return false;

	out = devJoysticks_[stickNo].state_.xInput_;
	return true;
}

bool Input::GetJoystickStatePrevious(int32_t stickNo, XINPUT_STATE& out) const
{
	if (stickNo < 0 || stickNo >= devJoysticks_.size()) return false;
	if (devJoysticks_[stickNo].type_ != PadType::XInput) return false;

	out = devJoysticks_[stickNo].statePre_.xInput_;
	return true;
}

void Input::SetJoystickDeadZone(int32_t stickNo, int32_t deadZoneL, int32_t deadZoneR)
{
}

size_t Input::GetNumberOfJoysticks()
{
	return size_t();
}

bool Input::TriggerControllerButton(int32_t stickNo, WORD button)
{
	XINPUT_STATE currentState;
	XINPUT_STATE previousState;
	if (GetJoystickState(stickNo, currentState) && GetJoystickStatePrevious(stickNo, previousState)) {
		return ((currentState.Gamepad.wButtons & button) != 0) && ((previousState.Gamepad.wButtons & button) == 0);
	}
	return false;
}
