#pragma once
#include "Device.h"
#include "Input.h"
#include "GlobalGameTimer.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
#include "Writer.h"
#include "RenderTarget.h"

class GameCore : public Device
{
public:
	bool			_isGameRun = true;	
	bool			_isWireFrame = false;
	Writer			_writer;
	BaseObject		_background;
	RenderTarget	_renderTarget;
public:
	virtual bool	Init()		{ return true; }; 
	virtual bool	Frame()		{ return true; }; 
	virtual bool	Render()	{ return true; }; 
	virtual bool	Release()	{ return true; };
private:
	bool	CoreInit(); 
	bool	CoreFrame();
	bool	CorePreRender();
	bool	CoreRender();
	bool	CorePostRender();
	bool	CoreRelease();
public:
	virtual HRESULT	CreateDXResource();
	virtual HRESULT	DeleteDXResource();
	void	ClearD3D11DeviceContext();
	bool	Run() override;
};
