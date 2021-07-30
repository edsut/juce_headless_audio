/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
#include "justaudio.h"

#define SPKRPHONE   "RODIO, USB Audio; Direct hardware device without any conversions"

#define MICTOT    2
#define SPKRTOT   2
#define AUDIO_DEVNAME SPKRPHONE

#define HWBUFSIZ        1024
#define SAMPFREQ        48000

//==============================================================================
class justaudioApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    justaudioApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    justaudio justaudiomain;

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        struct justaudioinfo justaudiostuff;
        juce::String cfgfile = juce::String("./parameters.config");
        bool headless = juce::Desktop::getInstance().isHeadless();

        // Set up all the configuration defaults:
        justaudiostuff.bufsz = HWBUFSIZ;
        justaudiostuff.sampfreq = SAMPFREQ;
        justaudiostuff.mictot = MICTOT;
        justaudiostuff.spkrtot = SPKRTOT;
        justaudiostuff.devname = AUDIO_DEVNAME;

        // Process incoming command line parameters:
        juce::StringArray clis = juce::JUCEApplicationBase::getCommandLineParameterArray();
        for(int i=0;i<clis.size();i++) {
            if (clis[i].compare("headless") == 0) {
                if (headless == false)
                    std::cout << "Running in headless mode" << std::endl;
                headless = true;
            }
            else if (clis[i].compare("version") == 0) {
                std::cout << "JUCE vsn " << JUCE_MAJOR_VERSION << "." << JUCE_MINOR_VERSION << "." << JUCE_BUILDNUMBER << std::endl;
                exit(1);
            }
            else if (clis[i].startsWith("devname=")) {
                justaudiostuff.devname = clis[i].substring(8);
            }
            else {
                if (clis[i].compare("help") != 0)
                    std::cerr << "Unexpected command line argument: '" << clis[i] << "'" << std::endl;
                std::cerr << "Usage: " << getApplicationName() << " {cmd | param=val} [cmd | param=val] ..." << std::endl;
                std::cerr << " Cmds:" << std::endl;
                std::cerr << "   headless                # Force headless mode" << std::endl;
                std::cerr << "   help                    # Display this output" << std::endl;
                std::cerr << "   juceversion             # Display version of JUCE" << std::endl;
                std::cerr << " Params:" << std::endl;
                std::cerr << "   devname={name}          # Override deafult device name" << std::endl;
                std::cerr << std::endl;
                shutdown();
            }
        }

        if (headless == false) {
            mainWindow.reset (new MainWindow (getApplicationName()));
        }

        if (justaudiomain.init(&justaudiostuff) == false) {
            std::cerr << "Audio device initialization failed" << std::endl;
            shutdown();
        }
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr;
        justaudiomain.shutdown();
        exit(1);
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (justaudioApplication)
