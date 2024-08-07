#include "Lumina/Application.h"
#include "Lumina/EntryPoint.h"

#include "Examples/Demo.h"
#include "Examples/Cube.h"

Lumina::Application* Lumina::CreateApplication(int argc, char** argv)
{
    Lumina::ApplicationSpecification spec;
    spec.Name = "Lumina Application";
    spec.Width = 900;
    spec.Height = 900;

    Lumina::Application* app = new Lumina::Application(spec);
    app->PushLayer<Demo>(); 
    
    return app;
}