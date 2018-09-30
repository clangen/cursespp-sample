//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007-2017 musikcube team
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////

#include <cursespp/App.h>
#include <cursespp/Screen.h>
#include <cursespp/TextLabel.h>
#include <cursespp/LayoutBase.h>
#include <cursespp/IViewRoot.h>

#include <boost/locale.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/detail/utf8_codecvt_facet.hpp>

#ifdef WIN32
#include <cursespp/Win32Util.h>
#undef MOUSE_MOVED
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

static const std::string APP_NAME = "cursespp-sample";

using namespace cursespp;

#define MIN_WIDTH 54
#define MIN_HEIGHT 12

class MainLayout: public LayoutBase, public IViewRoot {
    public:
        MainLayout() : LayoutBase() {
            this->label = std::make_shared<TextLabel>();
            this->label->SetText("hello, world", text::AlignCenter);
            this->SetFrameVisible(true);
            this->SetFrameTitle(APP_NAME);
            this->AddWindow(label);
        }

        virtual void ResizeToViewport() override {
            this->MoveAndResize(0, 0, Screen::GetWidth(), Screen::GetHeight());
        }

        virtual void OnLayout() override {
            this->label->MoveAndResize(0, this->GetContentHeight() / 2, this->GetContentWidth(), 1);
        }

    private:
        std::shared_ptr<TextLabel> label;
};

#ifdef WIN32
int start(int argc, wchar_t* argv[]);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {
    return App::Running(APP_NAME) ? 0 : start(0, 0);
}

int start(int argc, wchar_t* argv[]) {
#else
int main(int argc, char* argv[]) {
#endif
    srand((unsigned int)time(0));

    /* the following allows boost::filesystem to use utf8 on Windows */
    std::locale locale = std::locale();
    std::locale utf8Locale(locale, new boost::filesystem::detail::utf8_codecvt_facet);
    boost::filesystem::path::imbue(utf8Locale);

#ifdef WIN32
    PDC_set_resize_limits(MIN_HEIGHT, 1000, MIN_WIDTH, 1000);
    resize_term(26, 100); /* must be before app init */
    Colors::Mode colorMode = Colors::RGB;
#else
    Colors::Mode colorMode = Colors::Palette;
#endif

    App app(APP_NAME); /* must be before layout creation */

    app.SetColorMode(colorMode);
    app.SetMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    app.SetKeyHandler([&](const std::string& kn) -> bool {
        return false;
    });

    app.Run(std::make_shared<MainLayout>());

    return 0;
}
