#define FRAMED_IMPLEMENTATION
#include "framed/framed.h"

#include "Editor/src/Application.h"

int main() {
    CWEditor::Application app;
    app.Init();
    app.Run();

    return 0;
}