#include "UI.h"
#include <crtdbg.h>


int main()
{
	CountryRepo* repo = createRepo();
	OperationsStack* undo = createStack();
	OperationsStack* redo = createStack();
	Controller* ctrl = createController(repo, undo, redo);

	addCountryCtrl(ctrl, "Romania", "Europe", 41);
	addCountryCtrl(ctrl, "USA", "America", 220);
	addCountryCtrl(ctrl, "China", "Asia", 330);
	addCountryCtrl(ctrl, "Portugal", "Europe", 30);
	addCountryCtrl(ctrl, "Rusia", "Europe", 50);

	UI* ui = createUI(ctrl);

	startUI(ui);

	destroyUI(ui);

	_CrtDumpMemoryLeaks();

	return 0;
}