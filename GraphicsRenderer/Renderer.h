#pragma once

class Renderer
{
public:
	virtual ~Renderer() {}

	virtual bool Initialise() = 0;
	virtual void Render() = 0;
	virtual void Shutdown() {};
};