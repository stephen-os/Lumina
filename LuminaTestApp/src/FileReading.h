#pragma once

#include <vector>
#include <string>
#include <cctype>  // for isprint
#include <imgui.h>

#include "Lumina/Core/Layer.h"
#include "Lumina/Core/Log.h"
#include "Lumina/IO/File.h"

#include "Lumina/Core/Input.h"
#include "Lumina/Core/KeyCode.h"

class FileReading : public Lumina::Layer
{
public:
    virtual void OnAttach() override {}

    virtual void OnDetach() override {}

    virtual void OnUpdate(float ts) override {}

    virtual void OnUIRender() override
    {
        static std::string textFileContent;
        static std::string binaryFileContent;
        static bool loaded = false;

        if (!loaded)
        {
            if (Lumina::File::Exists("res/files/textfile.txt"))
                textFileContent = Lumina::File::ReadText("res/shaders/Cube.vert");
            else
                textFileContent = "[textfile.txt not found]";

            if (Lumina::File::Exists("res/files/binaryfile.bin"))
                binaryFileContent = Lumina::File::ReadBinary("res/files/binaryfile.bin");
            else
                binaryFileContent = "[binaryfile.bin not found]";

            loaded = true;
        }

        ImGui::Begin("File I/O Test");

        ImGui::Text("Text File Content:");
        ImGui::InputTextMultiline("##TextFile", textFileContent.data(), textFileContent.size() + 1, ImVec2(400, 100), ImGuiInputTextFlags_ReadOnly);

        ImGui::Separator();

        ImGui::Text("Binary File Content (as characters):");
        std::string printableBinary;
        for (char c : binaryFileContent)
            printableBinary += (std::isprint(static_cast<unsigned char>(c)) ? c : '.');

        ImGui::InputTextMultiline("##BinaryFile", printableBinary.data(), printableBinary.size() + 1, ImVec2(400, 100), ImGuiInputTextFlags_ReadOnly);

        ImGui::Separator();

        if (ImGui::Button("Save Alternate Text File"))
        {
            std::string altText = "This is an alternate text file saved from ImGui.";
            Lumina::File::WriteText("res/files/alt_textfile.txt", altText);
        }

        if (ImGui::Button("Save Alternate Binary File"))
        {
            std::string altBinary = "\x42\x69\x6E\x61\x72\x79\x21"; // "Binary!"
            Lumina::File::WriteBinary("res/files/alt_binaryfile.bin", altBinary);
        }

        ImGui::End();
    }
};
