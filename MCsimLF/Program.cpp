#include "MCSimForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void Main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    MCsimLF::MCSimForm myForm;
    Application::Run(%myForm);
}