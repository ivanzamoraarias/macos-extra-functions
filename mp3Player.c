#include <stdio.h>
#include <mpg123.h>
#include <ao/ao.h>
#include <time.h>

#define BITS 8

// namas encuentra el lib y el include , estan dentro del opt/homewbrew 
// y luego corres este comando
//gcc -o your_program your_program.c -lmpg123 -I/usr/local/include -L/usr/local/lib


void printProgressBar(double progress) {
    int barWidth = 50;
    int pos = barWidth * progress;
    printf("[");
    for (int i = 0; i < barWidth; i++) {
        if (i < pos)
            printf("=");
        else
            printf(" ");
    }
    printf("] %.2f%%\r", progress * 100);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_sample_format format;
    ao_device *device;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <mp3 file>\n", argv[0]);
        return 1;
    }

    // Initialize mpg123
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    // Open the file and get the format information
    mpg123_open(mh, argv[1]);
    mpg123_getformat(mh, &driver, &format.rate, &format.channels, &format.bits);

    // Set up the audio device
    ao_initialize();
    driver = ao_default_driver_id();
    format.byte_format = AO_FMT_NATIVE;
    format.bits = BITS;

    // Open the audio device
    device = ao_open_live(driver, &format, NULL);

    // Decode and play the MP3 file
    time_t start_time = time(NULL);
    double total_duration = mpg123_length(mh) / (double)format.rate;

    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
        ao_play(device, buffer, done);

        // Calculate progress and print progress bar
        double current_time = difftime(time(NULL), start_time);
        double progress = current_time / total_duration;
        printProgressBar(progress);
    }

    // Clean up
    free(buffer);
    ao_close(device);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();

    return 0;
}
