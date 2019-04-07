//Core
#include <iostream>
#include <string>
#include <stdlib.h>

//Windows
#include <windows.h>
#include <UIAutomation.h>
#include <atlbase.h>

std::wstring get_opened_firefox_url()
{
	std::wstring url;

	//Initialize UIAutomation
	CComQIPtr<IUIAutomation> uia;
	if (FAILED(uia.CoCreateInstance(CLSID_CUIAutomation)) || !uia)
	{
		std::cout << "UIAutomation could not be initialized" << std::endl;
	}

	//Condition to match to be the url element
	CComPtr<IUIAutomationCondition> condition;
	uia->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(0xC354), &condition);

	HWND hwnd = NULL;
	while (true)
	{
		hwnd = FindWindowEx(NULL, hwnd, "MozillaWindowClass", NULL);

		if (!hwnd)
		{
			break;
		}
		if (!IsWindowVisible(hwnd))
		{
			continue;
		}

		//Get the root element
		CComPtr<IUIAutomationElement> root_element;
		if (FAILED(uia->ElementFromHandle(hwnd, &root_element)) || !root_element)
		{
			continue;
		}

		//Get all Elements that match the condition
		CComPtr<IUIAutomationElementArray> matched_elements;
		if (FAILED(root_element->FindAll(TreeScope_Descendants, condition, &matched_elements)))
		{
			std::cout << "FAILED TO GET MATCHED ELEMENTS" << std::endl;
			continue;
		}

		int num_of_matched_elements = 0;
		matched_elements->get_Length(&num_of_matched_elements);

		//Check every element and check if its an url
		for (int i = 0; i < num_of_matched_elements; ++i)
		{
			CComVariant value;
			static CComPtr<IUIAutomationElement> current_element;
			current_element.Release();

			if (FAILED(matched_elements->GetElement(i, &current_element)))
			{
				continue;
			}

			//Get value of the edit element and save it if its an url
			current_element->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &value);
			wchar_t* wcp = value.bstrVal;
			std::wstring ws(wcp);
			if (ws.find(L"http") != std::string::npos || ws.find(L"www.") != std::string::npos)
			{
				url = ws;
			}
		}
	}

	return url;
}
int main(int argc, char* argv[])
{
	CoInitialize(NULL);
	std::wcout << get_opened_firefox_url() << std::endl;;
	system("pause");
	CoUninitialize();
	return 0;
}