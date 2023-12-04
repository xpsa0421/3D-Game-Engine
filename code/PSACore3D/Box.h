#pragma once
#include "Line.h"

class Box : public BaseObject
{
public:
	Vector		_pos;
	Vector		_look;
	Vector		_up;
	Vector		_right; 
	DirLine*	_dirLine = nullptr;
public:
	void			SetVertexPoints();
	void			SetVertexNormals();
	void			SetVertexColours();
	void			SetVertexTextures();
	virtual void	CreateVertexData() override;
	virtual void	CreateIndexData() override;
	virtual void	SetTransformationMatrix(Matrix* worldMat, Matrix* viewMat, Matrix* projMat)	override;
	void			Scale(float x, float y, float z);
public:
	virtual bool	Init()		override;
	virtual bool	Frame()		override;
	virtual bool	Render()	override;
	virtual bool	Release()	override;
};

