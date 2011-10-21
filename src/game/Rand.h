#ifndef RAND_H
#define RAND_H

// Random number generator for game logic
class Rand
{
public:
    Rand(int seed = 42);

    // POSIX-style (1-RAND_MAX)
    int get() const;
    int operator()() { return get(); }
};

#endif // RAND_H
