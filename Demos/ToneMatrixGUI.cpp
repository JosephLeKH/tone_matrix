#include "ToneMatrix.h"
#include "AudioSystem.h"
#include "GUI/MiniGUI.h"
#include "DrawRectangle.h"
#include "gthread.h"
#include <algorithm>
using namespace std;

namespace {
    const Color kBackgroundColor(0x10, 0x10, 0x10);

    /* Minimum size of a light. */
    const int kMinSize = 5;

    class GUI: public ProblemHandler {
    public:
        GUI(GWindow& window) : ProblemHandler(window) {
            /* This is a standard high-fidelity sample rate. */
            AudioSystem::setSampleRate(44100);

            /* Figure out the size of the biggest square that
             * (1) fits into the window and
             * (2) has a size that's a multiple of 16.
             */
            int minDimension = min(window.getCanvasWidth(), window.getCanvasHeight());

            cellSize = max(kMinSize, minDimension / 16);
            matrix = new ToneMatrix(cellSize);

            baseX = (window.getCanvasWidth()  - cellSize * 16) / 2;
            baseY = (window.getCanvasHeight() - cellSize * 16) / 2;

            setDrawFunction([&](Rectangle bounds, Color color) {
                window.setColor(color.toRGB());
                window.fillRect(bounds.x + baseX, bounds.y + baseY, bounds.width, bounds.height);

                window.setColor(Color(color.red() / 2, color.green() / 2, color.blue() / 2).toRGB());
                window.drawRect(bounds.x + baseX, bounds.y + baseY, bounds.width, bounds.height);
            });

            AudioSystem::play([=](double* buffer, int toRead) {
                for (int i = 0; i < toRead; i++) {
                    buffer[i] = matrix->nextSample();
                }
            });
        }

        ~GUI() {
            AudioSystem::stop();
            delete matrix;
            setDrawFunction(nullptr);
        }

        void mousePressed(double x, double y) override {
            int mouseX = x - baseX;
            int mouseY = y - baseY;
            if (mouseX >= 0 && mouseY >= 0 && mouseX < cellSize * 16 && mouseY < cellSize * 16) {
                GThread::runOnQtGuiThread([&] {
                    matrix->mousePressed(mouseX, mouseY);
                });
                requestRepaint();
            }
        }

        void mouseDragged(double x, double y) override {
            int mouseX = x - baseX;
            int mouseY = y - baseY;
            if (mouseX >= 0 && mouseY >= 0 && mouseX < cellSize * 16 && mouseY < cellSize * 16) {
                GThread::runOnQtGuiThread([&] {
                    matrix->mouseDragged(mouseX, mouseY);
                });
                requestRepaint();
            }
        }

        void repaint() override {
            window().clearCanvas();
            window().setColor(kBackgroundColor.toRGB());
            window().fillRect(canvasBounds());
            matrix->draw();
        }

    private:
        int baseX, baseY, cellSize;

        ToneMatrix* matrix;
    };
}

GRAPHICS_HANDLER("Tone Matrix", GWindow& window) {
    return make_shared<GUI>(window);
}
