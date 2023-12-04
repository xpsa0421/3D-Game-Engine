#pragma once
#include "BaseObject.h"

class Line : public BaseObject
{
public:
	virtual void	CreateVertexData()	override;
	virtual void	CreateIndexData()	override;
	virtual bool	Frame()				override;
	virtual bool	Render()			override;
};

class DirLine : public Line
{
public:
	virtual void	CreateVertexData()	override;
	virtual void	CreateIndexData()	override;
};