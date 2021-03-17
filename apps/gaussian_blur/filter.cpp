#include <cassert>
#include <cstdio>
#include <cstdlib>

#include "HalideBuffer.h"
#include "HalideRuntime.h"

#include "gaussian_blur.h"
#include "gaussian_blur_auto_schedule.h"
#include "gaussian_blur_direct.h"
#include "gaussian_blur_direct_auto_schedule.h"

#include "halide_benchmark.h"
#include "halide_image_io.h"

using namespace Halide::Tools;

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s in out\n", argv[0]);
        return 1;
    }

    Halide::Runtime::Buffer<uint8_t> input = load_and_convert_image(argv[1]);
    Halide::Runtime::Buffer<uint8_t> output(input.width(), input.height(), input.channels());

    for (int r = 1; r < 20; r++) {
        double best_manual = benchmark([&]() {
            gaussian_blur_direct(input, r, output);
            output.device_sync();
        });
        printf("Manually-tuned time (direct) (%d): %gms\n", r, best_manual * 1e3);
    }

    for (int r = 1; r < 20; r++) {
        double best_manual = benchmark([&]() {
            gaussian_blur(input, r, output);
            output.device_sync();
        });
        printf("Manually-tuned time (IIR) (%d): %gms\n", r, best_manual * 1e3);
    }

    convert_and_save_image(output, argv[2]);

    printf("Success!\n");
    return 0;
}
