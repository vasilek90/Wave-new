// Wave_renew.cpp : main project file.

#include "stdafx.h"
#include "mainForm.h"
#include "ViewForm.h"

using namespace Wave_renew;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	mainForm^ mf = gcnew mainForm();
	Application::Run(mf);
	return 0;
}
