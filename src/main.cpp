// don't change

#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>
#include "Theme.h"

extern "C" void setThemeIndex(int idx);

int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    mu::dbgLog("INFO: Application just created!");
    //load properties from OS environment (registry on windows, plist on mac, settings scheme on linux...)
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "EN");

    // Load persisted theme and apply it before initializing views
    int themeIdx = appProperties->getValue("theme", to_int(ThemeIndex::Classic)); // default to Classic
    setThemeIndex(themeIdx);
    mu::dbgLog("INFO: Theme loaded: %d", themeIdx);

    app.init(trLang);
    return app.run();
}
