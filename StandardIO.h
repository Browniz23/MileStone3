// 316482355 Oz Browner

#ifndef MILESTONE3_STANDARDIO_H
#define MILESTONE3_STANDARDIO_H

#include "commands.h"

/*class StandardIO: public DefaultIO {        // add virtual? * 3?
public:
    StandardIO();
    ~StandardIO() override;
    string read() override;
    void write(string text) override;               //todo: override?
    void write(float f) override;
    void read(float* f) override;
};*/
class StandardIO: public DefaultIO {        // add virtual? * 3?
public:
    StandardIO();
    ~StandardIO() override;
    string read() override {
        string text;
        cin >> text;
        return text;
    }
    void write(string text) override {
        cout << text;
    }
    void write(float f) override {
        cout << f;
    }
    void read(float* f) override {
        cin >> *f;
    }
};


#endif //MILESTONE3_STANDARDIO_H
