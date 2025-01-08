#include "png.hpp"

using namespace AppCUI;
using namespace AppCUI::Utils;
using namespace AppCUI::Application;
using namespace AppCUI::Controls;
using namespace GView::Utils;
using namespace GView::Type;
using namespace GView;
using namespace GView::View;

extern "C"
{
PLUGIN_EXPORT bool Validate(const AppCUI::Utils::BufferView& buf, const std::string_view& extension)
{
    // Check PNG signature
    static constexpr uint8_t PNG_SIGNATURE[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    if (buf.GetLength() < sizeof(PNG_SIGNATURE))
        return false;
    return memcmp(buf.GetData(), PNG_SIGNATURE, sizeof(PNG_SIGNATURE)) == 0;
}

PLUGIN_EXPORT TypeInterface* CreateInstance()
{
    return new PNG::PNGFile;
}

void CreateBufferView(Reference<GView::View::WindowInterface> win, Reference<PNG::PNGFile> png)
{
    BufferViewer::Settings settings;
    auto& data = png->obj->GetData();
    const uint64 dataSize = data.GetSize();

    // PNG Header (first 8 bytes)
    settings.AddZone(0, 8, ColorPair{ Color::Magenta, Color::DarkBlue }, "PNG Signature");

    // Parse chunks (IHDR, IDAT, etc.)
    uint64 offset = 8;
    while (offset + 12 <= dataSize) // Minimum chunk size
    {
        uint32 chunkLength, chunkType;
        if (!data.Copy(offset, chunkLength) || !data.Copy(offset + 4, chunkType))
            break;

        chunkLength = Endian::BigToNative(chunkLength);
        std::string chunkLabel = "Chunk: " + std::string(reinterpret_cast<char*>(&chunkType), 4);

        settings.AddZone(offset, 8 + chunkLength + 4, ColorPair{ Color::Yellow, Color::DarkBlue }, chunkLabel.c_str());
        offset += 8 + chunkLength + 4; // Move to the next chunk
    }

    win->CreateViewer(settings);
}

void CreateImageView(Reference<GView::View::WindowInterface> win, Reference<PNG::PNGFile> png)
{
    GView::View::ImageViewer::Settings settings;
    settings.SetLoadImageCallback(png.ToBase<View::ImageViewer::LoadImageInterface>());
    settings.AddImage(0, png->obj->GetData().GetSize());
    win->CreateViewer(settings);
}

PLUGIN_EXPORT bool PopulateWindow(Reference<GView::View::WindowInterface> win)
{
    auto png = win->GetObject()->GetContentType<PNG::PNGFile>();
    png->Update();

    // Add viewers
    CreateImageView(win, png);
    CreateBufferView(win, png);

    // Add panels
    win->AddPanel(Pointer<TabPage>(new PNG::Panels::Information(png)), true);

    return true;
}

PLUGIN_EXPORT void UpdateSettings(IniSection sect)
{
    sect["Pattern"]     = "magic:89 50 4E 47 0D 0A 1A 0A";
    sect["Priority"]    = 1;
    sect["Description"] = "PNG image file (*.png)";
}
}

int main()
{
    return 0;
}
