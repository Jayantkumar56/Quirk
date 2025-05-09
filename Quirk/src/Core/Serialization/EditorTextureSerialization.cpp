

#include "Qkpch.h"

#include "EditorTextureSerialization.h"
#include "Core/Reflection/Registrations/TextureEnums.h"
#include "Core/Utility/YamlUtility.h"
#include "Core/AssetManager/AssetImporter/TextureImporter.h"

#include "yaml-cpp/yaml.h"

#include <fstream>


namespace Quirk {

    bool Serializer<EditorTexture2D>::Serialize(const EditorTexture2D& texture) {
        YAML::Emitter out;

        out << YAML::BeginMap; // root
        out << YAML::Key << "Uuid"      << YAML::Value << texture.Texture->GetHandle();
        out << YAML::Key << "ImagePath" << YAML::Value << texture.ImagePath.string();

        out << YAML::Key << "TextureProperties";
        {
            out << YAML::BeginMap;	// TextureProperties
            auto textureProp = texture.Texture->GetSpec();

            out << YAML::Key << "TextureFilter";
            {
                out << YAML::BeginMap;	// TextureFilter

                out << YAML::Key << "MinFilter";
                out << YAML::Value << EnumRegistry<decltype(textureProp.MinFilter)>::ToString(textureProp.MinFilter);

                out << YAML::Key << "MagFilter";
                out << YAML::Value << EnumRegistry<decltype(textureProp.MagFilter)>::ToString(textureProp.MagFilter);

                out << YAML::EndMap;    // TextureFilter
            }

            out << YAML::Key << "TextureWrap";
            {
                out << YAML::BeginMap;	// TextureWrap

                out << YAML::Key << "WrapS";
                out << YAML::Value << EnumRegistry<decltype(textureProp.WrapS)>::ToString(textureProp.WrapS);

                out << YAML::Key << "WrapT";
                out << YAML::Value << EnumRegistry<decltype(textureProp.WrapT)>::ToString(textureProp.WrapT);

                out << YAML::EndMap;    // TextureWrap
            }

            out << YAML::Key << "TextureSwizzle";
            {
                out << YAML::BeginMap;	// TextureSwizzle

                out << YAML::Key << "R";
                out << YAML::Value << EnumRegistry<decltype(textureProp.SwizzleMask.R)>::ToString(textureProp.SwizzleMask.R);

                out << YAML::Key << "G";
                out << YAML::Value << EnumRegistry<decltype(textureProp.SwizzleMask.G)>::ToString(textureProp.SwizzleMask.G);

                out << YAML::Key << "B";
                out << YAML::Value << EnumRegistry<decltype(textureProp.SwizzleMask.B)>::ToString(textureProp.SwizzleMask.B);

                out << YAML::Key << "A";
                out << YAML::Value << EnumRegistry<decltype(textureProp.SwizzleMask.A)>::ToString(textureProp.SwizzleMask.A);

                out << YAML::EndMap;    // TextureSwizzle
            }

            out << YAML::EndMap;	// TextureProperties
        }

        out << YAML::EndMap;   // root

        std::ofstream fout(texture.TexturePath);
        fout << out.c_str();

        return true;
    }

    bool Deserializer<EditorTexture2D>::Deserialize(EditorTexture2D& outTexture) {
        YAML::Node textureData;

        try {
            textureData = YAML::LoadFile(outTexture.TexturePath.string());
        }
        catch (YAML::ParserException e) {
            QK_CORE_ERROR("Failed to load project file");
            return false;
        }

        if (!textureData["Uuid"])
            return false;

        AssetHandle handle{ textureData["Uuid"].as< uint64_t>() };

        if (!textureData["ImagePath"])
            return false;

        outTexture.ImagePath = textureData["ImagePath"].as<std::string>();

        if (!textureData["TextureProperties"])
            return false;
        
        const YAML::Node& texPropsNode = textureData["TextureProperties"];

        const YAML::Node& filterNode  = texPropsNode["TextureSwizzle"];
        const YAML::Node& wrapNode    = texPropsNode["TextureSwizzle"];
        const YAML::Node& swizzleNode = texPropsNode["TextureSwizzle"];

        TextureProperties texProps{
            .MinFilter  { EnumRegistry<decltype(texProps.MinFilter)>::FromString(filterNode["MinFilter"].as<std::string>()) },
            .MagFilter  { EnumRegistry<decltype(texProps.MagFilter)>::FromString(filterNode["MagFilter"].as<std::string>()) },
            .WrapS      { EnumRegistry<decltype(texProps.WrapS    )>::FromString(wrapNode  ["WrapS"    ].as<std::string>()) },
            .WrapT      { EnumRegistry<decltype(texProps.WrapT    )>::FromString(wrapNode  ["WrapT"    ].as<std::string>()) },
            .SwizzleMask{
                .R { EnumRegistry<decltype(texProps.SwizzleMask.A)>::FromString(swizzleNode["A"].as<std::string>()) },
                .G { EnumRegistry<decltype(texProps.SwizzleMask.G)>::FromString(swizzleNode["G"].as<std::string>()) },
                .B { EnumRegistry<decltype(texProps.SwizzleMask.B)>::FromString(swizzleNode["B"].as<std::string>()) },
                .A { EnumRegistry<decltype(texProps.SwizzleMask.A)>::FromString(swizzleNode["A"].as<std::string>()) }
            }
        };

        outTexture.Texture = TextureImporter::CreateFromImage(handle, outTexture.ImagePath, texProps);
        return true;
    }

}

