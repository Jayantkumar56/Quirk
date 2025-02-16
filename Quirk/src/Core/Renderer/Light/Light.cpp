



#include "Qkpch.h"
#include "Light.h"

namespace Quirk {

	std::string_view LightTypeToString(LightType type) {
		switch (type) {
			case LightType::None:        return "None";
			case LightType::Point:		 return "Point";
			case LightType::Directional: return "Directional";
			case LightType::Spot:		 return "Spot";
		}

		return "";
	}

	LightType StringToLightType(std::string_view type) {
		if ( type == "Point"       ) { return LightType::Point;	      }
		if ( type == "Directional" ) { return LightType::Directional; }
		if ( type == "Spot"        ) { return LightType::Spot;		  }

		return LightType::None;
	}

}
