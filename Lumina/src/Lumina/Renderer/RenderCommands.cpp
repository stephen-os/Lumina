#include "RenderCommands.h"

#include "Commands.h"

namespace Lumina
{
	Shared<Commands> RenderCommands::s_Commands;

	void RenderCommands::Init()
	{
		s_Commands = Commands::Create();
	}

	void RenderCommands::SetViewport(int x, int y, int width, int height)
	{
		s_Commands->SetViewport(x, y, width, height);
	}

	void RenderCommands::Clear()
	{
		s_Commands->Clear();
	}

	void RenderCommands::EnableDepthTest()
	{
		s_Commands->EnableDepthTest();
	}
}