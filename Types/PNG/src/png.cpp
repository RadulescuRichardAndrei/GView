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
    if (buf.GetLength() < sizeof(uint32)) {
        return false;
    }
    auto signature = buf.GetObject<uint32>(0);
    auto result = (uint32) signature == PNG::PNG_SIGNATURE;
    return result;
}

PLUGIN_EXPORT TypeInterface* CreateInstance()
{
    return new PNG::PNGFile;
}

void CreateBufferView(Reference<GView::View::WindowInterface> win, Reference<PNG::PNGFile> png)
{

    BufferViewer::Settings settings;
    const std::vector<ColorPair> colors = { 
        ColorPair{ Color::Teal, Color::DarkBlue }, 
        ColorPair{ Color::Yellow, Color::DarkBlue }
    };
    settings.AddZone(0, 4, colors[0], "Header");
    
    //auto& data = png->GetObject()->GetData();
    //uint64 offset = 0;
    //uint32 colorIndex = 0;

    //// PNG Signature
    //settings.AddZone(0, 8, ColorPair{ Color::Magenta, Color::DarkBlue }, "PNG Signature");
    //offset += 8;

    //// Parse chunks
    //while (offset < data.GetSize()) {
    //    uint32 length, type;
    //    if (!data.Copy<uint32>(offset, length) || !data.Copy<uint32>(offset + 4, type)) {
    //        break;
    //    }
    //    length = Endian::BigToNative(length);
    //    
    //    std::string chunkName;
    //    switch (type) {
    //        case PNG::PNG_CHUNK_TYPE_IHDR: chunkName = "IHDR Chunk"; break;
    //        case PNG::PNG_CHUNK_TYPE_IDAT: chunkName = "IDAT Chunk"; break;
    //        case PNG::PNG_CHUNK_TYPE_IEND: chunkName = "IEND Chunk"; break;
    //        default: {
    //            char typeName[5] = {0};
    //            memcpy(typeName, &type, 4);
    //            chunkName = std::string(typeName) + " Chunk";
    //        }
    //    }

    //    uint32 totalLength = 12 + length; // Length(4) + Type(4) + Data(length) + CRC(4)
    //    settings.AddZone(offset, totalLength, colors[colorIndex], chunkName.c_str());
    //    offset += totalLength;
    //    colorIndex = (colorIndex + 1) % colors.size();

    //    if (type == PNG::PNG_CHUNK_TYPE_IEND) {
    //        break;
    //    }
    //}

    png->SetSelectionZoneInterface(win->GetSelectionZoneInterfaceFromViewerCreation(settings));
}

void CreateImageView(Reference<GView::View::WindowInterface> win, Reference<PNG::PNGFile> png)
{
    GView::View::ImageViewer::Settings settings;
    settings.SetLoadImageCallback(png.ToBase<View::ImageViewer::LoadImageInterface>());
    settings.AddImage(0, win->GetObject()->GetData().GetSize());
    win->CreateViewer(settings);
}

PLUGIN_EXPORT bool PopulateWindow(Reference<GView::View::WindowInterface> win)
{
    auto png = win->GetObject()->GetContentType<PNG::PNGFile>();
    png->Update();

    CreateImageView(win, png);
    CreateBufferView(win, png);
    win->AddPanel(Pointer<TabPage>(new PNG::Panels::Information(png)), true);

    return true;
}

PLUGIN_EXPORT void UpdateSettings(IniSection sect)
{
    sect["Pattern"] = "magic:89 50 4E 47";
    sect["Priority"] = 1;
    sect["Description"] = "PNG image file (*.png)";
}
}