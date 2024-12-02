
#include "Qkpch.h"
#include "ImguiUIUtility.h"


namespace Quirk { namespace ImguiUI { namespace Utility {

	void TextCentered(const char* text) {
		float cell_width = ImGui::GetContentRegionAvail().x;
		ImVec2 textSize  = ImGui::CalcTextSize(text);
		float offset	 = (cell_width - textSize.x) * 0.5f;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
		ImGui::Text(text);
	}

	void TextWrappedCentered(const char* text) {
		float cell_width = ImGui::GetContentRegionAvail().x;
		ImVec2 textSize  = ImGui::CalcTextSize(text);
		float offset     = (cell_width - textSize.x) * 0.5f;

		if(offset > 0) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
		ImGui::TextWrapped(text);
	}

	bool DrawFloat3(const char* label, float* data, float resetValue, float speed, float labelColumnWidth,
					ImFont* lableFont, ImFont* buttonFont, ImFont* valuesFont) 
	{
		float min = 0.0f, max = 0.0f;
		bool valueChanged = false;
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y;

		if (ImGui::BeginTable("transform", 4)) {

			ImGui::TableNextColumn();
			ImGui::PushFont(lableFont);
			ImGui::AlignTextToFramePadding();
			ImGui::Text(label);
			ImGui::PopFont();

			ImGui::TableNextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.769f, 0.125f, 0.129f, 1.0f });
			ImGui::PushFont(buttonFont);
			if (ImGui::Button("X", { lineHeight, lineHeight })) {
				valueChanged = true;
				data[0] = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor();

			ImGui::SameLine(0.0f, 0.0f);

			ImGui::PushFont(valuesFont);
			valueChanged |= ImGui::DragFloat("##X", data, speed, min, max, "%.2f");
			ImGui::PopFont();

			ImGui::TableNextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.596f, 0.808f, 0.0f, 1.0f });
			ImGui::PushFont(buttonFont);
			if (ImGui::Button("Y", { lineHeight, lineHeight })) {
				valueChanged = true;
				data[1] = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor();

			ImGui::SameLine(0.0f, 0.0f);

			ImGui::PushFont(valuesFont);
			valueChanged |= ImGui::DragFloat("##Y", data + 1, speed, min, max, "%.2f");
			ImGui::PopFont();

			ImGui::TableNextColumn();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.031f, 0.239f, 0.467f, 1.0f });
			ImGui::PushFont(buttonFont);
			if (ImGui::Button("Z", { lineHeight, lineHeight })) {
				valueChanged = true;
				data[2] = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor();

			ImGui::SameLine(0.0f, 0.0f);

			ImGui::PushFont(valuesFont);
			valueChanged |= ImGui::DragFloat("##Z", data + 2, speed, min, max, "%.2f");
			ImGui::PopFont();

			ImGui::EndTable();
		}
		return valueChanged;
	}

}}}
